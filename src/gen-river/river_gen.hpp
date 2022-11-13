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

#ifndef RIVER_GEN_H
#define RIVER_GEN_H

#include <string>

#include <gentech/gentech.hpp>

#include "river.hpp"

class RiverProblem : public jau::gentech::Chromosomen {
    public:
        // Der Konstruktor mit allen Einstellungen zum Loesen des Problems.
        RiverProblem( int Monk, int Cannibal,
                size_type MaxChromosomen,
                size_type StartChromosomNumber,
                size_type StartChromosomLength,
                size_type Nukleotide=3,
                jau::gentech::SpliceCodeInfo *ptrSpliceCodeInfo=nullptr,
                size_type InversionFreq=0,
                size_type TranslocationFreq=0,
                size_type AsymXOverFreq=0,
                size_type CrossVal=1,
                size_type MutationFreq=10000, // [1]
                size_type max_no_improving_gens=100
        ) ;
        RiverProblem( int Monk, int Cannibal,
                size_type MaxChromosomen,
                const std::string& StartGenFile,
                size_type Nukleotide=3,
                jau::gentech::SpliceCodeInfo *ptrSpliceCodeInfo=nullptr,
                size_type InversionFreq=0,
                size_type TranslocationFreq=0,
                size_type AsymXOverFreq=0,
                size_type CrossVal=1,
                size_type MutationFreq=10000,
                size_type max_no_improving_gens=100
        ) ;

        // Fitness fuer das uebergebene Chromosom.
        // Wert [0..1] !
        double Fitness(const jau::gentech::Chromosom&) override;

        // Und das Spiel selbst als Element ...
        RiverGame TheGame;
};

#endif

