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

#include "river.hpp"

using namespace jau::gentech;

double RiverGame::Play(const Chromosom& Lsg, bool Gfx)
{
  bool Regeln = true;
  int BoatDir=TO_RIVERSIDE_B;
  m_riversideA_monks=m_monks;          m_riversideB_monks=0;
  m_riversideA_cannibals=m_cannibals;  m_riversideB_cannibals=0;

  if(Gfx==true) {
    printf("\n\n\nFlussproblem mit %d Moenche und %d Kannibalen.",
     m_monks, m_cannibals );
    printf("\nLoesungschromosom hat %zu Zuege.", (size_t)Lsg.size());
    printf("\nFuer den naechsten Zug bitte Taste druecken !!!\n\n");
    printf("\n %5d M, %5d K  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
      m_riversideA_monks, m_riversideA_cannibals );
    printf(" %5d M, %5d K\n", m_riversideB_monks, m_riversideB_cannibals );
  }
  Chromosom::size_type ChromPos=0;
  while( ChromPos<Lsg.size()
	  && Regeln == true
	  && (m_riversideB_monks+m_riversideB_cannibals) /
	     (double)( m_monks+m_cannibals) < m_frontier
	)
  {
    if( BoatDir == TO_RIVERSIDE_B ) {
      // TO_RIVERSIDE_B
      Move( Lsg[ ChromPos ] );
      if(Gfx==true) {
	printf("\n\n                           --- %d M. %d K. --->     ",
	  GetMonkNumber( Lsg[ChromPos] ),
	  GetCannibalNumber( Lsg[ChromPos] ) );
	printf("\n\n %5d M, %5d K  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
	  m_riversideA_monks, m_riversideA_cannibals );
	printf(" %5d M, %5d K\n", m_riversideB_monks, m_riversideB_cannibals );
      }
      if( ( Regeln = Referee() ) == false ) {
	// Die nicht regelgerechte Transportation Rueckgaengig machen.
	MoveBack( Lsg[ ChromPos ] );
	if(ChromPos>0) {
	  // Die Rueckfahrt Rueckgaengig machen !!!
	  Move( Lsg[ ChromPos-1 ] );
	}
      }
    } else {
      // TO_RIVERSIDE_A
      MoveBack( Lsg[ ChromPos ] );
      if(Gfx==true) {
	printf("\n                           <--- %d M. %d K. ---     ",
	  GetMonkNumber( Lsg[ChromPos] ),
	  GetCannibalNumber( Lsg[ChromPos] ) );
	printf("\n %5d M, %5d K  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
	  m_riversideA_monks, m_riversideA_cannibals );
	printf(" %5d M, %5d K\n", m_riversideB_monks, m_riversideB_cannibals );
	getchar();
      }
      if( ( Regeln = Referee() ) == false ) {
	// Die nicht regelgerechte Transportation Rueckgaengig machen.
	Move( Lsg[ ChromPos ] );
      }
    }
    ChromPos++;
    BoatDir =
      ( ( BoatDir == TO_RIVERSIDE_A ) ? TO_RIVERSIDE_B : TO_RIVERSIDE_A );
  }

  if(Gfx==true) {
    printf("\n\nFitness : %lf",
      ( (double)m_riversideB_cannibals+(double)m_riversideB_monks ) /
      ( (double)m_cannibals+(double)m_monks )
    );

    if(Regeln==true && ChromPos<Lsg.size() ) {
      printf("\n\nLoesung mit %zu Zuegen gefunden !!!! <TASTE DRUECKEN>", (size_t)ChromPos );
    } else if(Regeln==false) {
      printf("\n\nKeine Loesung mit %zu Zuegen gefunden !!!! <TASTE DRUECKEN>", (size_t)ChromPos );
    } else {
      printf("\n\nLoesungschromosom ist mit %zu Zuegen zu kurz  !!!! <TASTE DRUECKEN>", (size_t)ChromPos );
    }
    getchar();
  }

  return ( ( (double)m_riversideB_cannibals+(double)m_riversideB_monks ) /
	   ( (double)m_cannibals+(double)m_monks )
	 );
}

bool RiverGame::Referee (void) const
{
  if ( m_riversideA_monks    <0               ||
       m_riversideA_cannibals<0               ||
       m_riversideB_monks    <0               ||
       m_riversideB_cannibals<0               ||
       ( m_riversideA_cannibals>m_riversideA_monks && m_riversideA_monks>0 ) ||
       ( m_riversideB_cannibals>m_riversideB_monks && m_riversideB_monks>0 )
     ) return false;

  if ( m_riversideB_cannibals-m_riversideB_monks > 4 ) {
    // Wenn mehr als 4 Kannibalen mehr am Zielufer sind,
    // so gibt es keine Moeglichkeit mehr Moenche nachzuholen !!!
    return false;
  }

  return true;
}

void RiverGame::Move( int move )
{
  int Monks = GetMonkNumber( move ),
      Cannibals = GetCannibalNumber( move );

  m_riversideA_monks -= Monks;
  m_riversideB_monks += Monks;
  m_riversideA_cannibals -= Cannibals;
  m_riversideB_cannibals += Cannibals;
}

 void RiverGame::MoveBack( int move )
 {
  int Monks = GetMonkNumber( move ),
      Cannibals = GetCannibalNumber( move );

  m_riversideA_monks += Monks;
  m_riversideB_monks -= Monks;
  m_riversideA_cannibals += Cannibals;
  m_riversideB_cannibals -= Cannibals;
 }

int RiverGame::GetMonkNumber( int move )
{
  int result = 0;
  switch( move ) {
    case 0:
    case 4: result = 1;
	    break;

    case 1: result = 2;
	    break;
  }
  return result;
}

int RiverGame::GetCannibalNumber( int move )
{
  int result=0;

  switch( move ) {
    case 2:
    case 4: result = 1;
	    break;

    case 3: result =2;
	    break;
  }

  return result;
}

