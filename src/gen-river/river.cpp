// river.cpp                                                     6.3.1994
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

# include "river.h"

double RiverGame::Play(const Chromosom& Lsg, bool Gfx)
{
  int ChromPos=0, Regeln = true, BoatDir=TO_RIVERSIDE_B;
  RiversideAMonk=Monk;          RiversideBMonk=0;
  RiversideACannibal=Cannibal;  RiversideBCannibal=0;

  if(Gfx==true) {
    printf("\n\n\nFlussproblem mit %d Moenche und %d Kannibalen.",
     Monk, Cannibal );
    printf("\nLoesungschromosom hat %d Zuege.", Lsg.laenge());
    printf("\nFuer den naechsten Zug bitte Taste druecken !!!\n\n");
    printf("\n %5d M, %5d K  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
      RiversideAMonk, RiversideACannibal );
    printf(" %5d M, %5d K\n", RiversideBMonk, RiversideBCannibal );
  }
  while (    ChromPos<Lsg.laenge()
	  && Regeln == true
	  && (RiversideBMonk+RiversideBCannibal) /
	     (double)( Monk+Cannibal) < Frontier
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
	  RiversideAMonk, RiversideACannibal );
	printf(" %5d M, %5d K\n", RiversideBMonk, RiversideBCannibal );
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
	  RiversideAMonk, RiversideACannibal );
	printf(" %5d M, %5d K\n", RiversideBMonk, RiversideBCannibal );
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
      ( (double)RiversideBCannibal+(double)RiversideBMonk ) /
      ( (double)Cannibal+(double)Monk )
    );

    if(Regeln==true && ChromPos<Lsg.laenge() ) {
      printf("\n\nLoesung mit %d Zuegen gefunden !!!! <TASTE DRUECKEN>",
	ChromPos );
    } else if(Regeln==false) {
      printf("\n\nKeine Loesung mit %d Zuegen gefunden !!!! <TASTE DRUECKEN>",
	ChromPos );
    } else {
      printf("\n\nLoesungschromosom ist mit %d Zuegen zu kurz  !!!! <TASTE DRUECKEN>",
	ChromPos );
    }
    getchar();
  }

  return ( ( (double)RiversideBCannibal+(double)RiversideBMonk ) /
	   ( (double)Cannibal+(double)Monk )
	 );
}

bool RiverGame::Referee (void) const
{
  if ( RiversideAMonk    <0               ||
       RiversideACannibal<0               ||
       RiversideBMonk    <0               ||
       RiversideBCannibal<0               ||
       ( RiversideACannibal>RiversideAMonk && RiversideAMonk>0 ) ||
       ( RiversideBCannibal>RiversideBMonk && RiversideBMonk>0 )
     ) return false;

  if ( RiversideBCannibal-RiversideBMonk > 4 ) {
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

  RiversideAMonk -= Monks;
  RiversideBMonk += Monks;
  RiversideACannibal -= Cannibals;
  RiversideBCannibal += Cannibals;
}

 void RiverGame::MoveBack( int move )
 {
  int Monks = GetMonkNumber( move ),
      Cannibals = GetCannibalNumber( move );

  RiversideAMonk += Monks;
  RiversideBMonk -= Monks;
  RiversideACannibal += Cannibals;
  RiversideBCannibal -= Cannibals;
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

