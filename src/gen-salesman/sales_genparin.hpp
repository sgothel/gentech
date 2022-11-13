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

#ifndef SALES_GENPARIN_H
#define SALES_GENPARIN_H

#include <string>

#include <gentech/gentech.hpp>

// external template function definition in .cpp
template <class T>
extern void Input ( const std::string& question, T &dest, T def );

enum { WRITE_MODE, READ_MODE };

std::string FileNameInput ( int mode, const std::string& end );

class GenParameter {
    public:
        void Input( int NukleoMinVal, int NukleoMaxVal, jau::gentech::nucleotide_t *ptrSpliceCode=nullptr );
        std::string FileName;
        std::string FileNameHeader;
        jau::gentech::SpliceCodeInfo TheSpliceCodeInfo;
        double BirthRate;
        int Bigamie;
        int min_nucleotide_value;
        int max_nucleotide_value;
        int max_chromosom_count;
        int init_chromosom_count;
        int init_chromosom_len;
        int nucleotide_count;
        long InversionFreq;
        long TranslocationFreq;
        long AsymXOverFreq;
        int CrossVal;
        long MutationFreq;
        int MaxNoImprove;
    private:
        jau::gentech::nucleotide_t TheSpliceCode[100];
};

#endif
