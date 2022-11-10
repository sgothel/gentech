// rivergen.cpp                                                  6.3.1994
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

# include "river_gen.hpp"

RiverProblem::RiverProblem( int Monk, int Cannibal,
                size_type MaxChromosomen,
                size_type StartChromosomNumber,
                size_type StartChromosomLength,
                size_type Nukleotide,
			    SpliceCodeInfo *ptrSpliceCodeInfo,
			    size_type InversionFreq,
			    size_type TranslocationFreq,
			    size_type AsymXOverFreq,
			    size_type CrossVal,
			    size_type MutationFreq
			  )
: Chromosomen ( RiverGame::CODE_MIN_VAL, RiverGame::CODE_MAX_VAL,
                MaxChromosomen, StartChromosomNumber,
		StartChromosomLength, Nukleotide, ptrSpliceCodeInfo,
		InversionFreq, TranslocationFreq, AsymXOverFreq,
		CrossVal, MutationFreq ),
  TheGame( Monk, Cannibal, 1 )
{ }

RiverProblem::RiverProblem( int Monk, int Cannibal,
                size_type MaxChromosomen,
			    const std::string& StartGenFile,
			    size_type Nukleotide,
			    SpliceCodeInfo *ptrSpliceCodeInfo,
			    size_type InversionFreq,
			    size_type TranslocationFreq,
			    size_type AsymXOverFreq,
			    size_type CrossVal,
			    size_type MutationFreq
			  )
: Chromosomen ( RiverGame::CODE_MIN_VAL, RiverGame::CODE_MAX_VAL,
                MaxChromosomen, StartGenFile,
		Nukleotide, ptrSpliceCodeInfo,
		InversionFreq, TranslocationFreq, AsymXOverFreq,
		CrossVal, MutationFreq ),
  TheGame( Monk, Cannibal, 1 )
{ }

double RiverProblem::Fitness (const Chromosom &Lsg)
{
  return ( TheGame.Play(Lsg, false) );
}










