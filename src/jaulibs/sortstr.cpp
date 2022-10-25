// author : Sven Gothel - Stapenhorststrasse 35a - 33615 Bielefeld
//
// sortstr.cc   
//
// Jegliches Copyright aller Dateien ist im Besitz der Autoren.
// Sven Gothel * Stapenhorststr.35a * 33615 Bielefeld * 0521/139228
// Bielefeld, den 11.3.1994.
//
// Diese Datei enthaelt eine besondere Version fuer eine StrClass-Instanz
// der Template-Klasse SortListe !!!!!
//
// Der StrCmp Operator muss nicht des oeffteren aufgerufen werden.
//
// Seien sie sicher das diese Datei vor allen anderen Kompiliert wird,
// so dass der Linker diese Instanz verwendet.
/*
 *
 *  $Log: sortstr.cc $
 *  Revision 1.4  1995/04/07 16:54:56  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.3  1995/04/04 10:37:36  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.2  1994/12/15 15:28:25  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.1  1994/12/15 15:18:44  Sven_Gothel
 *  Initial revision
 *
*/

# include "sortlist.h"
# include "strclass.h"

#include <cstring>

/*------------------------------------------------------------------------------

	Variante fuer StrClass der Methode findeIndex der template
	klasse SortListe
	----------------------------------------------------------
	int ::findeIndex(const StrClass &x, int &u, int &o) const

	- Beschreibung
	--------------
	Variante fuer StrClass ...
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
	dieselben des Parsers.


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
			  else *u == x 	=> i=u, done
			  else *o == x 	=> i=o, done

------------------------------------------------------------------------------*/
template<> int SortListe<StrClass>::findeIndex(const StrClass &x, int &u, int &o) const
{
  int i=0;
  int done=0;
  int result, result_o, result_u;

  // da gibt es nix ....
  if ( (*this).laenge() == 0 ) return -1;

  //Anfangsgrenzen austesten
  result_o=strcmp( (*this)[o].getStrPtr(), x.getStrPtr() );
  result_u=strcmp( (*this)[u].getStrPtr(), x.getStrPtr() );
  if(Type==UP)
  {
	if( u == o && result_o < 0 ) o++;
	else if( result_u > 0 	)  { o=u;			}
	else if( result_o < 0 	)  { u=o; o++;  	}
	else if( result_u == 0 	)  { i=u, done=1; 	}
	else if( result_o == 0 	)  { i=o; done=1; 	}
  } else {
	if( u == o && result_o > 0 ) o++;
	else if( result_u < 0 	)  { o=u;			}
	else if( result_o > 0 	)  { u=o; o++;  	}
	else if( result_u == 0 	)  { i=u, done=1; 	}
	else if( result_o == 0 	)  { i=o; done=1; 	}
  }

  // bisektionieren bis kein Element zwischen den grenzen
  while ( !done && o-u>1)
  {
	i=(u+o)/2;
	result=strcmp( (*this)[i].getStrPtr(), x.getStrPtr() );
	if ( result < 0 )
	{
	  if ( Type==UP ) u=i;
	  else o=i;
	}
	else if ( result > 0 )
	{
	  if ( Type==UP ) o=i;
	  else u=i;
	}
	else done=1;
  }

  return done==0 ? -1 : i ;
}




