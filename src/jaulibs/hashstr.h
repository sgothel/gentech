// hashstr.h
//
// Jegliches Copyright aller Dateien ist im Besitz der Autoren.
// Sven Gothel * Stapenhorststr.35a * 33615 Bielefeld * 0521/139228
// Bielefeld, den 22.8.1994.
//
/*
 *
 *  $Log: hashstr.h $
 *  Revision 1.4  1995/04/07 16:54:47  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.3  1995/04/04 10:37:22  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.2  1994/12/15 15:27:55  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.1  1994/12/15 15:18:37  Sven_Gothel
 *  Initial revision
 *
*/

# ifndef _HASHSTRLISTE_H
  # define _HASHSTRLISTE_H

  # include "sortlist.h"
  # include "interror.h"
  # ifdef __BORLANDC__
	# ifndef NO_HEAP_CHECK
	  # include <alloc.h>
	  # define HEAP_CHECK heapcheck
	# else
	  # define HEAP_CHECK() 2
	# endif
  # else
	# define HEAP_CHECK() 2
  # endif
  # include <cmath>
  # include "strclass.h"

  // Die HashStrClassListe enthaelt ein Array von SortListe<int> !!!
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
  // Die HashStrClassListe Simuliert eine normale Liste.
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
  // HashStrClassListe& operator=(const HashStrClassListe &m)
  // Der Zuwisungsoperator
  //
  // laenge() liefert die Anzahl der Element-Eintraege.
  //

  class HashStrClassListe {
  public:
	HashStrClassListe( int size = 0 ) ;
	HashStrClassListe( const HashStrClassListe &m );
	virtual ~HashStrClassListe( void ) { Dest(); };
	StrClass& operator[](int i) const ;
	HashStrClassListe& operator=(const HashStrClassListe &m);
	int laenge() const { return anzahl; }
	int fuegeEin( const StrClass& );
	int loesche( int i );
	int findeElement( const StrClass& ) const ;
	virtual int HashFunktion(const StrClass &a) const = NULL;

  private:
	void AktualisiereAnzahlElementePlus(int HPos);
	void AktualisiereAnzahlElementeMinus(int HPos);
	int HoleLinearerIndex(int HPos, int SPos) const ;
	int HoleHashNSortPosition ( int LinearerIndex,
								int &HPos,
								int &SortPos
							  ) const ;
	void Copy(const HashStrClassListe &);
	void Dest();
	SortListe<StrClass> **HashArray; // Ein Array von Pointern .....
	int * AnzahlElemente;
	int Size;
	int anzahl;
  };

#endif

