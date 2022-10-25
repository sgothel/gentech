// author : Sven Gothel - Stapenhorststrasse 35a - 33615 Bielefeld
//
// hashstr.h
// DATUM                        : 11.09.1994
//
// Jegliches Copyright aller Dateien ist im Besitz der Autoren.
// Sven Gothel * Stapenhorststr.35a * 33615 Bielefeld * 0521/139228
// Bielefeld, den 22.8.1994.
/*
 *
 *  $Log: hashstr.cc $
 *  Revision 1.5  1995/04/07 16:54:47  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.4  1995/04/04 10:37:22  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.3  1994/12/31 01:01:49  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.2  1994/12/15 15:27:53  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.1  1994/12/15 15:18:36  Sven_Gothel
 *  Initial revision
 *
*/

# include "hashstr.h"

void HashStrClassListe::Copy(const HashStrClassListe &m)
{
   anzahl=m.anzahl;
   Size=m.Size;

   int i;
   assert(HashArray==nullptr);
   assert(HEAP_CHECK()>=1);

   if(Size>0)
   {
	 if( ( HashArray=new SortListe<StrClass>*[Size] ) == nullptr )
	 {
		fprintf(stderr, "<HashStrClassListe> ERROR : Es wurde versucht ein HashArray\n");
		fprintf(stderr, "<HashStrClassListe> mit der Groesse %d zu eroeffnen.\n", Size);
		INT_ERR(__LINE__);
	 }
	 if( ( AnzahlElemente=new int[Size] ) == nullptr )
	 {
		fprintf(stderr, "<HashStrClassListe> ERROR : Es wurde versucht ein int-Array\n");
		fprintf(stderr, "<HashStrClassListe> fuer internen Gebrauch mit der Groesse %d zu eroeffnen.\n", Size);
		INT_ERR(__LINE__);
	 }

	 // Initialisierung .. Copy
	 for( i=0; i<Size; i++ )
	 {
	   if(m.HashArray[i]!=nullptr)
	   {
		 HashArray[i]=new SortListe<StrClass>(*m.HashArray[i]);
		 if(HashArray[i]==nullptr) INT_ERR(__LINE__);
	   } else HashArray[i]=nullptr;
	   AnzahlElemente[i]=m.AnzahlElemente[i];
	 }
	 assert(HEAP_CHECK()>=1);
   }
}

void HashStrClassListe::Dest()
{
   assert ( ( HashArray && AnzahlElemente ) ||
			( !HashArray && !AnzahlElemente )
		  );
   if( HashArray && AnzahlElemente )
   {
	 for(int i=0; i<Size; i++ )
	 {
	   assert(HEAP_CHECK()>=1);
	   if(HashArray[i]!=nullptr)
		 delete HashArray[i];
	 }
	 assert(HEAP_CHECK()>=1);
	 delete[] HashArray;
	 HashArray=nullptr;
	 delete[] AnzahlElemente;
	 AnzahlElemente=nullptr;
	 assert(HEAP_CHECK()>=1);
   }
}

HashStrClassListe::HashStrClassListe( int size )
: HashArray(nullptr), AnzahlElemente(nullptr), Size(size), anzahl(0)
{
   assert(HashArray==nullptr);
   assert(HEAP_CHECK()>=1);
   if(size>0)
   {
	 if( ( HashArray= new SortListe<StrClass>*[Size] ) == nullptr )
	 {
		fprintf(stderr, "<HashStrClassListe> ERROR : Es wurde versucht ein HashArray\n");
		fprintf(stderr, "<HashStrClassListe> mit der Groesse %d zu eroeffnen.\n", Size);
		INT_ERR(__LINE__);
	 }
	 assert( HashArray!=nullptr );
	 assert(HEAP_CHECK()>=1);
	 if( ( AnzahlElemente=new int[Size] ) == nullptr )
	 {
		fprintf(stderr, "<HashStrClassListe> ERROR : Es wurde versucht ein int-Array\n");
		fprintf(stderr, "<HashStrClassListe> fuer internen Gebrauch mit der Groesse %d zu eroeffnen.\n", Size);
		INT_ERR(__LINE__);
	 }
	 assert(HEAP_CHECK()>=1);
	 // initialisierung
	 for ( int i=0; i<Size; i++ )
	 {
	   HashArray[i]=nullptr;
	   AnzahlElemente[i]=0;
	 }
	 assert(HEAP_CHECK()>=1);
   }
}

HashStrClassListe::HashStrClassListe( const HashStrClassListe &m )
: HashArray(nullptr), AnzahlElemente(nullptr), Size(0), anzahl(0)
{ Copy(m); }

HashStrClassListe& HashStrClassListe::operator=(const HashStrClassListe &m)
{
  if (this == &m)
	 return *this;
  Dest();
  Copy(m);
  return *this;
}

StrClass& HashStrClassListe::operator[](int i) const
{
   if(Size<=0) INT_ERR(__LINE__);

   int HPos, SPos;
   if ( i>=anzahl || HoleHashNSortPosition ( i, HPos, SPos ) <= 0 )
   {
	  fprintf(stderr, "<HashStrClassListe> ERROR : Es wurde versucht auf das Element mit dem Index %d\n", i);
	  fprintf(stderr, "<HashStrClassListe> zuzugreifen. Die HashStrClassListe enthaelt aber nur %d Elemente.\n", anzahl);
	  INT_ERR(__LINE__);
   }
   assert(HEAP_CHECK()>=1);
   assert(HashArray[HPos]!=nullptr);
   return HashArray[HPos]->operator[](SPos);
}

int HashStrClassListe::fuegeEin(const StrClass& a)
{
   if(Size<=0) INT_ERR(__LINE__);

   int HPos=abs(HashFunktion(a)%Size);
   assert (HPos>=0);
   assert (HPos<Size);
   assert(HEAP_CHECK()>=1);
   if(HashArray[HPos]==nullptr)
   {
	 // SortListe<StrClass> Erzeugen
	 HashArray[HPos]=new SortListe<StrClass>();
	 if( HashArray[HPos] == nullptr ) INT_ERR(__LINE__);
   }
   int SPos=HashArray[HPos]->fuegeEin(a);
   assert(HEAP_CHECK()>=1);
   AktualisiereAnzahlElementePlus(HPos);
   anzahl++;
   return HoleLinearerIndex(HPos, SPos);
}

int HashStrClassListe::loesche(int i)
{
   if(Size<=0) INT_ERR(__LINE__);

   int HPos, SPos;
   if ( i>=anzahl || HoleHashNSortPosition ( i, HPos, SPos ) <= 0 )
   {
	  fprintf(stderr, "<HashStrClassListe> ERROR : Es wurde versucht das Element mit dem Index %d zu loesche.\n", i);
	  fprintf(stderr, "<HashStrClassListe> Die HashStrClassListe enthaelt aber nur %d Elemente.\n", anzahl);
	  INT_ERR(__LINE__);
   }
   assert(HEAP_CHECK()>=1);
   assert(HashArray[HPos]!=nullptr);
   if(SPos>=HashArray[HPos]->laenge()) INT_ERR(__LINE__);
   HashArray[HPos]->loesche(SPos);
   assert(HEAP_CHECK()>=1);
   AktualisiereAnzahlElementeMinus(HPos);
   anzahl--;
   return 1;
}

int HashStrClassListe::findeElement(const StrClass& x) const
{
   if(Size<=0) INT_ERR(__LINE__);

   int HPos=abs(HashFunktion(x)%Size);
   assert (HPos>=0);
   assert (HPos>=0);
   if(HashArray[HPos]!=nullptr)
   {
	 int SPos=HashArray[HPos]->findeElement(x);
	 if(SPos>=0) return HoleLinearerIndex(HPos, SPos);
   }
   return -1;
}

int HashStrClassListe::HoleLinearerIndex(int HPos, int SPos) const
{
   int LinearePos=SPos;
   if(--HPos>=0) LinearePos+=AnzahlElemente[HPos];
   return LinearePos;
}

void HashStrClassListe::AktualisiereAnzahlElementePlus(int HPos)
{
  while(HPos<Size) AnzahlElemente[HPos++]++;
}

void HashStrClassListe::AktualisiereAnzahlElementeMinus(int HPos)
{
  while(HPos<Size) AnzahlElemente[HPos++]--;
}


int HashStrClassListe::HoleHashNSortPosition ( int LinearerIndex,
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

/*
  // Hier ist ein Testprogramm fuer die ganz virtuelle Klasse HashStrClassListe !
  // TESTEN !!!!!!

  class HashStr : public HashStrClassListe {
  public:
	HashStr(int S=1) : HashStrClassListe(S) {}
	HashStr(const HashStr &m) : HashStrClassListe(m) {}
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
