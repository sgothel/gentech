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

#ifndef _SALES_H
#define _SALES_H

#include <cstdio>
#include <gentech/gentech.hpp>

class SalesGame {
    public:
        // Wertebereich des Nukleotids auf dem Chromosom,
        // welcher fuer dieses Spiel relevant ist.
        enum { CODE_MIN_VAL = 0,
               CODE_MAX_VAL = 24 };

        SalesGame(double frontier)
        : Frontier((int)frontier) {}

        // Spielen...
        enum { TO_RIVERSIDE_A = 1,
               TO_RIVERSIDE_B = 2 };

        double Play(const jau::gentech::Chromosom& Lsg, bool Gfx) const;

    protected:
        int Frontier;
};

#endif
