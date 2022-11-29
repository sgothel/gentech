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

#include <gentech/gentech.hpp>
#include <gentech/random.hpp>

#include <jau/basic_algos.hpp>

#include <climits>
#include <limits>
#include <iostream>

#include <cassert>

using namespace jau::gentech;

#define THIS (*this)

std::ostream& std::operator<< (std::ostream& os, const jau::gentech::nucleotide_array_t& list)
{
    const nucleotide_array_t::size_type Zeilenlaenge = 10;

    os << "( " << list.size() << " ) < ";
    for (nucleotide_array_t::size_type i=0; i<list.size(); i++) {
        if (i % Zeilenlaenge == 0 &&  i)   os << "\n\t";

        os << list[i];
        if (i % Zeilenlaenge != Zeilenlaenge-1 && i != list.size()-1) {
            os << ", ";
        }
    }
    os << " >";
	return os;
}

// *******************************************************************
// *******************************************************************
//  C H R O M O S O M
// *******************************************************************
// *******************************************************************

// STATISCHE-KLASSEN-GLOBALE VARIABLEN !!!

Chromosom::Chromosom ( Chromosomen & env, size_type initial_length )
: nucleotide_array_t(initial_length), m_env(env), m_fitness(0)
// Zufaellig erzeugte Chromosomen
{
    for ( Chromosom::size_type i=initial_length; i > 0; --i) {
        push_back( Random<size_type>(env.m_min_nucleotide_value,env.m_max_nucleotide_value) );
    }
    assert (size() == initial_length);
}

Chromosom::Chromosom ( Chromosomen & env, const std::string& FileName )
: nucleotide_array_t(), m_env(env), m_fitness(0)
{
    Load(FileName);
}

void Chromosom::Copy (const Chromosom &a)
{
    m_fitness = a.m_fitness;
    assert (size() == a.size());
}

Chromosom& Chromosom::operator=(const Chromosom& m)
{
    if (this == &m) { return *this; }

    // Neue Liste der Nukleotide anlegen.
    // In Liste<NukleoTyp> wird der *this-Zeiger auf Gleichheit ueberprueft
    // Das Environment env (Chromosomen) bleibt !
    nucleotide_array_t::operator=(m);
    Copy(m);
    return *this;
}

bool Chromosom::operator==(const Chromosom& ch) const noexcept
{
    if (this == &ch) { return true; };     // gleiches Chromosom
    if (size() != ch.size()) {    // garantiert Ungleich
        return false;
    }
    size_type i;
    for(i=0; i < size() && THIS[i] == ch[i]; ++i ) ; // Ueberprufung
    return i == size();
}

void Chromosom::Save(const std::string& FileName) const
{
    FILE *file;
    if( ( file=fopen(FileName.c_str(),"wb") ) == nullptr ) {
        ABORT("Couldn't open file '%s'", FileName.c_str());
    }

    {
        // 4-7-94, 2:51 PM
        // Speichert die Groesse des NukleoDatenTyps als erstes in der Datei
        const uint16_t NukleoLen = sizeof(nucleotide_t);
        if( fwrite (&NukleoLen, sizeof (NukleoLen), 1, file) != 1 ) {
            ABORT("Couldn't write to file '%s'", FileName.c_str());
        }
        // 4-7-94, 2:51 PM
    }
    const size_type len=size();
    if( fwrite (&len, sizeof(len), 1, file) != 1 ) {
        ABORT("Couldn't write to file '%s'", FileName.c_str());
    }
    for(size_type i=0; i<len; ++i) {
        if( fwrite (&(THIS[i]), sizeof (nucleotide_t), 1, file) != 1 ) {
            ABORT("Couldn't write to file '%s'", FileName.c_str());
        }
    }
    fclose(file);
}

void Chromosom::Load(const std::string& FileName) {
    FILE *file;
    if ((file=fopen(FileName.c_str(), "rb")) == nullptr) {
        ABORT("Couldn't open file '%s'", FileName.c_str());
    }

    {
        // 4-7-94, 3:3 PM
        // Liest als erstes die Groesse des abgespeicherten NukleoTyps
        // aus dem File
        uint16_t NukleoLen;
        if( fread (&NukleoLen, sizeof (NukleoLen), 1, file) != 1 ) {
            ABORT("Couldn't read file '%s'", FileName.c_str());
        }
        if( NukleoLen != sizeof(nucleotide_t) ) {
            std::cerr << "!! ERROR !!\n"
                    << "   Laenge des Datentyps der gespeicherten Nuklotide\n"
                    << "   ist kleiner als das abgespeicherte Format\n";
            exit (0);
        }
        // 4-7-94, 3:3 PM
    }
    // Puffer der richtigen Laenge bereit stellen
    nucleotide_t value;
    size_type len;
    if (fread (&len, sizeof (len), 1, file) == 0) {
        ABORT("Couldn't read file '%s'", FileName.c_str());
    }
    reserve(len);
    for(size_type i=0; i<len; ++i) {
        if( fread (&value, sizeof(value), 1, file) == 0 ) {
            ABORT("Couldn't read file '%s'", FileName.c_str());
        }
        insert(i, value);
    }
    fclose(file);
}

Chromosom::size_type Chromosom::GetNukleotidNumber(size_type nuckleotide_idx) const noexcept
{
    nucleotide_t nuckleotide_value=THIS[nuckleotide_idx];

    if( m_env.m_nucleotide_value_scale * (m_env.m_nucleotide_count-1) <= nuckleotide_value &&
        nuckleotide_value <= m_env.m_max_nucleotide_value )
    {
        // Trifft auch fuer Nukleotide gleich nullptr zu !!!
        return m_env.m_nucleotide_count;
    }

    for (size_type n=m_env.m_nucleotide_count-1; n > 0; --n) { // FIXME: Double '-1' here and on 'n'?
        if( m_env.m_nucleotide_value_scale * (n-1) <= nuckleotide_value &&
            nuckleotide_value < m_env.m_nucleotide_value_scale * n ) {
            return n;
        }
    }

    ABORT("Failed to determine nucleotide number of nucleotide[%zu]=%zu", (size_t)nuckleotide_idx, (size_t)nuckleotide_value);
    return 0;
}

nucleotide_t Chromosom::GetUserNukleotidValue(size_type i) const noexcept
{
    return m_env.m_nucleotide_value_scale*(i-1);
}

bool Chromosom::FindIntron(size_type &von, size_type &bis) const noexcept
{
    bool done = false;
    size_type i, i2, i3;

    if (bis-von+1 >= m_env.m_splice_code_ptr->Length)
    {
        // Spleissen..Anfang suchen
        i=von; i2=0;
        while (i <= bis && m_env.m_splice_code_ptr->SpliceCode[i2] != 0) {
            if (GetNukleotidNumber (i) == m_env.m_splice_code_ptr->SpliceCode[i2]) {
                ++i2;
                ++i;
            } else {
                i2=0;
                von=++i;
            }
        }
        if (i < bis) {
            assert (m_env.m_splice_code_ptr->SpliceCode[i2] == 0) ;
            // von gefunden...
            // Spleissen..Ende suchen
            i3=i;
            i=++i2;
            while (i3 <= bis && m_env.m_splice_code_ptr->SpliceCode[i2] != 0) {
                if (GetNukleotidNumber(i3) == m_env.m_splice_code_ptr->SpliceCode[i2]) {
                    i2++;
                } else {
                    i2=i;
                }
                i3++;
            }
            if (m_env.m_splice_code_ptr->SpliceCode[i2] == 0) {
                // bis gefunden...
                bis=i3-1;
                done=true;
            }
        }
    }
    return done;
}

Chromosom::size_type Chromosom::Splicing()
{
  size_type NNukleotide=0, IntronStart=0, IntronEnd=size()-1;
  #ifndef NDEBUG
      size_type len_old;
  #endif

  // Die Bedingung fuer den SpliceCode ist Nukleotide > 1 !
  // Nukleotide wird in Chromosomen::CalcParameter gesetzt !
  if (m_env.m_nucleotide_count <= 1) {
      // Kein Splicing , da kein SpliceCode !!!
      return 0;
  }

  while( FindIntron(IntronStart, IntronEnd) ) {
      #ifndef NDEBUG
          len_old=size();
      #endif
      assert( IntronStart <= IntronEnd );
      erase( IntronStart, IntronEnd+1 );
      assert ( len_old - ( IntronEnd - IntronStart + 1 ) == size() );
      NNukleotide+=IntronEnd-IntronStart+1;
      IntronEnd=size()-1;
  }
  return NNukleotide;
}

void Chromosom::InsertIntrons (size_type von, size_type length)
{
    size_type i, iRumpf;

    if (m_env.m_splice_code_ptr != nullptr &&
            m_env.m_splice_code_ptr->SpliceCode[0] != 0 &&
            von <= size()           &&
            length >= m_env.m_splice_code_ptr->Length)
    {
        // StartSequenz einfuegen.
        for (i=0; m_env.m_splice_code_ptr->SpliceCode[i] != 0; ++i, ++von, --length) {
            insert(von, GetUserNukleotidValue(m_env.m_splice_code_ptr->SpliceCode[i]));
        }
        // Stelle fuer Rumpf merken.
        iRumpf=von;
        // EndSequenz einfuegen.
        for (++i; m_env.m_splice_code_ptr->SpliceCode[i] != 0; ++i, ++von, --length) {
            insert(von, GetUserNukleotidValue (m_env.m_splice_code_ptr->SpliceCode[i]));
        }
        // Rumpf fuellen.
        while (length > 0) {
            insert(iRumpf, Random (m_env.m_min_nucleotide_value, m_env.m_max_nucleotide_value));
            length--;
        }
    }
}

void Chromosom::Inversion()
{
    const size_type start = Random<size_type>(0, size()-2);
    assert (start < size()-1);

    const size_type end = Random(start+1, size()-1) ;
    assert (end < size());
    assert (end > start);

    const size_type length = end-start+1;
    nucleotide_array_t save(length);

    /**
     *   S   E
     * 0123456789     01234
     * abcdefghij  -> cdefg
     *
     *   S   E
     * 0123456789     43210
     * abgfedchij  <- gfedc
     */
    for (size_type i=start; i <= end; ++i) {
        save.push_back( THIS[i] );
    }
    assert( save.size() == length );

    for (size_type i=start; i <= end; ++i)  {
        THIS[i]=save[end-i];
    }
}

void Chromosom::Translocation()
{
    const size_type start = Random<size_type>(0, size()-2);
    assert (start < size()-1);
    const size_type end = Random (start+1, size()-1) ;
    assert (end < size());
    assert (end > start);

    const size_type length = end-start+1;
    nucleotide_array_t save(length);

    for(size_type i=start; i <= end; ++i) {
        save.push_back(THIS[i]);
    }
    erase(start, end+1);
    assert( save.size() == length );

    const size_type dest = Random<size_type>( 0, size()-1 );
    for(size_type i=0; i<save.size(); ++i) {
        THIS.insert(dest+i, save[i]);
    }
}

// *******************************************************************
// *******************************************************************
//  C H R O M O S O M E N
// *******************************************************************
// *******************************************************************

// STATISCHE-KLASSEN-GLOBALE VARIABLEN !!!

const Chromosomen::size_type Chromosomen::m_mutation_freq_variance=1000;

Chromosomen::Chromosomen (nucleotide_t UserNukleoMinVal, 
                          nucleotide_t UserNukleoMaxVal,
                          size_type max_chromosom_count,
                          const std::string& StartGenFile,
                          size_type Nukleotide,
                          SpliceCodeInfo *PtrSpliceCodeInfo,
                          size_type InversionFreq,
                          size_type TranslocationFreq,
                          size_type AsymXOverFreq,
                          size_type CrossVal,
                          size_type MutationFreq,
                          size_type max_no_improving_gens)
: chromosom_array_t(max_chromosom_count),
  m_all_time_best_gen(1),
  m_all_time_best_avrg_fitness(0),
  m_all_time_best(*this),

  m_avrg_fitness(0),
  m_best_fitness(0),
  m_fitness_sum(0),

  m_max_no_improving_gens(max_no_improving_gens),
  m_no_improving_gens(0),

  m_generation(1),
  m_max_chromosom_count(max_chromosom_count),
  m_intro_code_len_sum(0),
  m_spliced_chromosom_count(0),

  m_file_ptk_ptr(nullptr),

  m_generation_start(0),
  m_generation_end(0),
  m_evolution_start(0),
  m_evolution_end(0),

  m_min_chromosom_len(0),
  m_max_chromosom_len(0),
  m_avrg_chromosom_len(0),
  m_chromosom_above_zero_count(0),

  m_mutations_this_gen(0),
  m_inversions_this_gen(0),
  m_translocations_this_gen(0),
  m_mutation_freq(MutationFreq),

  m_inversion_freq(InversionFreq),
  m_translocation_freq(TranslocationFreq),
  m_asymxover_freq(AsymXOverFreq),
  m_cross_val(CrossVal),
  m_xover_number(0), // FIXME

  m_splice_code_ptr(PtrSpliceCodeInfo),
  m_min_nucleotide_value(UserNukleoMinVal),
  m_max_nucleotide_value(UserNukleoMaxVal),
  m_nucleotide_value_scale(1),
  m_nucleotide_count(Nukleotide)
{
    // StartGene aus dem File holen ! ....
    FILE *file;

    CalcParameter();

    if ((file=fopen(StartGenFile.c_str(),"rb") ) == nullptr ) {
        ABORT("Couldn't open file '%s'", StartGenFile.c_str());
    }
    {
        // 4-7-94, 3:3 PM
        // Liest als erstes die Groesse des abgespeicherten NukleoTyps
        // aus dem File
        uint16_t NukleoLen;
        if (fread (&NukleoLen, sizeof(NukleoLen), 1, file)!=1) {
            ABORT("can't read file %s\n", StartGenFile.c_str());
        }
        if( NukleoLen != sizeof(nucleotide_t) ) {
            std::cerr << "!! ERROR !!\n"
                    << "   Laenge des Datentyps der gespeicherten Nuklotide\n"
                    << "   ist kleiner als das abgespeicherte Format\n";
            exit (0);
        }
        // 4-7-94, 3:3 PM
    }

    // int ChromLen, ChromQuantity, i, j;
    size_type ChromQuantity;
    if (fread(&ChromQuantity, sizeof(ChromQuantity), 1, file)==0) {
        ABORT("can't read file %s\n", StartGenFile.c_str());
    }

    // Puffer der richtigen Laenge bereitstellen
    nucleotide_t value;
    for(size_type j=0; j<ChromQuantity; ++j ) {
        Chromosom Gamma( *this, 0 );
        assert (Gamma.size()==0);

        size_type ChromLen;
        if (fread(&ChromLen, sizeof(ChromLen), 1, file) == 0) {
            ABORT("can't read file %s\n", StartGenFile.c_str());
        } // FIXME: Use fixed width type
        for(size_type i=0; i<ChromLen; ++i) {
            if (fread(&value, sizeof(value), 1, file) == 0) {
                ABORT("can't read file %s\n", StartGenFile.c_str());
            }
            Gamma.push_back(value);
        }
        assert (Gamma.size()==ChromLen);
        push_back(Gamma);
    }
    assert (size()==ChromQuantity);
    fclose (file);
}

Chromosomen::Chromosomen ( nucleotide_t UserNukleoMinVal, nucleotide_t UserNukleoMaxVal,
                           size_type max_chromosom_count,
                           size_type StartChromosomNumber,
                           size_type StartChromosomLength,
                           size_type Nukleotide,
                           SpliceCodeInfo *PtrSpliceCodeInfo,
                           size_type InversionFreq,
                           size_type TranslocationFreq,
                           size_type AsymXOverFreq,
                           size_type CrossVal,
                           size_type MutationFreq,
                           size_type max_no_improving_gens)
: chromosom_array_t(max_chromosom_count),
  m_all_time_best_gen(1),
  m_all_time_best_avrg_fitness(0),
  m_all_time_best(*this),

  m_avrg_fitness(0),
  m_best_fitness(0),
  m_fitness_sum(0),

  m_max_no_improving_gens(max_no_improving_gens),
  m_no_improving_gens(0),

  m_generation(1),
  m_max_chromosom_count(max_chromosom_count),
  m_intro_code_len_sum(0),
  m_spliced_chromosom_count(0),

  m_file_ptk_ptr(nullptr),

  m_generation_start(0),
  m_generation_end(0),
  m_evolution_start(0),
  m_evolution_end(0),

  m_min_chromosom_len(0),
  m_max_chromosom_len(0),
  m_avrg_chromosom_len(0),
  m_chromosom_above_zero_count(0),

  m_mutations_this_gen(0),
  m_inversions_this_gen(0),
  m_translocations_this_gen(0),
  m_mutation_freq(MutationFreq),

  m_inversion_freq(InversionFreq),
  m_translocation_freq(TranslocationFreq),
  m_asymxover_freq(AsymXOverFreq),
  m_cross_val(CrossVal),
  m_xover_number(0), // FIXME

  m_splice_code_ptr(PtrSpliceCodeInfo),
  m_min_nucleotide_value(UserNukleoMinVal),
  m_max_nucleotide_value(UserNukleoMaxVal),
  m_nucleotide_value_scale(1),
  m_nucleotide_count(Nukleotide)
{
    CalcParameter();

    // StartGene zufaellig setzen !
    for (size_type i=0; i<StartChromosomNumber ; ++i) {
        Chromosom Gamma (*this, StartChromosomLength);
        assert (Gamma.size() == StartChromosomLength);
        push_back(Gamma);
    }
    assert (size()==StartChromosomNumber);
}

void Chromosomen::Save (const std::string& FileName) const
{
    FILE *file;

    if((file=fopen(FileName.c_str(), "wb")) == nullptr) {
        ABORT("Couldn't open file '%s'", FileName.c_str());
    }
    {
        // 4-7-94, 2:51 PM
        // Speichert die Groesse des NukleoTyps als erstes im File ab
        const uint16_t NukleoLen=sizeof(nucleotide_t);
        if (fwrite (&NukleoLen, sizeof (NukleoLen), 1, file) != 1) {
            ABORT("can't write to file %s\n", FileName.c_str());
        }
        // 4-7-94, 2:51 PM
    }
    size_type ChromQuantity = size();
    if (fwrite (&ChromQuantity, sizeof (ChromQuantity), 1, file) != 1 ) {
        ABORT("can't write to file %s\n", FileName.c_str());
    }
    for (size_type j=0; j<ChromQuantity; j++ ) {
        const size_type ChromLen = THIS[j].size();
        if (fwrite (&ChromLen, sizeof (ChromLen), 1, file) != 1) {
            ABORT("can't write to file %s\n", FileName.c_str());
        }
        for(size_type i=0; i<ChromLen; ++i) {
            nucleotide_t value = THIS[j][i];
            if (fwrite (&value, sizeof (nucleotide_t), 1, file) != 1) {
                ABORT("can't write to file %s\n", FileName.c_str());
            }
        }
    }
    fclose(file);
}

Chromosomen::size_type Chromosomen::Evolution (double GoalFitness, const std::string& chrptrPtkFile,
			                                   double BirthRate, int Bigamie )
{
    double BestEverAverageFitness = -1,
           Cut = 0; // Der Cut beim Sterben
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
    m_no_improving_gens = 0;
    Protokoll();
    if( !Echo() ) { stop = true; }

    if( BirthRate <= 0.0 || BirthRate > 1.0 ) { stop = true; }

    while( m_best_fitness < GoalFitness &&
           m_no_improving_gens < m_max_no_improving_gens &&
           !stop )
    {
        m_generation++;
        m_spliced_chromosom_count=0;
        m_intro_code_len_sum=0;
        if( BirthRate < 1.0 ) {
            NewGeneration(BirthRate, Bigamie);
            // Fuer LetDie !!!
            Cut = GetXWorstFitness( size() - m_max_chromosom_count );
        } else {
            NewGeneration(Bigamie);
            Cut = -.5;	// Eltern Fitness auf -1 gesetzt
        }
        // Sterben !!!
        LetDie(Cut);
        // Die Mutation
        Mutation();
        // Fitness berechnen
        CalcWholeFitness();
        // Weitere Mutationen
        InversionsMutation();
        TranslocationsMutation();

        m_generation_end=time(nullptr);

        if( BestEverAverageFitness < m_avrg_fitness ) {
            m_no_improving_gens=0;
            BestEverAverageFitness = m_avrg_fitness ;
        } else {
            m_no_improving_gens++ ;
        }
        Protokoll();
        if(!Echo()) { stop = true; }
    }

    m_evolution_end=time(nullptr);
    Echo();
    Protokoll();

    if (m_file_ptk_ptr != nullptr) { fclose (m_file_ptk_ptr); }

    return m_generation;
}

void Chromosomen::InitFitness()
{
    for (size_type i = 0; i < size(); ++i) {
        Chromosom Neu = THIS[i];
        Neu.Splicing();
        THIS[i].SetFitness(Fitness(Neu));
    }
    CalcWholeFitness();
}

void Chromosomen::NewGeneration(double BirthRate, bool Bigamie)
{
    indexlist_t ElternPaare;

    size_type l = (size_type) ( (double)size() * BirthRate );
    size_type i;

    m_generation_start=time(nullptr);

    // Gerade Anzahl der ElternPaare !!!
    if( l%2 > 0 ) {
        --l;
    }

    // ooops, die letzten beiden Elternpaare sind nicht-bigamistisch SCHLECHT
    // zu finden !!!
    if( 2 <= m_chromosom_above_zero_count && m_chromosom_above_zero_count <= l ) {
        m_chromosom_above_zero_count -= 2;
    }

    // Neue Partner in die ElternPaare-Menge einfuegen
    // "Bigamistische Beziehung" werden, wenn moeglich, nicht erlaubt.
    while(l>0) {
        // Nur NEUE Partner !!!
        if (!Bigamie && ElternPaare.size() < m_chromosom_above_zero_count) {
            do {
                i=RouletteSelect();
            } while( jau::contains(ElternPaare, i) ) ;
            ElternPaare.push_back(i);
            l--;
        } else {
            if (!Bigamie)   {
                // the hard way ...
                do {
                    i = Random<size_type>(0, size()-1);
                } while( jau::contains(ElternPaare, i) ) ;
                ElternPaare.push_back(i);
                l--;
            } else {
                // Bigamistische Beziehung JA, aber sexuell !!!
                size_type w, m;
                ElternPaare.push_back((w = Random<size_type>(0, size()-1))) ;
                while (w == (m=Random<size_type>(0, size()-1))) ;
                ElternPaare.push_back(m) ;
                l-=2;
            }
        }
    };

    l=ElternPaare.size();

    #ifndef NDEBUG
        // alles ok ?? GERADE ELTERNPAARE ????
        assert( l%2==0 );
        assert( l== ( (size_type)( size() * BirthRate ) % 2 ) ?
                    (size_type)( size() * BirthRate ) - 1 :
                    (size_type)( size() * BirthRate )
        );
    #endif

    // Fortpflanzen !!!
    while (l > 1) {
        CrossingOver (ElternPaare[l-1], ElternPaare[l-2]);
        l -= 2 ;
    }
    assert (l == 0);
}


void Chromosomen::NewGeneration(bool Bigamie)
{
    indexlist_t ElternPaare;
    size_type l = m_max_chromosom_count;

    m_generation_start=time(nullptr);

    // Gerade Anzahl der ElternPaare !!!
    if( l%2 > 0 ) {
        --l;
    }

    // ooops, die letzten beiden Elternpaare sind nicht-bigamistisch SCHLECHT
    // zu finden !!!
    if( 2 <= m_chromosom_above_zero_count && m_chromosom_above_zero_count <= l ) {
        m_chromosom_above_zero_count -= 2;
    }

    // Neue Partner in die ElternPaare-Menge einfuegen
    // "Bigamistische Beziehung" werden, wenn moeglich, nicht erlaubt.
    while( l > 0 ) {
        // Nur NEUE Partner !!!
        if( !Bigamie && ElternPaare.size() < m_chromosom_above_zero_count ) {
            size_type i;
            do {
                i = RouletteSelect();
            } while ( jau::contains(ElternPaare, i) ) ;
            ElternPaare.push_back(i);
            l--;
        } else {
            if( !Bigamie ) {
                size_type i;
                do {
                    i = Random<size_type>(0, size()-1);
                } while( jau::contains(ElternPaare, i) ) ;
                ElternPaare.push_back(i);
                l--;
            } else { // Bigamistische Beziehung JA, aber sexuell !!!
                size_type w, m;
                ElternPaare.push_back( ( w = Random<size_type>(0, size()-1) ) );
                while( w == ( m = Random<size_type>(0, size()-1) ) ) ;
                ElternPaare.push_back(m);
                l-=2;
            }
        }
    };

    l=ElternPaare.size();

    // alles ok ?? GERADE ANZAHL DER ELTERN !!!
    assert (l%2==0);
    (void)l;

    // Neue Population...
    l=ElternPaare.size();
    while (l > 1) {
        // Beim Crossing Over werden die Nachkommen am Ende der Populations-Liste
        // eingefuegt.
        // Die Indizes fuer die ElternPaare bleiben somit aktuell !!!
        CrossingOver (ElternPaare[l-1], ElternPaare[l-2]);
        l-=2;
    }

    // Eltern markieren, damit sie "getoetet" werden koennen !!!
    // Das geschied in 'LetDie', Aufruf in Evolution
    l=ElternPaare.size()-1;
    while (l > 0) {
        const size_type w=ElternPaare[l-1];
        // markieren !!!
        THIS[w].SetFitness(-1);
        l--;
    }
}

void Chromosomen::LetDie(double cut)
{
    size_type i=0;

    while (i < size() && size() > m_max_chromosom_count) {
        if (THIS[i].GetFitness() <= cut) {
            Kill (i);
        } else {
            ++i;
        }
    }

    // Sicherheitsabfrage
    if (size() > m_max_chromosom_count) {
        cut = GetXWorstFitness (size()-m_max_chromosom_count);
        i = 0;
        while (i < size() && size() > m_max_chromosom_count) {
            if (THIS[i].GetFitness() <= cut) {
                Kill (i);
            } else {
                ++i;
            }
        }
    }
    assert (size() <= m_max_chromosom_count);
}

Chromosomen::size_type Chromosomen::RouletteSelect() const
// Chris: geaendert
{
    size_type i;

    double Sum       = 0.0;
    const double Threshold = Random (0.0, 1.0) * GetFitnessSum();

    // Aufgrund der Ungenauigkeit der Flieskommaoperatoren
    // muss auch das Ende der Liste abgefragt werden.
    for (i=0; Sum < Threshold && i<size(); ++i) {
        Sum += THIS[i].GetFitness();
    }
    return ( i < size() ) ? i : size()-1;
}

void Chromosomen::CreateNewSymChromosom ( Chromosom &dest, size_type m, size_type w,
                                          crosspoints_t &CrossPoints )
{
    // i          : Indize des Crosspoints
    // von, bis   : Zu Uebertragender Chromosomenabschnitt [von..bis[
    // ch         : Alternierendes Indize zwischen den beiden Chromosomen
    //              mit den Geschlechtern 'm' und 'w'
    // done       : Abbruch Indikator
    bool done = false;
    size_type i, von, bis, ch;

    // Startwerte !!
    i = bis = 0;
    ch = w;

    dest.reserve( THIS[ch].size() );

    do {
        // An dem letzten exklusiven Ende fortfahren
        von = bis;
        if ( i < CrossPoints.size() ) {
            // Chromosomenabschnitt-Ende holen.
            if( CrossPoints[i] < THIS[ch].size() ) bis = CrossPoints[i];
            else                                   bis = THIS[ch].size();
            ++i;
        } else {
            // ...und den Rest uebertragen
            bis = THIS[ch].size();
            done = true;
        }
        assert ( bis <= THIS[ch].size() );
        assert ( von == dest.size() );
        // Chromosomenabschnitt uebertragen [von..bis[
        for (size_type n=von; n<bis; n++) {
            dest.insert(n, THIS[ch][n]);
        }
        // Indize zwischen den Geschlechtern alternieren
        ch = ( ch==m ? w : m ) ;
    } while ( !done );

    // ...sonst Fehler !!!
    assert( dest.size() > 0 );
}

void Chromosomen::CrossingOver (size_type m, size_type w)
{
    if ( m_cross_val == 0 ) { return; }

    size_type shortest=( ( THIS[m].size() < THIS[w].size() ) ? m : w);

    if ( m_xover_number++ < m_asymxover_freq || m_asymxover_freq==0 ) {
        // Symmetrisches XOver !!!
        crosspoints_t CrossPoints;
        assert( CrossPoints.size()==0 );

        // Kreuzungspunkte sortiert eintragen.
        for ( size_type i = 0; i < m_cross_val; ++i ) {
            CrossPoints.insert( Random<size_type>(0 , THIS[shortest].size()) );
        }

        Chromosom NeuA (*this, 0);
        Chromosom NeuB (*this, 0);
        size_type SplicedCode;

        CreateNewSymChromosom ( NeuA, m, w, CrossPoints );
        push_back( NeuA );
        SplicedCode=NeuA.Splicing();
        if(SplicedCode>0) {
            m_spliced_chromosom_count++;
            m_intro_code_len_sum+=SplicedCode;
        }
        // Die Fitness des gespleissten Chromosomes in das ungespleisste
        // eingebundene Chromosom einsetzen !!!
        THIS[size()-1].SetFitness(Fitness(NeuA));

        CreateNewSymChromosom ( NeuB, w, m, CrossPoints );
        push_back( NeuB );
        SplicedCode=NeuB.Splicing();
        if(SplicedCode>0) {
            m_spliced_chromosom_count++;
            m_intro_code_len_sum+=SplicedCode;
        }
        // Die Fitness des gespleissten Chromosomes in das ungespleisste
        // eingebundene Chromosom einsetzen !!!
        THIS[size()-1].SetFitness(Fitness(NeuB));

    } else {

        m_xover_number = 0;
        // Assymetrisches XOver : Nur ein neues Element !
        Chromosom Neu (*this, 0);

        // Kopieren
        Neu=THIS[m];

        // anhaengen
        for (size_type n=0; n<THIS[w].size(); n++) {
            Neu.push_back( THIS[w][n] );
        }
        Neu.SetFitness(Fitness(Neu));
        push_back( Neu );

    }
}

void Chromosomen::Mutation()
{
    static size_type next_nukleotide_idx = m_mutation_freq + Random<size_type>( 0, (size_type)(m_mutation_freq/m_mutation_freq_variance) ) ;
    m_mutations_this_gen=0;

    if ( m_mutation_freq > 0 ) {
        size_type nukleotide_idx = next_nukleotide_idx;
        for(size_type chromosom_idx = 0; chromosom_idx < size(); ++chromosom_idx ) {
            bool mutated = false;
            const size_type chromosom_len = THIS[chromosom_idx].size();
            while( nukleotide_idx < chromosom_len ) {
                // Die Mutation.
                (THIS[chromosom_idx])[nukleotide_idx] = Random<size_type>( m_min_nucleotide_value, m_max_nucleotide_value);
                m_mutations_this_gen++;
                mutated=true;
                nukleotide_idx += m_mutation_freq + Random<size_type>( 0, (size_type)(m_mutation_freq/m_mutation_freq_variance) );
            }
            if( nukleotide_idx >= chromosom_len ) {
                nukleotide_idx -= chromosom_len;
            }
            if(mutated) {
                // Fitness neuberechnen ...
                // ... this is a social event - we are hard folks :-)
                Chromosom Neu (THIS[chromosom_idx]);
                Neu.Splicing();
                THIS[chromosom_idx].SetFitness(Fitness(Neu));
            }
        }
        next_nukleotide_idx = nukleotide_idx;
    }
}


void Chromosomen::InversionsMutation()
{
    static size_type next_chromosomen_idx = m_inversion_freq;
    m_inversions_this_gen=0;
    size_type chromosomen_idx = next_chromosomen_idx;
    while( chromosomen_idx < size() ) {
        m_inversions_this_gen++;
        THIS[chromosomen_idx].Inversion();
        // Fitness neuberechnen ...
        Chromosom Neu (THIS[chromosomen_idx]);
        Neu.Splicing();
        THIS[chromosomen_idx].SetFitness(Fitness(Neu));
        chromosomen_idx += m_inversion_freq;
    }
    if( chromosomen_idx >= size() ) {
        chromosomen_idx -= size();
    }
    next_chromosomen_idx = chromosomen_idx;
}

void Chromosomen::TranslocationsMutation()
{
    static size_type next_chromosomen_idx = m_translocation_freq;
    m_translocations_this_gen=0;
    size_type chromosomen_idx = next_chromosomen_idx;
    while( chromosomen_idx < size() ) {
        m_translocations_this_gen++;
        THIS[chromosomen_idx].Translocation();
        // Fitness neuberechnen ...
        Chromosom Neu (THIS[chromosomen_idx]);
        Neu.Splicing();
        THIS[chromosomen_idx].SetFitness(Fitness(Neu));
        chromosomen_idx += m_translocation_freq;
    }
    if( chromosomen_idx >= size() ) {
        chromosomen_idx -= size();
    }
    next_chromosomen_idx = chromosomen_idx;
}

void Chromosomen::CalcWholeFitness()
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
        if( m_best_fitness < TempFitness ) {
            m_best_fitness = TempFitness;
            BestChrom = i;
        }
    }
    m_avrg_fitness = Total / (double)size();
    m_fitness_sum = Total;
    m_avrg_chromosom_len = (double)ChromLenSum/(double)size();
    if( m_all_time_best.GetFitness() < m_best_fitness && npos != BestChrom) {
        m_all_time_best=THIS[BestChrom];
        m_all_time_best.Splicing();
    }
    if (m_all_time_best_avrg_fitness < m_avrg_fitness) {
        m_all_time_best_avrg_fitness = m_avrg_fitness;
        m_all_time_best_gen = m_generation;
    }
}

Chromosomen::size_type Chromosomen::GetWorstChromosom() const
{
    double WorstFitness=2, TempFitness;
    size_type iw = npos;

    for (size_type i=0; i<size(); ++i) {
        TempFitness=THIS[i].GetFitness();
        if (WorstFitness > TempFitness) {
            WorstFitness = TempFitness ;
            iw=i;
        }
    }
    return iw;
}

double Chromosomen::GetXWorstFitness(size_type count) const
{
    doubles_sorted_t Worst;

    if (count == 0) { return -1; } // Falsche Anzahl uebergeben
    for (size_type i=0; i<size(); ++i) {
        if( Worst.size() < count ||
            THIS[i].GetFitness() < Worst[count-1] ) 
        {
            Worst.insert(THIS[i].GetFitness());
        }
    }
    if (Worst.size() >= count) {
        return Worst[count-1];
    } else {
        return Worst[Worst.size()-1];
    }
}

Chromosomen::size_type Chromosomen::GetBestChromosom() const
{
    double BestFitness=-1, TempFitness;
    size_type ib = npos;

    for (size_type i=0; i<size(); ++i) {
        TempFitness=THIS[i].GetFitness();
        if (BestFitness < TempFitness) {
            BestFitness = TempFitness ;
            ib=i;
        }
    }
    return ib;
}

const Chromosom &Chromosomen::GetTheBestEverChromosom()
// Hier kein kein Slicing mehr, da beim abspeichern des besten
// Chromosoms in der Variablen 'TheBestEver' der Code schon
// gesplict abgespeichert wird.
{
  return (const Chromosom&) m_all_time_best;
}

void Chromosomen::CalcParameter()
{
    if (m_splice_code_ptr==nullptr ||
        m_splice_code_ptr->SpliceCode==nullptr ||
        m_splice_code_ptr->SpliceCode[0]==0 )
    {
        // Ohne Splicing ....
        m_nucleotide_count=0;
        m_nucleotide_value_scale=1;
    } else {
        if (m_splice_code_ptr->Length ==0) {
            // Laenge berechnen ...
            int i;

            for (i=0; m_splice_code_ptr->SpliceCode[i]!=0; ++i) {
                m_splice_code_ptr->Length ++;
            }
            for (++i; m_splice_code_ptr->SpliceCode[i]!=0; ++i) {
                m_splice_code_ptr->Length ++;
            }
        }
        if(m_nucleotide_count>0) {
            m_nucleotide_value_scale = ( m_max_nucleotide_value - m_min_nucleotide_value + 1 ) / m_nucleotide_count ;
        } else {
            m_nucleotide_value_scale = 1;
        }
    }
}

void Chromosomen::Protokoll()
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

bool Chromosomen::Echo() const
{
    if (m_generation == 1) {
        printf(" Generation: BestGeneration: AverageFitness: BestFitness:"
                " TheBestLength:\n");
    }
    printf("\r%11zu%16zu%16.9lf%13.9lf%15zu",
            (size_t)GetGeneration(), (size_t)m_all_time_best_gen,
            GetAverageFitness(), GetBestFitness(),
            (size_t)m_all_time_best.size() );

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

void Chromosom::Ausgabe (std::ostream& OS) const noexcept
{
    const int Zeilenlaenge = 10;

    OS << "( l " << size() << ", f "
       << ( ( GetFitness() < std::numeric_limits<double>::epsilon() ) ? (double)(0) : GetFitness() )
       << " ) < ";
    for (size_type i=0; i<size(); ++i) {
        if (i % Zeilenlaenge == 0 &&  i) {
            OS << "\n\t";
        }
        double wandler = THIS[i];
        OS << wandler;
        if (i % Zeilenlaenge != Zeilenlaenge-1 && i != size()-1) {
            OS << ", ";
        }
    }
    OS << " >";
}

void Chromosomen::Ausgabe (std::ostream& OS) const noexcept
{
    const int Zeilenlaenge = 7;

    OS << "( " << size() << " ) < ";
    for (size_type i=0; i<size(); ++i) {
        if (i % Zeilenlaenge == 0 &&  i) {
            OS << "\n\t";
        }
        OS << ( ( THIS[i].GetFitness() < std::numeric_limits<double>::epsilon() ) ? (double)(0) : THIS[i].GetFitness() );
        if (i % Zeilenlaenge != Zeilenlaenge-1 && i != size()-1) {
            OS << ", ";
        }
    }
    OS << " >";
}

/*
std::ostream& operator << (std::ostream& OS, Liste<NukleoTyp>& Ch)
{
  Ch.Ausgabe (OS);
  return OS;
}
*/
# ifdef TEST
    // Der Testteil
    # include "error.cpp"
    # include "maschine.cpp"

    int main()
    {
      Chromosom test1(0, 4, 5, nullptr, 50), test2(0, 4, 5, nullptr, 60);

      randomize();
      if (test1 == test1)
	cout << "\ntest2\n" << test2;
      else
	cerr << "\nopps: Operator \"==\" arbeitet nicht korekt !!";

      if (test1 != test2)
	cout << "\ntest1\n" << test1;
      else
	cerr << "\nopps: Operator \"!=\" arbeitet nicht korekt !!";

      test2 = test1;
      getch();

      if (test2 == test1)
	cout << "\ntest2 nach Zuweisung\n" << test2;
      else
	cerr << "\nopps: Operator \"==\" arbeitet nicht korekt !!";

      while (!kbhit())  {
	Chromosom test3(0, Random (0, 10), 5, nullptr, Random (30, 50));
	cout << endl << test3;
	getch();
      }
      return 0;
    }
# endif

