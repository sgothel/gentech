// river.h							 6.3.1994
//
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

# ifndef _RIVER_H
  # define _RIVER_H

  # include <stdio.h>
  # include "gentech.h"

  class RiverGame {
  public:
    // Wertebereich des Nukleotids auf dem Chromosom,
    // welcher fuer dieses Spiel relevant ist.
    enum { CODE_MIN_VAL=0, CODE_MAX_VAL=4 };
    RiverGame (int Monk, int Cannibal, double Frontier )
    : Monk(Monk), Cannibal(Cannibal), Frontier((int)Frontier) {}
    // Spielen...
    enum { TO_RIVERSIDE_A = 1, TO_RIVERSIDE_B = 2 };
    double Play(const Chromosom& Lsg, int Gfx) ;
    // Der Schiedsrichter ...
    int Referee (void) const;
  protected:
    int Frontier;
    int Cannibal;
    int Monk;
    int RiversideAMonk;
    int RiversideBMonk;
    int RiversideACannibal;
    int RiversideBCannibal;
  private:
    void Move( int );
    void MoveBack( int );
    int GetMonkNumber( int );
    int GetCannibalNumber( int );
  };

# endif
