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

#ifndef _RIVER_H
#define _RIVER_H

#include <cstdio>
#include <gentech/gentech.hpp>

class RiverGame {
    public:
        // Wertebereich des Nukleotids auf dem Chromosom,
        // welcher fuer dieses Spiel relevant ist.
        enum { CODE_MIN_VAL=0, CODE_MAX_VAL=4 };

        RiverGame (int monks, int cannibals, double Frontier )
        : m_frontier((int)Frontier), m_cannibals(cannibals), m_monks(monks)
        {}

        // Spielen...
        enum { TO_RIVERSIDE_A = 1, TO_RIVERSIDE_B = 2 };

        double Play(const jau::gentech::Chromosom& Lsg, bool Gfx) ;

        // Der Schiedsrichter ...
        bool Referee () const;

    protected:
        int m_frontier;
        int m_cannibals;
        int m_monks;
        int m_riversideA_monks = 0;
        int m_riversideB_monks = 0;
        int m_riversideA_cannibals = 0;
        int m_riversideB_cannibals = 0;

    private:
        void Move( int );
        void MoveBack( int );
        int GetMonkNumber( int );
        int GetCannibalNumber( int );
};

#endif
