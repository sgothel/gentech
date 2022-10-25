// author : Sven Gothel - Stapenhorststrasse 35a - 33615 Bielefeld
//
// sortlist.h    
//
// Jegliches Copyright aller Dateien ist im Besitz der Autoren.
// Sven Gothel * Stapenhorststr.35a * 33615 Bielefeld * 0521/139228
// Bielefeld, den 11.3.1994.
//
// Diese Datei enthaelt eine Erweiterung der Template-Klasse Liste.
// In der hier deklarierten Template-Klasse 'SortListe' werden die
// einzelnen Listen Elemente sofort beim Einfuegen der Groesse nach
// sortiert ( In aufsteigender(UP)/absteigender(DOWN) Reihenfolge ).
// D.h. die Methode 'fuegeEin' fuegt die Listenelemente mittels
// Bisektionierung gleich sortiert ein.
// fuegeEin liefert nun den Index zurueck, den das eingefuegte Element
// besitzt, ansonsten -1.
// die Listenelemente gleich sortiert eingefuegt werden, mittel Bisektionierung.
//
// Neu ist die Methode findeElement, welche ebenfalls nach der MEthode der 
// Bisektionierung arbeitet.
/*
 *
 *  $Log: sortlist.h $
 *  Revision 1.4  1995/04/07 16:54:56  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.3  1995/04/04 10:37:35  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.2  1994/12/15 15:28:23  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.1  1994/12/15 15:18:43  Sven_Gothel
 *  Initial revision
 *
*/

# ifndef _SORTLISTE_H
  # define _SORTLISTE_H

  # include "liste.h"

  template <class Tmp> class SortListe : protected Liste<Tmp> {
  public:
	enum typ { UP, DOWN };
	SortListe(typ Type=UP) : Liste<Tmp>(), Type(Type) { }
	SortListe(const SortListe& m) : Type(m.Type), Liste<Tmp>(m) {}
        virtual ~SortListe() { }

	SortListe& operator=(const SortListe &m)
	{
	  Type=m.Type;
	  Liste<Tmp>::operator=(m);
	  return *this;
	}
	// fuegeEin RETURN : neuer Index, oder -1 fuer Fehler
	int     fuegeEin(const Tmp&);
	int     findeElement(const Tmp& x) const;
	// Spezielle Zugriffsrechte fuer SortListe
	Liste<Tmp>::loesche;
	Liste<Tmp>::operator !;
	Liste<Tmp>::operator[];
	Liste<Tmp>::laenge;
	Liste<Tmp>::Referenz2Index;
	Liste<Tmp>::istElement;

  private:
	// RETURN >= 0 gefunden index
	// RETURN -1   nicht gefunden
	int findeIndex(const Tmp &x, int &u, int &o) const;
	typ Type;
  };

  template<class Tmp> int SortListe<Tmp>::fuegeEin(const Tmp& a)
  {
	int u=0, o=laenge()>0 ? laenge()-1 : 0;
	int i=findeIndex(a, u, o);
	if ( i < 0 )
	{
	  if( Liste<Tmp>::fuegeEin(a, o) == 0 ) return -1;
	  else return o;
	}
	else
	{
	  if( Liste<Tmp>::fuegeEin(a, i) == 0 ) return -1;
	  else return i;
	}
  }

  template<class Tmp> int SortListe<Tmp>::findeElement(const Tmp& x) const
  {
	int u=0, o=laenge()>0 ? laenge()-1 : 0;
	return findeIndex(x, u, o);
  }

  /*------------------------------------------------------------------------------

	  ----------------------------------------------------------
	  template<class Tmp> int ::findeIndex(const Tmp &x, int &u, int &o) const

	  - Beschreibung
	  --------------
	  Rueckgabe des Index oder -1.
	  Rueckgabe der unteren, u, und oberen, o, Grenze
	  der letzten Bisektionierung.
	  Die Grenze o wird bei nichtauffinden des index derart gesetzt, dass
	  das Element x dort eingesetzt werden  koennte !

	  - Parameter
	  -----------
	  E: 		x	: Die zu suchende StrClass
	  E/A:    u	: Start der unteren Grenze, RETURN siehe oben
	  E/A:    o	: Start der oberen Grenze, RETURN siehe oben


	  - allgemeine Ein- und Ausgabeeinheiten
	  --------------------------------------
	  keine

	  - Returncode
	  ------------
	  Der Index des gefundenen Elementes oder -1.

	  - Historie
	  ----------
	  12.8.1994 Sven Gothel

	  Die Anfangsgrenzen werden wie folgt ausgetestet :

	  UP :
				u == o &&
				  *o < x 		=> 	o++
				else *u > x 	=> o=u
				else *o < x 	=> u=o, o++
				else *u == x 	=> i=u, done
				else *o == x 	=> i=o, done

	  DOWN :
				u == o &&
				  *o > x 		=> 	o++
				else *u < x 	=> o=u
				else *o > x 	=> u=o, o++

				else * u == x 	-> 
				i = u, done

				else * o == x 	=> i=o, done

  ------------------------------------------------------------------------------*/
  template<class Tmp> int SortListe<Tmp>::findeIndex(const Tmp &x, int &u, int &o) const
  {
	int i=0;
	int done=0;

	// da gibt es nix ....
	if ( (*this).laenge() == 0 ) return -1;

	//Anfangsgrenzen austesten
	if(Type==UP)
	{
	  if( u == o && (*this)[o] < x ) o++;
	  else if( (*this)[u] > x 	)  { o=u;			}
	  else if( (*this)[o] < x 	)  { u=o; o++;  	}
	  else if( (*this)[u] == x 	)  { i=u, done=1; 	}
	  else if( (*this)[o] == x 	)  { i=o; done=1; 	}
	} else {
	  if( u == o && (*this)[o] > x ) o++;
	  else if( (*this)[u] < x 	)  { o=u;			}
	  else if( (*this)[o] > x 	)  { u=o; o++;  	}
	  else if( (*this)[u] == x 	)  { i=u, done=1; 	}
	  else if( (*this)[o] == x 	)  { i=o; done=1; 	}
	}

	// bisektionieren bis kein Element zwischen den grenzen
	while ( !done && o-u>1)
	{
	  i=(u+o)/2;
	  if ( (*this)[i] < x )
	  {
		if ( Type==UP ) u=i;
		else o=i;
	  }
	  else if ( (*this)[i] > x )
	  {
		if ( Type==UP ) o=i;
		else u=i;
	  }
	  else done=1;
	}

	return done==0 ? -1 : i ;
  }

# endif


