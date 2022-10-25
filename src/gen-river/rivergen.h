// rivergen.h							 6.3.1994
//
// Diese Modul wurde fuer eine Hausarbeit im Fach
// Objektorietierter Programierung (OOP) an der FH Bielefeld
// unter der Betreuung von Prof. Dr. Bunse erstellt.
//
// Autoren der Hausarbeit : Sven Goethel und Christian Mueller
//
//
// Jegliches Copyright aller Dateien ist im Besitz der Autoren.
// Sven Goethel * http://www.jausoft.com - mailto:info@jausoft.com
// Bielefeld, den 11.3.1994.

# ifndef _RIVERGEN_H
  # define _RIVERGEN_H

  # include <stdio.h>
  #ifdef __MSDOS__ 
	  # include <conio.h>
  #endif
  # include "gentech.h"
  # include "river.h"

  class RiverProblem : public Chromosomen {
  public:
    // Der Konstruktor mit allen Einstellungen zum Loesen des Problems.
    RiverProblem( int Monk, int Cannibal,
		  int MaxChromosomen,
		  int StartChromosomNumber,
		  int StartChromosomLength,
		  int Nukleotide=3,
		  SpliceCodeInfo *ptrSpliceCodeInfo=NULL,
		  long InversionFreq=0,
		  long TranslocationFreq=0,
		  long AsymXOverFreq=0,
		  int CrossVal=1,
		  long MutationFreq=10000 // [1]
		) ;
    RiverProblem( int Monk, int Cannibal,
		  int MaxChromosomen,
		  char *StartGenFile,
		  int Nukleotide=3,
		  SpliceCodeInfo *ptrSpliceCodeInfo=NULL,
		  long InversionFreq=0,
		  long TranslocationFreq=0,
		  long AsymXOverFreq=0,
		  int CrossVal=1,
		  long MutationFreq=10000
		) ;
    // Fitness fuer das uebergebene Chromosom.
    // Wert [0..1] !
    virtual double Fitness (const Chromosom&);
    // Und das Spiel selbst als Element ...
    RiverGame TheGame;
  };

# endif

