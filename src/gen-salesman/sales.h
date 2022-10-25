// sales.h                                                       6.3.1994
//
// Diese Modul wurde fuer eine Hausarbeit im Fach
// Objektorietierter Programierung (OOP) an der FH Bielefeld
// unter der Betreuung von Prof. Dr. Bunse erstellt.
//
// Autoren der Hausarbeit : Sven Goethel und Christian Mueller
//
//
// Jegliches Copyright aller Dateien ist im Besitz der Autoren.
// Sven Goethel * Stapenhorststr.35a * 33615 Bielefeld * 0521/139228
// Bielefeld, den 11.3.1994.

# ifndef _SALES_H
  # define _SALES_H

  # ifdef __BORLANDC__
      # include <conio.h>
  # else
      # include <stdio.h>
  # endif

  # include "gentech.h"
  class SalesGame {
  public:
    // Wertebereich des Nukleotids auf dem Chromosom,
    // welcher fuer dieses Spiel relevant ist.
    enum { CODE_MIN_VAL = 0, CODE_MAX_VAL = 24 };
    SalesGame (int monk, 	int cannibal,
	       double frontier, double basis = .7
	      )
    : Frontier((int)frontier) {}
    // Spielen...
    enum { TO_RIVERSIDE_A = 1, TO_RIVERSIDE_B = 2 };
    double Play(const Chromosom& Lsg, int Gfx) const ;
  protected:
    int Frontier;
  };

# endif
