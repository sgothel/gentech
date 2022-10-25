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

# include "rivergen.h"

RiverProblem::RiverProblem( int Monk, int Cannibal,
			    int MaxChromosomen,
			    int StartChromosomNumber,
			    int StartChromosomLength,
			    int Nukleotide,
			    SpliceCodeInfo *ptrSpliceCodeInfo,
			    long InversionFreq,
			    long TranslocationFreq,
			    long AsymXOverFreq,
			    int CrossVal,
			    long MutationFreq
			  )
: Chromosomen ( RiverGame::CODE_MIN_VAL, RiverGame::CODE_MAX_VAL,
                MaxChromosomen, StartChromosomNumber,
		StartChromosomLength, Nukleotide, ptrSpliceCodeInfo,
		InversionFreq, TranslocationFreq, AsymXOverFreq,
		CrossVal, MutationFreq
	      ),
  TheGame( Monk, Cannibal, 1 )
{ }

RiverProblem::RiverProblem( int Monk, int Cannibal,
			    int MaxChromosomen,
			    char *StartGenFile,
			    int Nukleotide,
			    SpliceCodeInfo *ptrSpliceCodeInfo,
			    long InversionFreq,
			    long TranslocationFreq,
			    long AsymXOverFreq,
			    int CrossVal,
			    long MutationFreq
			  )
: Chromosomen ( RiverGame::CODE_MIN_VAL, RiverGame::CODE_MAX_VAL,
                MaxChromosomen, StartGenFile,
		Nukleotide, ptrSpliceCodeInfo,
		InversionFreq, TranslocationFreq, AsymXOverFreq,
		CrossVal, MutationFreq
	      ),
  TheGame( Monk, Cannibal, 1 )
{ }

double RiverProblem::Fitness (const Chromosom &Lsg)
{
  return ( TheGame.Play(Lsg,FALSE) );
}










