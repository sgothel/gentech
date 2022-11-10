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

#ifndef RIVER_GEN_H
  #define RIVER_GEN_H

  #include <string>

  #include <gentech/gentech.hpp>

  #include "river.hpp"

  class RiverProblem : public Chromosomen {
  public:
    // Der Konstruktor mit allen Einstellungen zum Loesen des Problems.
    RiverProblem( int Monk, int Cannibal,
          size_type MaxChromosomen,
          size_type StartChromosomNumber,
          size_type StartChromosomLength,
          size_type Nukleotide=3,
		  SpliceCodeInfo *ptrSpliceCodeInfo=nullptr,
		  size_type InversionFreq=0,
		  size_type TranslocationFreq=0,
		  size_type AsymXOverFreq=0,
		  size_type CrossVal=1,
		  size_type MutationFreq=10000 // [1]
		) ;
    RiverProblem( int Monk, int Cannibal,
          size_type MaxChromosomen,
		  const std::string& StartGenFile,
		  size_type Nukleotide=3,
		  SpliceCodeInfo *ptrSpliceCodeInfo=nullptr,
		  size_type InversionFreq=0,
		  size_type TranslocationFreq=0,
		  size_type AsymXOverFreq=0,
		  size_type CrossVal=1,
		  size_type MutationFreq=10000
		) ;
    // Fitness fuer das uebergebene Chromosom.
    // Wert [0..1] !
    virtual double Fitness(const Chromosom&);
    // Und das Spiel selbst als Element ...
    RiverGame TheGame;
  };

# endif

