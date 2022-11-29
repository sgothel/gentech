// random.h                                                    4-11-94, 12:21 AM
//
// Autoren der Hausarbeit : Sven Goethel und Christian Mueller
//
// Jegliches Copyright aller Dateien ist im Besitz der Autoren.
// Sven Goethel * Stapenhorststr.35a * 33615 Bielefeld * 0521/139228
//
// In dieser Datei wird ein Zufallszahlengenerator als C++-Klasse
// implementiert.
// Es gibt verschiedene Implementierungen:
//      1. Die Template-Funktion 'Random':
//         Sie erwartet als Aufrufparameter zwei Zahlen gleichen Typ's.
//         Je nachdem ob der Typ ein Festkommatyp ist, oder ein Gleit-
//         kommantyp wird verschieden die Zufallszahlberechnet.
//         Der Rueckgabetyp ist der selbe, wie der Typ der Aufrufparameter.
//
// Bielefeld, den 11.04.1994.
//
// Random Bufferung herausgenommen .....
//
// LESEN SIE AUCH 'GENTECH.DOC' im ASCII-Format !!!

# ifndef _RANDOM_H
  # define _RANDOM_H

  # include <cstdlib>          // fuer random(), randomize()


  #ifdef __linux__

  // Die Inline-Funktionen
  template<class T> T Random (const T l, const T h)
  {
     return l + (T)  ( ( h-l+1 ) * (double)rand()/(double)RAND_MAX ) ;
  }

  #else

  // Die Inline-Funktionen
  template<class T> T Random (const T l, const T h) ;

  // Spezielle Instanz von template<class T> Random

  double Random (const double l, const double h) ;

  #endif

# endif

