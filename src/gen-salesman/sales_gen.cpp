// salesgen.cpp                                                  6.3.1994
//
// Diese Modul wurde fuer eine Hausarbeit im Fach
// Objektorietierter Programierung (OOP) an der FH Bielefeld
// unter der Betreuung von Prof. Dr. Bunse erstellt.
//
// Autoren der Hausarbeit : Sven Goethel und Christian Mueller
//
//
// Jegliches Copyright aller Dateien ist im Besitz der Autoren.
// Sven Goethel * Stapenhorststr.35a * 33615 Bielefeld * 0521/139228
// Bielefeld, den 11.3.1994.

#include <climits>

#include "sales_gen.hpp"

#include <gentech/random.hpp>

#include "menge.h"

SalesChromosom::SalesChromosom ( SalesChromosomen & env, const std::string& FileName )
: Chromosom(env, FileName) {}

SalesChromosom::SalesChromosom ( SalesChromosomen & env, size_type StartChromosomLength)
: Chromosom(env, 0)
// Zufaellig erzeugte Chromosomen
{
    /* UNIQUE NUKLEONS */
    if( StartChromosomLength > (size_type)( env.UserNukleoMaxVal - env.UserNukleoMinVal + 1 ) ) {
        std::cerr << "\nFehler !! "
                << "Chromsomenlaenge groesser als Nukleotideauswahl !!";
    }
    for (size_type i=0; i < StartChromosomLength; ++i) {
        long Rand;
        while (istElement (Rand = Random (env.UserNukleoMinVal, env.UserNukleoMaxVal)) );
        fuegeEin (Rand);
    }
    assert (size() == StartChromosomLength);
}

SalesChromosomen::SalesChromosomen(
        size_type MaxChromosomen,
        size_type StartChromosomNumber,
        size_type StartChromosomLength,
        size_type Nukleotide,
        SpliceCodeInfo *ptrSpliceCodeInfo,
        size_type InversionFreq,
        size_type TranslocationFreq,
        size_type AsymXOverFreq,
        size_type CrossVal,
        size_type MutationFreq,
        size_type NoImprovingCrossingOvers )
: Chromosomen ( SalesGame::CODE_MIN_VAL, SalesGame::CODE_MAX_VAL,
		MaxChromosomen, 0 /* use SalesChromosomes here */,
		StartChromosomLength, Nukleotide, ptrSpliceCodeInfo,
		InversionFreq, TranslocationFreq, AsymXOverFreq,
		CrossVal, MutationFreq ),
  TheGame( 1 ),
  NoImproving(1),
  NoImprovingCrossingOvers(NoImprovingCrossingOvers),
  Flag (0),
  WorstDistance (0),
  BestDistance (-1)
{ 
  // StartGene zufaellig setzen !
  for (int i=StartChromosomNumber ; i > 0  ; i--) {
    SalesChromosom Gamma (*this, StartChromosomLength);
    assert (Gamma.size() == StartChromosomLength);
    fuegeEin (Gamma);
  }
  assert (size()==StartChromosomNumber);
}

SalesChromosomen::SalesChromosomen(
        size_type MaxChromosomen,
        const std::string& StartGenFile,
        size_type Nukleotide,
        SpliceCodeInfo *ptrSpliceCodeInfo,
        size_type InversionFreq,
        size_type TranslocationFreq,
        size_type AsymXOverFreq,
        size_type CrossVal,
        size_type MutationFreq,
        size_type NoImprovingCrossingOvers )
: Chromosomen ( SalesGame::CODE_MIN_VAL, SalesGame::CODE_MAX_VAL,
		MaxChromosomen, StartGenFile,
		Nukleotide, ptrSpliceCodeInfo,
		InversionFreq, TranslocationFreq, AsymXOverFreq,
		CrossVal, MutationFreq ),
  TheGame( 1 ),
  NoImproving(1),
  NoImprovingCrossingOvers(NoImprovingCrossingOvers),
  Flag (0),
  WorstDistance (0),
  BestDistance (-1)
{ }

double SalesChromosomen::Fitness (const Chromosom &Lsg)
{
  double Distance = TheGame.Play (Lsg, false);

  if (Distance > WorstDistance)	{
    WorstDistance = Distance;
    Flag |= 1;
  }
  assert (WorstDistance != 0);

  return (WorstDistance - Distance) / WorstDistance;
}

int SalesChromosomen::Evolution(double GoalFitness, const std::string& chrptrPtkFile,
			                    double BirthRate, int Bigamie )
{
    (void) GoalFitness;
    double CutFitness = 0; 	// Der Cut beim Sterben
    bool stop = false;

    InitFitness() ;
    if( !chrptrPtkFile.empty() ) {
        if ((fileptrPtk=fopen (chrptrPtkFile.c_str(), "wt")) == nullptr) {
            INT_ERR (__LINE__);
        }
    } else {
        fileptrPtk = nullptr;
    }

    EvolutionStart=time(nullptr);
    Generation=1;
    SplicedChromosoms=0;
    IntroCodeLenSum=0;
    Protokoll();
    if( !Echo() ) { stop = true; }

    if( BirthRate <= 0.0 || BirthRate > 1.0 ) { stop=1; }

    while( NoImproving < NoImprovingCrossingOvers && !stop )
    {
        Generation++;
        SplicedChromosoms=0;
        IntroCodeLenSum=0;
        if( BirthRate < 1.0 ) {
            NewGeneration (BirthRate, Bigamie);
            // Fuer LetDie !!!
            CutFitness = GetXWorstFitness (size()-MaxChromosomen);
        } else {
            NewGeneration (Bigamie);
            CutFitness = -.5;	// Eltern Fitness auf -1 gesetzt
        }
        // Sterben !!!
        LetDie(CutFitness);

        // Die Mutation
        Mutation();

        // Fitness berechnen. Vorher Abfrage ob sich 'WorstDistance' geaendert
        // hat. Wenn ja, muss die Fitness fuer die gesammte Population berechnet
        // werden
        if( Flag & 1 ) {
            InitFitness();
            Flag &= ~1;
        } else {
            CalcWholeFitness();
        }

        // Weitere Mutationen
        InversionsMutation();
        TranslocationsMutation();

        GenerationEnd=time(nullptr);

        if( Flag & 2 ) {
            NoImproving=0;
            Flag &= ~2;
        } else {
            NoImproving++ ;
        }
        Protokoll();
        if( !Echo() ) { stop=1; }
    }

    EvolutionEnd=time(nullptr);
    Echo();
    Protokoll();

    if (fileptrPtk != nullptr) { fclose(fileptrPtk); }

    TheGame.Play(TheBestEver, true);

    return Generation;
}

void SalesChromosomen::validate(const Chromosom &c ) {
    // Ueberpruefung, ob jedes Element nur einmal vorhanden ist
    Menge<NukleoTyp> test;

    for (size_type j = 0; j < c.size(); j++) {
        test.fuegeEin (c[j]);
    }
    if( test.card() != c.size() ) {
        std::cout << "\ndest:\n" << c
                << std::endl << "test.size()    = " << test.card()
                << std::endl;
    }
    assert (test.card() == c.size());
}

/* UNIQUE NUKLEOTIDS */
void SalesChromosomen::CreateNewSymChromosom (Chromosom &dest, size_type m, size_type w,
					                          SortListe<size_type, size_type>& CrossPoints)
// Programiert nach Pseudocode aus:
// Eberhard Schoeneburg, Frank Heinzmann, Sven Feddersen:
// Genetische Algorithmen und Evolutionsstrategien,
// Addison-Wesley, ISBN 3-89319-493-2,
// Seite 274/275
{
    // i          : Indize des Crosspoints
    // von, bis   : Zu Uebertragender Chromosomenabschnitt [von..bis[
    // ch         : Alternierendes Indize zwischen den beiden Chromosomen
    //              mit den Geschlechtern 'm' und 'w'
    // done       : Abbruch Indikator
    bool done = false;
    size_type i, von, bis;

    // Startwerte !!
    i = bis = 0;
    dest = THIS[m];

# ifndef NDEBUG
    if (THIS[w].size() != THIS[m].size() ||
            THIS[w].size() != dest.size())
        std::cout << std::endl << "THIS[w].size() = " << THIS[w].size()
        << std::endl << "THIS[m].size() = " << THIS[m].size()
        << std::endl << "dest.size()    = " << dest.size();
# endif

    do {
        // An dem letzten exklusiven Ende fortfahren
        von = bis;
        if (i < CrossPoints.size()) {
            // Chromosomenabschnitt-Ende holen.
            bis = CrossPoints[i];
            i++;
        } else {
            // ...und den Rest uebertragen
            bis = THIS[m].size();
            done=true;
        }

        if (i % 2) {
            size_type n;
            Liste<NukleoTyp> intervall;
            assert (intervall.size() == 0);

            // Gene im Intervall puffern
            for (n = von; n < bis; n++)
                intervall.fuegeEin (THIS[w][n], intervall.size());

            // Gene aus 'w1' in 'dest' suchen und loeschen
            for (n = 0; n < dest.size() && intervall.size() > 0; n++)
            {
                size_type l = 0;
                while( l < intervall.size() ) {
                    if (intervall[l] == dest[n])        {
                        dest.loesche (n);
                        intervall.loesche (l);
                        l = 0;	// Neuen Suchdurchlauf initiieren
                    } else {
                        ++l;
                    }
                }
            }
            assert (intervall.size() == 0);

            // Intervall von 'dest' mit Genen von 'w' fuellen
            for (n = von; n < bis; n++) {
                dest.fuegeEin (THIS[w][n], n);
            }
        }
    } while( !done );

#ifndef NDEBUG
    validate( dest );

    if (dest.size() != THIS[w].size() ) {
        std::cout << "\ndest:\n" << dest
                << std::endl << "THIS[m].size() = " << THIS[m].size()
                << std::endl << "THIS[w].size() = " << THIS[w].size()
                << std::endl;
    }
    assert ( dest.size() == THIS[m].size() &&
             dest.size() == THIS[w].size() );
#endif
}

/* UNIQUE NUKLEOTIDS */
void SalesChromosomen::CrossingOver (size_type m, size_type w)
// Order Crossing Over
//
{
    if (CrossVal == 0) { return; }

    // Symmetrisches XOver !!!
    SortListe<size_type, size_type> CrossPoints;
    assert (CrossPoints.size()==0);

    // Kreuzungspunkte sortiert eintragen.
    for (size_type i = 0; i < CrossVal; ++i) {
        CrossPoints.fuegeEin( Random (0 , THIS[w].size()));
    }

    SalesChromosom NeuA (*this);
    SalesChromosom NeuB (*this);
    int SplicedCode;

    CreateNewSymChromosom (NeuA, m, w, CrossPoints);
    fuegeEin (NeuA, size());
    SplicedCode=NeuA.Splicing();
    if(SplicedCode>0) {
        SplicedChromosoms++;
        IntroCodeLenSum+=SplicedCode;
    }
    // Die Fitness des gespleissten Chromosomes in das ungespleisste
    // eingebundene Chromosom einsetzen !!!
    THIS[size()-1].SetFitness(Fitness(NeuA));
    CreateNewSymChromosom (NeuB, w, m, CrossPoints);
    fuegeEin (NeuB, size());
    SplicedCode=NeuB.Splicing();
    if(SplicedCode>0) {
        SplicedChromosoms++;
        IntroCodeLenSum+=SplicedCode;
    }
    THIS[size()-1].SetFitness(Fitness(NeuB));
}

void SalesChromosomen::Mutation()
{
    static size_type next_nukleotide_idx = MutationFreq + Random( 0, (size_type)(MutationFreq/MutationFreqVar) ) ;
    MutationsThisGeneration=0;

    if (MutationFreq > 0) {
        size_type nukleotide_idx = next_nukleotide_idx;
        for (size_type chromosom_idx = 0; chromosom_idx < size(); ++chromosom_idx) {
            bool mutated = false;
            const size_type chromosom_len = THIS[chromosom_idx].size();
            while( nukleotide_idx < chromosom_len-1 ) {
                // Die Move-Mutation.
                /*
                    // Ungewichtete Mutation
                    while ((Rand = Random (0, THIS[i].size()))
                          == NukleotidsPassedInChromosom-1);
                    THIS[i].fuegeEin (THIS[i][NukleotidsPassedInChromosom-1], Rand);
                    if (Rand > NukleotidsPassedInChromosom)
                      THIS[i].loesche (NukleotidsPassedInChromosom-1);
                    else
                      THIS[i].loesche (NukleotidsPassedInChromosom);
                 */
                // Gewichtete Mutation
                // MutationsStaerke proportional zur Stagnation der Besten Fitness !!!
                size_type MutationStrength = (size_type) (
                                    ( (double)NoImproving * chromosom_len ) /
	                                  (double)(NoImprovingCrossingOvers << 1) );
                // mindestens einen...
                MutationStrength = std::max<size_type>(3, MutationStrength);
                size_type pos;
                {
                    size_type l_pos, h_pos; // clipped to chromosom_len
                    if( MutationStrength <= nukleotide_idx ) {
                        l_pos = nukleotide_idx - MutationStrength;
                        if( nukleotide_idx + MutationStrength < chromosom_len ) {
                            h_pos = nukleotide_idx + MutationStrength;
                        } else {
                            h_pos = chromosom_len - 1;
                        }
                    } else {
                        l_pos = nukleotide_idx + 1;
                        if( nukleotide_idx + 2 * MutationStrength < chromosom_len ) {
                            h_pos = nukleotide_idx + 2 * MutationStrength;
                        } else {
                            h_pos = chromosom_len - 1;
                        }
                    }
                    do {
                        pos =  Random (l_pos, h_pos);
                    } while( nukleotide_idx == pos );
                }
                THIS[chromosom_idx].fuegeEin( THIS[chromosom_idx][nukleotide_idx], pos );
                if( pos <= nukleotide_idx ) {
                    THIS[chromosom_idx].loesche (nukleotide_idx+1);
                } else {
                    THIS[chromosom_idx].loesche (nukleotide_idx);
                }
                assert( THIS[chromosom_idx].size() == chromosom_len );

                MutationsThisGeneration++;
                mutated = true;
                nukleotide_idx += MutationFreq + Random( 0, (size_type)(MutationFreq/MutationFreqVar) );
            }
            if( nukleotide_idx >= chromosom_len-1 ) {
                nukleotide_idx -= chromosom_len-1;
            }
            if( mutated ) {
                // Neue Fitness berechnen
                Chromosom Neu( THIS[chromosom_idx] );
                Neu.Splicing();
                THIS[chromosom_idx].SetFitness(Fitness(Neu));
#ifndef NDEBUG
                validate( THIS[chromosom_idx] );
#endif
            } else {
            }
        }
        next_nukleotide_idx = nukleotide_idx;
    }
}

int SalesChromosomen::Echo()
{
    if (Generation == 1 ) {
        printf(" Generation: BestGeneration: AverageDistance: BestDistance:"
                " WorstDistance:\n");
    }
    printf("\r%11zu%16zu%16ld%16ld%16ld",
            (size_t)GetGeneration(), (size_t)TheBestEversGeneration,
            (long)(GetWorstDistance()-GetAverageFitness()*GetWorstDistance()),
            (long)GetBestDistance(), (long)GetWorstDistance() );

    if (EvolutionEnd > 0) {
        if (Generation > 1) {
            printf ("\n\nAvrg. Generationsdauer                : %f s / Generation\n",
                    ((double)(EvolutionEnd-EvolutionStart))/((double)(GetGeneration()-1)) );

        }
        printf ("\n\nGenerationen / Evolutionsdauer        : %3zu  /  %3zu s\n",
                (size_t)Generation, (size_t)(EvolutionEnd-EvolutionStart));
    }
    return 1;
}

void SalesChromosomen::CalcWholeFitness()
{
    double Total=0, TempFitness;
    size_type BestChrom=npos, ChromLen;
    size_type ChromLenSum=0;

    ChromosomenLenMin = std::numeric_limits<size_type>::max();
    ChromosomenLenMax = std::numeric_limits<size_type>::min();
    BestFitness = -1;
    ChromBetterZeroNumber = 0;

    for (size_type i = 0; i < size(); ++i) {
        ChromLen = THIS[i].size();
        ChromLenSum += ChromLen;
        if( ChromosomenLenMin > ChromLen ) { ChromosomenLenMin=ChromLen; }
        if( ChromosomenLenMax < ChromLen ) { ChromosomenLenMax=ChromLen; }
        if( ( TempFitness = THIS[i].GetFitness() ) > 2*std::numeric_limits<double>::epsilon() ) {
            ChromBetterZeroNumber++ ;
        }
        Total += TempFitness ;
        if( GetBestDistance() > 0.5 + WorstDistance - TempFitness * WorstDistance ||
            GetBestDistance() < 0.0 )
        {
            BestFitness = TempFitness;
            BestChrom = i;
            if( !(TheBestEver == THIS[BestChrom]) ) {
                TheBestEver = THIS[BestChrom];
                TheBestEversGeneration = Generation;
                TheBestEver.Splicing();
                Flag |= 2;	// NoImproving to ZERO
            }
            BestDistance = WorstDistance - TempFitness * WorstDistance;
        }
    }
    AverageFitness = Total / size();
    FitnessSum = Total;
    ChromosomenLenAvrg = (double)ChromLenSum/(double)size();
}

void SalesChromosomen::Protokoll()
{
    double SpliceCodePerChrom ;

    SpliceCodePerChrom = (SplicedChromosoms > 0) ?
            ((double)IntroCodeLenSum/(double)SplicedChromosoms)
            : 0.0
              ;

    if (fileptrPtk != nullptr) {
        if (EvolutionEnd == 0) {
            fprintf (fileptrPtk, "=======================================================\n\n\n");
            fprintf (fileptrPtk, "\nGeneration / Generierungsdauer        : %3zu  /  %zu s\n",
                    (size_t)Generation, (size_t)(GenerationEnd-GenerationStart) );
            fprintf (fileptrPtk, "Populationsgroesse                    : %3zu\n",
                    (size_t)size());
            fprintf (fileptrPtk, "Chromosomen Laenge Minimum            : %3zu\n",
                    (size_t)ChromosomenLenMin);
            fprintf (fileptrPtk, "Chromosomen Laenge Maximum            : %3zu\n",
                    (size_t)ChromosomenLenMax);
            fprintf (fileptrPtk, "Chromosomen Laenge Durchschnitt       : %10.6lf\n",
                    ChromosomenLenAvrg);
            fprintf (fileptrPtk, "Gespleisste Chromosomen               : %3zu\n",
                    (size_t)SplicedChromosoms);
            fprintf (fileptrPtk, "Gespleisstes Code pro Chromosom       : %10.6lf\n",
                    SpliceCodePerChrom);

            fprintf (fileptrPtk, "\nBestFitness                           : %10.6lf\n",
                    BestFitness);
            fprintf (fileptrPtk, "Average Fitness                       : %10.6lf\n",
                    AverageFitness);

            fprintf (fileptrPtk, "\nBest Distance                         : %10.0lf\n",
                    GetBestDistance());
            fprintf (fileptrPtk, "Average Distance                      : %10ld\n",
                    (long)(GetWorstDistance()-GetAverageFitness()*GetWorstDistance()));
            fprintf (fileptrPtk, "Worst Distance                        : %10.0lf\n",
                    GetWorstDistance());

            fprintf (fileptrPtk, "\nMutationen dieser Generation          : %3zu\n",
                    (size_t)MutationsThisGeneration);
            fprintf (fileptrPtk, "Inversionen dieser Generation         : %3zu\n",
                    (size_t)InversionsThisGeneration);
            fprintf (fileptrPtk, "Translokationen dieser Generation     : %3zu\n",
                    (size_t)TranslocationsThisGeneration);
            fprintf (fileptrPtk, "\nTheBestEverFitness                    : %10.6lf\n",
                    TheBestEver.GetFitness());
            fprintf (fileptrPtk, "TheBestEversAverageFitness            : %10.6lf\n",
                    TheBestEversAverageFitness);
            fprintf (fileptrPtk, "TheBestEversGeneration                : %3zu\n\n",
                    (size_t)TheBestEversGeneration);
        } else {
            if(Generation>1) {
                fprintf (fileptrPtk, "Avrg. Generationsdauer                  : %f s / Generation\n",
                        ((double) (EvolutionEnd-EvolutionStart))/((double)(GetGeneration()-1)) );
            }
            fprintf (fileptrPtk, "\nGenerationen / Evolutionsdauer        : %3zu /  %zu s\n",
                    (size_t)Generation, (size_t)(EvolutionEnd-EvolutionStart));
        }
    }
}

