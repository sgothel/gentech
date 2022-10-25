// author : Sven Gothel - Stapenhorststrasse 35a - 33615 Bielefeld
//
// gliste.h && gliste.cc
// hashlist.h
// DATUM				 : 11.09.1994
//
// Jegliches Copyright aller Dateien ist im Besitz der Autoren.
// Sven Gothel * Stapenhorststr.35a * 33615 Bielefeld * 0521/139228
// Bielefeld, den 22.8.1994.
/*
 *
 *  $Log: hashlist.h $
 *  Revision 1.4  1995/04/07 16:54:46  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.3  1995/04/04 10:37:22  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.2  1994/12/15 15:27:50  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.1  1994/12/15 15:18:35  Sven_Gothel
 *  Initial revision
 *
*/

# ifndef _HASHLISTE_H
  # define _HASHLISTE_H

  # include "sortlist.h"
  # include "interror.h"
  # ifdef __BORLANDC__
	# ifndef NO_HEAP_CHECK
	  # include <alloc.h>
	  # define HEAP_CHECK heapcheck
	# else
	  # define HEAP_CHECK() 1
	# endif
  # else
	# define HEAP_CHECK() 1
  # endif
  # include <cmath>

  // Die HashListe enthaelt ein Array von SortListe<int> !!!
  // und ist eine ganz virtuelle Klasse (pure virtual).
  // D.h. die Klasse , die von dieser abgeleitet wird, muss
  // die Funktion : HashFunktion mit genau den selben Prototypen
  // zur Verfuegung stellen !!!
  //
  // Vom Typ int wird erwartet, dass er einen Zuweisungoperator,
  // und einen Copykonstruktor hat.
  //
  // Mittels einer eigenen Hash Funktion koennen vor der Sortierung
  // die Elemente in das zugewiesene Hash-Feld unterkommen.
  // Eine Geschwindigkeitssteigerung um den Faktor 5 bis 6 werden
  // hierdurch fuer die Suche erwartet.
  // Die HashListe Simuliert eine normale Liste.
  // Somit kann diese Liste wie die einfache Liste, oder die
  // SortListe als lineares dynamisches Array behanddelt werden.
  // Dieses lineare Hash-Array ist natuerlich nicht mehr sortiert,
  // da es jau mittels uebergebenden Hash-Algorithmus
  // definiert wurde. Die gehashten SortListen selber sind natuerlich
  // weiterhin sortiert. ALSO : siehe Testprogramm.
  // Siehe das Testprogramm am Ende diese Files.......
  //
  // fuegeEin liefert einen linearen Index zurueck,
  // an welcher Position das Element eingefuegt wurde.
  //
  // findeElement(const int&) sucht mittels Hashing und Bisektionierung
  // nach dem uebergebenden Element und liefert seine lineare
  // Position zurueck.
  //
  // loesche(int i) loescht mittels linearen index i, ein Element
  // aus der Hash-Tabelle, der SortListe<int>...
  //
  // operator[](int) nimmt als Argument einen linearen Index,
  // mit welchen er auf das ge-haschte - listensortierte Element
  // zugreifen kann.
  //
  // HashListe& operator=(const HashListe &m)
  // Der Zuwisungsoperator
  //
  // laenge() liefert die Anzahl der Element-Eintraege.
  //

  template<class Tmp> class HashListe {

  friend ostream& operator<< (ostream& OS, HashListe<Tmp>& List);

  public:
	HashListe( int size = 0 ) ;
	HashListe( const HashListe &m );
	virtual ~HashListe( void ) { Dest(); };
	Tmp& operator[](int i) const ;
	HashListe& operator=(const HashListe &m);
	int laenge() const { return anzahl; }
	int fuegeEin( const Tmp& );
	int loesche( int i );
	int findeElement( const Tmp& ) const ;
	virtual int HashFunktion(const Tmp &a) const = NULL;
        virtual void Ausgabe (ostream&);

  private:
	void AktualisiereAnzahlElementePlus(int HPos);
	void AktualisiereAnzahlElementeMinus(int HPos);
	int HoleLinearerIndex(int HPos, int SPos) const ;
	int HoleHashNSortPosition ( int LinearerIndex,
								int &HPos,
								int &SortPos
							  ) const ;
	void Copy(const HashListe &);
	void Dest();
	SortListe<Tmp> **HashArray; // Ein Array von Pointern .....
	int * AnzahlElemente;
	int Size;
	int anzahl;
  };

  template<class Tmp> void HashListe<Tmp>::Copy(const HashListe<Tmp> &m)
  {
	 anzahl=m.anzahl;
	 Size=m.Size;

	 int i;
	 assert(HashArray==NULL);
	 assert(HEAP_CHECK()>=1);

	 if(Size>0)
	 {
	   if( ( HashArray=new SortListe<Tmp>*[Size] ) == NULL )
	   {
		  fprintf(stderr, "<HashListe> ERROR : Es wurde versucht ein HashArray\n");
		  fprintf(stderr, "<HashListe> mit der Groesse %d zu eroeffnen.\n", Size);
		  INT_ERR(__LINE__);
	   }
	   if( ( AnzahlElemente=new int[Size] ) == NULL )
	   {
		  fprintf(stderr, "<HashListe> ERROR : Es wurde versucht ein int-Array\n");
		  fprintf(stderr, "<HashListe> fuer internen Gebrauch mit der Groesse %d zu eroeffnen.\n", Size);
		  INT_ERR(__LINE__);
	   }

	   // Initialisierung .. Copy
	   for( i=0; i<Size; i++ )
	   {
		 if(m.HashArray[i]!=NULL)
		 {
		   HashArray[i]=new SortListe<Tmp>(*m.HashArray[i]);
		   if(HashArray[i]==NULL) INT_ERR(__LINE__);
		 } else HashArray[i]=NULL;
		 AnzahlElemente[i]=m.AnzahlElemente[i];
	   }
	   assert(HEAP_CHECK()>=1);
	 }
  }

  template<class Tmp> void HashListe<Tmp>::Dest()
  {
	 assert ( ( HashArray && AnzahlElemente ) ||
			  ( !HashArray && !AnzahlElemente )
			);
	 if( HashArray && AnzahlElemente )
	 {
	   for(int i=0; i<Size; i++ )
	   {
		 assert(HEAP_CHECK()>=1);
		 if(HashArray[i]!=NULL)
		   delete HashArray[i];
	   }
	   assert(HEAP_CHECK()>=1);
	   delete[] HashArray;
	   HashArray=NULL;
	   delete[] AnzahlElemente;
	   AnzahlElemente=NULL;
	   assert(HEAP_CHECK()>=1);
	 }
  }

  template<class Tmp> HashListe<Tmp>::HashListe( int size )
  : Size(size), anzahl(0), HashArray(NULL), AnzahlElemente(NULL)
  {
	 assert(HashArray==NULL);
	 assert(HEAP_CHECK()>=1);
	 if(size>0)
	 {
	   if( ( HashArray= new SortListe<Tmp>*[Size] ) == NULL )
	   {
		  fprintf(stderr, "<HashListe> ERROR : Es wurde versucht ein HashArray\n");
		  fprintf(stderr, "<HashListe> mit der Groesse %d zu eroeffnen.\n", Size);
		  INT_ERR(__LINE__);
	   }
	   assert( HashArray!=NULL );
	   assert(HEAP_CHECK()>=1);
	   if( ( AnzahlElemente=new int[Size] ) == NULL )
	   {
		  fprintf(stderr, "<HashListe> ERROR : Es wurde versucht ein int-Array\n");
		  fprintf(stderr, "<HashListe> fuer internen Gebrauch mit der Groesse %d zu eroeffnen.\n", Size);
		  INT_ERR(__LINE__);
	   }
	   assert(HEAP_CHECK()>=1);
	   // initialisierung
	   for ( int i=0; i<Size; i++ )
	   {
		 HashArray[i]=NULL;
		 AnzahlElemente[i]=0;
	   }
	   assert(HEAP_CHECK()>=1);
	 }
  }

  template<class Tmp> HashListe<Tmp>::HashListe( const HashListe<Tmp> &m )
  : HashArray(NULL), AnzahlElemente(NULL), anzahl(0), Size(0)
  { Copy(m); }

  template<class Tmp> HashListe<Tmp>& HashListe<Tmp>::operator=(const HashListe<Tmp> &m)
  {
	if (this == &m)
	   return *this;
	Dest();
	Copy(m);
	return *this;
  }

  template<class Tmp> Tmp& HashListe<Tmp>::operator[](int i) const
  {
	 if(Size<=0) INT_ERR(__LINE__);

	 int HPos, SPos;
	 if ( i>=anzahl || HoleHashNSortPosition ( i, HPos, SPos ) <= 0 )
	 {
		fprintf(stderr, "<HashListe> ERROR : Es wurde versucht auf das Element mit dem Index %d\n", i);
		fprintf(stderr, "<HashListe> zuzugreifen. Die HashListe enthaelt aber nur %d Elemente.\n", anzahl);
		INT_ERR(__LINE__);
	 }
	 assert(HEAP_CHECK()>=1);
	 assert(HashArray[HPos]!=NULL);
	 return (*HashArray[HPos])[SPos];
  }

  template<class Tmp> int HashListe<Tmp>::fuegeEin(const Tmp& a)
  {
	 if(Size<=0) INT_ERR(__LINE__);

	 int HPos=abs(HashFunktion(a)%Size);
	 assert (HPos>=0);
	 assert (HPos<Size);
	 assert(HEAP_CHECK()>=1);
	 if(HashArray[HPos]==NULL)
	 {
	   // SortListe<Tmp> Erzeugen
	   HashArray[HPos]=new SortListe<Tmp>();
	   if( HashArray[HPos] == NULL ) INT_ERR(__LINE__);
	 }
	 int SPos=HashArray[HPos]->fuegeEin(a);
	 assert(HEAP_CHECK()>=1);
	 AktualisiereAnzahlElementePlus(HPos);
	 anzahl++;
	 return HoleLinearerIndex(HPos, SPos);
  }

  template<class Tmp> int HashListe<Tmp>::loesche(int i)
  {
	 if(Size<=0) INT_ERR(__LINE__);

	 int HPos, SPos;
	 if ( i>=anzahl || HoleHashNSortPosition ( i, HPos, SPos ) <= 0 )
	 {
		fprintf(stderr, "<HashListe> ERROR : Es wurde versucht das Element mit dem Index %d zu loesche.\n", i);
		fprintf(stderr, "<HashListe> Die HashListe enthaelt aber nur %d Elemente.\n", anzahl);
		INT_ERR(__LINE__);
	 }
	 assert(HEAP_CHECK()>=1);
	 assert(HashArray[HPos]!=NULL);
	 if(SPos>=HashArray[HPos]->laenge()) INT_ERR(__LINE__);
	 HashArray[HPos]->loesche(SPos);
	 assert(HEAP_CHECK()>=1);
	 AktualisiereAnzahlElementeMinus(HPos);
	 anzahl--;
	 return 1;
  }

  template<class Tmp> int HashListe<Tmp>::findeElement(const Tmp& x) const
  {
	 if(Size<=0) INT_ERR(__LINE__);

	 int HPos=abs(HashFunktion(x)%Size);
	 assert (HPos>=0);
	 assert (HPos>=0);
	 if(HashArray[HPos]!=NULL)
	 {
	   int SPos=HashArray[HPos]->findeElement(x);
	   if(SPos>=0) return HoleLinearerIndex(HPos, SPos);
	 }
	 return -1;
  }

  template<class Tmp>
  int HashListe<Tmp>::HoleLinearerIndex(int HPos, int SPos) const
  {
	 int LinearePos=SPos;
	 if(--HPos>=0) LinearePos+=AnzahlElemente[HPos];
	 return LinearePos;
  }

  template<class Tmp>
  void HashListe<Tmp>::AktualisiereAnzahlElementePlus(int HPos)
  {
	while(HPos<Size) AnzahlElemente[HPos++]++;
  }

  template<class Tmp>
  void HashListe<Tmp>::AktualisiereAnzahlElementeMinus(int HPos)
  {
	while(HPos<Size) AnzahlElemente[HPos++]--;
  }


  template<class Tmp>
	 int HashListe<Tmp>::HoleHashNSortPosition ( int LinearerIndex,
												 int &HPos,
												 int &SortPos
											   ) const
  {
	 int u=0, o=Size-1;
	 int i=0;
	 int done=0;
	 int LinearNumber=LinearerIndex+1;

	 // Ein Array von [0..Size-1]
	 // Untere Grenze Testen ....
	 if ( AnzahlElemente[0] >= LinearNumber )
	 {
	   o=0;
	   done=1;
	 }

	 // bisektionieren bis kein Element zwischen den grenzen
	 while ( !done && o-u>1)
	 {
	   i=(u+o)/2;
	   if ( AnzahlElemente[i] < LinearNumber )
	   {
		 u=i;
	   }
	   else if ( AnzahlElemente[i] >= LinearNumber )
	   {
		 o=i;
	   }
	 }

	 HPos=o;
	 SortPos = LinearerIndex - ( o>0 ? AnzahlElemente[o-1] : 0 ) ;
	 return 1;
  }

    template<class T> void HashListe<T>::Ausgabe (ostream& OS)
    {
      const Zeilenlaenge = 10;
  
      OS << "( " << laenge() << " ) < ";
      for (int i=0; i<laenge(); i++) {
        if (i % Zeilenlaenge == 0 &&  i)   OS << "\n\t";
  
        OS << (*this)[i];
        if (i % Zeilenlaenge != Zeilenlaenge-1 && i != laenge()-1)
  	  OS << ", ";
      }
      OS << " >";
    }
  
    template<class T>
    ostream& operator<< (ostream& OS, HashListe<T>& List)
    {
      List.Ausgabe (OS);
      return OS;
    }

  /*
  // Hier ist ein Testprogramm fuer die ganz virtuelle Klasse HashListe !
  // TESTEN !!!!!!

  # include "strclass.h"

  class HashStr : public HashListe<StrClass> {
  public:
	HashStr(int S=1) : HashListe<StrClass>(S) {}
	HashStr(const HashStr &m) : HashListe<StrClass>(m) {}
	virtual ~HashStr() {}
	virtual int HashFunktion(const StrClass &Str) const;
  };

  int HashStr::HashFunktion(const StrClass &Str) const
  {
	const char * const str=Str.GetStrPtr();
	int last = strlen(str)-1;

	if (last<=2) return(str[0]);
	if (str[last]=='2'){
		return((str[0]<<0)^(str[1]<<5)^(str[2]<<11)^(str[last-2]<<13));
	} else {
		return((str[0]<<0)^(str[1]<<5)^(str[2]<<11)^(str[last]<<13));
	}
  }


  int main (void)
  {
	 int j, i;
	 HashStr oh(111);
	 HashStr ih(oh);
	 char buffer[200];

	 for(i=0; i<20; i++)
	 {
		sprintf(buffer,"%d", i);
		StrClass str(buffer);
		j=ih.fuegeEin(str);
		printf("Habe Eingefuegt : %s, == %s ???\n",
		  str.GetStrPtr(), ih[j].GetStrPtr()
		);
		printf("Eingefuegtes hat den Index %d zurueckgegeben.\n", j);
		printf("Nach der Suche wurde der Index %d angegeben.\n",
		  ih.findeElement(str)
		);
	 }
	 oh=ih; // ACHTUNG DESTRUKTOR AUFRUF => ZEIT !
	 for(i=0;i<20; i++)
	 {
	   printf(" ih[%d]=%s  == oh[%d]=%s \n",
		 i, ih[i].GetStrPtr(), i, oh[i].GetStrPtr()
	   );
	 }
	 for(i=19;i>=0; i--)
	 {
	   printf("Versuche ih[%d] zu loeschen.\n", i);
	   ih.loesche(i);
	   printf("Versuche oh[%d] zu loeschen.\n", i);
	   oh.loesche(i);
	 }

	 // Zeittests ::

	 HashStr hl(111);

	 for(i=0; i<20; i++)
	 {
		sprintf(buffer,"%d", i);
		StrClass str(buffer);
		j=hl.fuegeEin(str);
		if(j!=hl.findeElement(str)) INT_ERR(__LINE__);
	 }

	 SortListe<StrClass> sl;

	 for(i=0; i<20; i++)
	 {
		sprintf(buffer,"%d", i);
		StrClass str(buffer);
		j=sl.fuegeEin(str);
		if(j!=sl.findeElement(str)) INT_ERR(__LINE__);
	 }


	 printf("Test erfolgreich ueberstanden....\n\n");
	 return 1;
  }
  */
# endif
