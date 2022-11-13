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

#include "river_gen.hpp"

using namespace jau::gentech;

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
			    size_type MutationFreq,
			    size_type max_no_improving_gens
			  )
: Chromosomen ( RiverGame::CODE_MIN_VAL, RiverGame::CODE_MAX_VAL,
                MaxChromosomen, StartChromosomNumber,
                StartChromosomLength, Nukleotide, ptrSpliceCodeInfo,
                InversionFreq, TranslocationFreq, AsymXOverFreq,
                CrossVal, MutationFreq, max_no_improving_gens ),
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
			    size_type MutationFreq,
			    size_type max_no_improving_gens
			  )
: Chromosomen ( RiverGame::CODE_MIN_VAL, RiverGame::CODE_MAX_VAL,
                MaxChromosomen, StartGenFile,
                Nukleotide, ptrSpliceCodeInfo,
                InversionFreq, TranslocationFreq, AsymXOverFreq,
                CrossVal, MutationFreq, max_no_improving_gens ),
  TheGame( Monk, Cannibal, 1 )
{ }

double RiverProblem::Fitness (const jau::gentech::Chromosom &lsg) {
    return ( TheGame.Play(lsg, false) );
}










