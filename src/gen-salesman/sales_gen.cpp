/*
 * Author: Sven Gothel <sgothel@jausoft.com>
 * Copyright (c) 1994-2022 Gothel Software e.K.
 * Copyright (c) 1994 Christian Mueller
 *
 * Proprietary licenses are available
 * via Gothel Software e.K. <sgothel@jausoft.com>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA  02111-1307, USA.
 *
 * You can also check the GNU General Public License
 * in the internet at http://www.gnu.org/copyleft/gpl.html !
 */

#include <climits>
#include <iostream>
#include <cassert>
#include <set>

#include "sales_gen.hpp"

#include <gentech/random.hpp>

#include <jau/basic_algos.hpp>

using namespace jau::gentech;

#define THIS (*this)

SalesChromosom::SalesChromosom ( SalesChromosomen & env, const std::string& FileName )
: Chromosom(env, FileName) {}

SalesChromosom::SalesChromosom ( SalesChromosomen & env, size_type StartChromosomLength)
: Chromosom(env, 0)
// Zufaellig erzeugte Chromosomen
{
    /* UNIQUE NUKLEONS */
    if( StartChromosomLength > (size_type)( env.m_max_nucleotide_value - env.m_min_nucleotide_value + 1 ) ) {
        std::cerr << "\nFehler !! "
                << "Chromsomenlaenge groesser als Nukleotideauswahl !!";
    }
    for (size_type i=0; i < StartChromosomLength; ++i) {
        nucleotide_t Rand;
        while( jau::contains(*this, Rand = Random (env.m_min_nucleotide_value, env.m_max_nucleotide_value) ) ) ;
        push_back(Rand);
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
        size_type max_no_improving_gens )
: Chromosomen ( SalesGame::CODE_MIN_VAL, SalesGame::CODE_MAX_VAL,
		MaxChromosomen, 0 /* use SalesChromosomes here */,
		StartChromosomLength, Nukleotide, ptrSpliceCodeInfo,
		InversionFreq, TranslocationFreq, AsymXOverFreq,
		CrossVal, MutationFreq, max_no_improving_gens),
  TheGame( 1 ),
  Flag (0),
  WorstDistance (0),
  BestDistance (-1)
{
    // StartGene zufaellig setzen !
    for (size_type i = 0; i < StartChromosomNumber; ++i) {
        SalesChromosom Gamma(*this, StartChromosomLength);
        assert(Gamma.size() == StartChromosomLength);
        push_back(Gamma);
    }
    assert(size() == StartChromosomNumber);
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
        size_type max_no_improving_gens )
: Chromosomen ( SalesGame::CODE_MIN_VAL, SalesGame::CODE_MAX_VAL,
		MaxChromosomen, StartGenFile,
		Nukleotide, ptrSpliceCodeInfo,
		InversionFreq, TranslocationFreq, AsymXOverFreq,
		CrossVal, MutationFreq, max_no_improving_gens),
  TheGame( 1 ),
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

SalesChromosomen::size_type SalesChromosomen::Evolution(double GoalFitness, const std::string& chrptrPtkFile,
			                                            double BirthRate, int Bigamie)
{
    (void) GoalFitness;
    double CutFitness = 0; 	// Der Cut beim Sterben
    bool stop = false;

    InitFitness() ;
    if( !chrptrPtkFile.empty() ) {
        if ((m_file_ptk_ptr=fopen (chrptrPtkFile.c_str(), "wt")) == nullptr) {
            ABORT("Couldn't open file '%s'", chrptrPtkFile.c_str());
        }
    } else {
        m_file_ptk_ptr = nullptr;
    }

    m_evolution_start=time(nullptr);
    m_generation=1;
    m_spliced_chromosom_count=0;
    m_intro_code_len_sum=0;
    m_no_improving_gens=0;
    Protokoll();
    if( !Echo() ) { stop = true; }

    if( BirthRate <= 0.0 || BirthRate > 1.0 ) { stop=true; }

    while( m_no_improving_gens < m_max_no_improving_gens && !stop )
    {
        m_generation++;
        m_spliced_chromosom_count=0;
        m_intro_code_len_sum=0;
        if( BirthRate < 1.0 ) {
            NewGeneration (BirthRate, Bigamie);
            // Fuer LetDie !!!
            CutFitness = GetXWorstFitness (size()-m_max_chromosom_count);
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

        m_generation_end=time(nullptr);

        if( Flag & 2 ) {
            m_no_improving_gens=0;
            Flag &= ~2;
        } else {
            m_no_improving_gens++ ;
        }
        Protokoll();
        if( !Echo() ) { stop=true; }
    }

    m_evolution_end=time(nullptr);
    Echo();
    Protokoll();

    if (m_file_ptk_ptr != nullptr) { fclose(m_file_ptk_ptr); }

    TheGame.Play(m_all_time_best, true);

    return m_generation;
}

void SalesChromosomen::validate(const Chromosom &c ) {
    // Ueberpruefung, ob jedes Element nur einmal vorhanden ist
    std::set<nucleotide_t> test;

    for (size_type j = 0; j < c.size(); j++) {
        test.insert(test.cend(), c[j]);
    }
    if( test.size() != c.size() ) {
        std::cout << "\ndest:\n" << c
                << std::endl << "test.size()    = " << test.size()
                << std::endl;
    }
    assert (test.size() == c.size());
}

/* UNIQUE NUKLEOTIDS */
void SalesChromosomen::CreateNewSymChromosom (Chromosom &dest, size_type m, size_type w,
                                              crosspoints_t& CrossPoints)
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
            nucleotide_array_t intervall;

            // Gene im Intervall puffern
            for (n = von; n < bis; n++)
                intervall.push_back( THIS[w][n] );

            // Gene aus 'w1' in 'dest' suchen und loeschen
            for (n = 0; n < dest.size() && intervall.size() > 0; n++)
            {
                size_type l = 0;
                while( l < intervall.size() ) {
                    if (intervall[l] == dest[n])        {
                        dest.erase(n);
                        intervall.erase(l);
                        l = 0;	// Neuen Suchdurchlauf initiieren
                    } else {
                        ++l;
                    }
                }
            }
            assert (intervall.size() == 0);

            // Intervall von 'dest' mit Genen von 'w' fuellen
            for (n = von; n < bis; n++) {
                dest.insert(n, THIS[w][n]);
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
    if (m_cross_val == 0) { return; }

    // Symmetrisches XOver !!!
    crosspoints_t CrossPoints;
    assert (CrossPoints.size()==0);

    // Kreuzungspunkte sortiert eintragen.
    for (size_type i = 0; i < m_cross_val; ++i) {
        CrossPoints.insert( Random<size_type>(0 , THIS[w].size()) );
    }

    SalesChromosom NeuA (*this);
    SalesChromosom NeuB (*this);
    size_type SplicedCode;

    CreateNewSymChromosom (NeuA, m, w, CrossPoints);
    push_back( NeuA );
    SplicedCode=NeuA.Splicing();
    if(SplicedCode>0) {
        m_spliced_chromosom_count++;
        m_intro_code_len_sum += SplicedCode;
    }
    // Die Fitness des gespleissten Chromosomes in das ungespleisste
    // eingebundene Chromosom einsetzen !!!
    THIS[size()-1].SetFitness(Fitness(NeuA));
    CreateNewSymChromosom (NeuB, w, m, CrossPoints);
    push_back( NeuB );
    SplicedCode=NeuB.Splicing();
    if(SplicedCode>0) {
        m_spliced_chromosom_count++;
        m_intro_code_len_sum += SplicedCode;
    }
    THIS[size()-1].SetFitness(Fitness(NeuB));
}

void SalesChromosomen::Mutation()
{
    static size_type next_nukleotide_idx = m_mutation_freq + Random<size_type>( 0, (size_type)(m_mutation_freq/m_mutation_freq_variance) ) ;
    m_mutations_this_gen=0;

    if (m_mutation_freq > 0) {
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
                                    ( (double)m_no_improving_gens * (double)chromosom_len ) /
	                                  (double)(m_max_no_improving_gens << 1) );
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
                const nucleotide_t n = THIS[chromosom_idx][pos];
                THIS[chromosom_idx][pos] = THIS[chromosom_idx][nukleotide_idx];
                THIS[chromosom_idx][nukleotide_idx] = n;

                m_mutations_this_gen++;
                mutated = true;
                nukleotide_idx += m_mutation_freq + Random<size_type>( 0, (size_type)(m_mutation_freq/m_mutation_freq_variance) );
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

bool SalesChromosomen::Echo() const
{
    if (m_generation == 1 ) {
        printf(" Generation: BestGeneration: AverageDistance: BestDistance:"
                " WorstDistance:\n");
    }
    printf("\r%11zu%16zu%16ld%16ld%16ld",
            (size_t)GetGeneration(), (size_t)m_all_time_best_gen,
            (long)(GetWorstDistance()-GetAverageFitness()*GetWorstDistance()),
            (long)GetBestDistance(), (long)GetWorstDistance() );

    if (m_evolution_end > 0) {
        if (m_generation > 1) {
            printf ("\n\nAvrg. Generationsdauer                : %f s / Generation\n",
                    ((double)(m_evolution_end-m_evolution_start))/((double)(GetGeneration()-1)) );
        }
        printf ("\n\nGenerationen / Evolutionsdauer        : %3zu  /  %3zu s\n",
                (size_t)m_generation, (size_t)(m_evolution_end-m_evolution_start));
    }
    return true;
}

void SalesChromosomen::CalcWholeFitness()
{
    double Total=0, TempFitness;
    size_type BestChrom=npos, ChromLen;
    size_type ChromLenSum=0;

    m_min_chromosom_len = std::numeric_limits<size_type>::max();
    m_max_chromosom_len = std::numeric_limits<size_type>::min();
    m_best_fitness = -1;
    m_chromosom_above_zero_count = 0;

    for (size_type i = 0; i < size(); ++i) {
        ChromLen = THIS[i].size();
        ChromLenSum += ChromLen;
        if( m_min_chromosom_len > ChromLen ) { m_min_chromosom_len=ChromLen; }
        if( m_max_chromosom_len < ChromLen ) { m_max_chromosom_len=ChromLen; }
        if( ( TempFitness = THIS[i].GetFitness() ) > 2*std::numeric_limits<double>::epsilon() ) {
            m_chromosom_above_zero_count++ ;
        }
        Total += TempFitness ;
        if( GetBestDistance() > 0.5 + WorstDistance - TempFitness * WorstDistance ||
            GetBestDistance() < 0.0 )
        {
            m_best_fitness = TempFitness;
            BestChrom = i;
            if( !(m_all_time_best == THIS[BestChrom]) ) {
                m_all_time_best = THIS[BestChrom];
                m_all_time_best_gen = m_generation;
                m_all_time_best.Splicing();
                Flag |= 2;	// NoImproving to ZERO
            }
            BestDistance = WorstDistance - TempFitness * WorstDistance;
        }
    }
    m_avrg_fitness = Total / (double)size();
    m_fitness_sum = Total;
    m_avrg_chromosom_len = (double)ChromLenSum/(double)size();
}

void SalesChromosomen::Protokoll()
{
    double SpliceCodePerChrom ;

    SpliceCodePerChrom = ( m_spliced_chromosom_count > 0 ) ?
                         ( (double)m_intro_code_len_sum / (double)m_spliced_chromosom_count )
                         : 0.0;

    if (m_file_ptk_ptr != nullptr) {
        if (m_evolution_end == 0) {
            fprintf (m_file_ptk_ptr, "=======================================================\n\n\n");
            fprintf (m_file_ptk_ptr, "\nGeneration / Generierungsdauer        : %3zu  /  %zu s\n",
                    (size_t)m_generation, (size_t)(m_generation_end-m_generation_start) );
            fprintf (m_file_ptk_ptr, "Populationsgroesse                    : %3zu\n",
                    (size_t)size());
            fprintf (m_file_ptk_ptr, "Chromosomen Laenge Minimum            : %3zu\n",
                    (size_t)m_min_chromosom_len);
            fprintf (m_file_ptk_ptr, "Chromosomen Laenge Maximum            : %3zu\n",
                    (size_t)m_max_chromosom_len);
            fprintf (m_file_ptk_ptr, "Chromosomen Laenge Durchschnitt       : %10.6lf\n",
                    m_avrg_chromosom_len);
            fprintf (m_file_ptk_ptr, "Gespleisste Chromosomen               : %3zu\n",
                    (size_t)m_spliced_chromosom_count);
            fprintf (m_file_ptk_ptr, "Gespleisstes Code pro Chromosom       : %10.6lf\n",
                    SpliceCodePerChrom);

            fprintf (m_file_ptk_ptr, "\nBestFitness                           : %10.6lf\n",
                    m_best_fitness);
            fprintf (m_file_ptk_ptr, "Average Fitness                       : %10.6lf\n",
                    m_avrg_fitness);

            fprintf (m_file_ptk_ptr, "\nBest Distance                         : %10.0lf\n",
                    GetBestDistance());
            fprintf (m_file_ptk_ptr, "Average Distance                      : %10ld\n",
                    (long)(GetWorstDistance()-GetAverageFitness()*GetWorstDistance()));
            fprintf (m_file_ptk_ptr, "Worst Distance                        : %10.0lf\n",
                    GetWorstDistance());

            fprintf (m_file_ptk_ptr, "\nMutationen dieser Generation          : %3zu\n",
                    (size_t)m_mutations_this_gen);
            fprintf (m_file_ptk_ptr, "Inversionen dieser Generation         : %3zu\n",
                    (size_t)m_inversions_this_gen);
            fprintf (m_file_ptk_ptr, "Translokationen dieser Generation     : %3zu\n",
                    (size_t)m_translocations_this_gen);
            fprintf (m_file_ptk_ptr, "\nTheBestEverFitness                    : %10.6lf\n",
                    m_all_time_best.GetFitness());
            fprintf (m_file_ptk_ptr, "TheBestEversAverageFitness            : %10.6lf\n",
                    m_all_time_best_avrg_fitness);
            fprintf (m_file_ptk_ptr, "TheBestEversGeneration                : %3zu\n\n",
                    (size_t)m_all_time_best_gen);
        } else {
            if(m_generation>1) {
                fprintf (m_file_ptk_ptr, "Avrg. Generationsdauer                  : %f s / Generation\n",
                        ((double) (m_evolution_end-m_evolution_start))/((double)(GetGeneration()-1)) );
            }
            fprintf (m_file_ptk_ptr, "\nGenerationen / Evolutionsdauer        : %3zu /  %zu s\n",
                    (size_t)m_generation, (size_t)(m_evolution_end-m_evolution_start));
        }
    }
}

