// author : Sven Gothel - Stapenhorststrasse 35a - 33615 Bielefeld
//
// maschine.cpp     
// Diese Modul wurde fuer eine Hausarbeit im Fach
// Objektorietierter Programierung (OOP) an der FH Bielefeld
// unter der Betreuung von Prof. Dr. Bunse erstellt.
//
// Autoren der Hausarbeit : Sven Gothel und Christian Mueller
//
//
// Jegliches Copyright aller Dateien ist im Besitz der Autoren.
// Sven Gothel * Stapenhorststr.35a * 33615 Bielefeld * 0521/139228
// Bielefeld, den 11.3.1994.
//
// Dieses Modul enthaelt Maschinentypische Variable und Methoden
/*
 *
 *  $Log: maschine.cc $
 *  Revision 1.4  1995/04/07 16:54:51  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.3  1995/04/04 10:37:26  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.2  1994/12/15 15:28:12  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.1  1994/12/15 15:18:40  Sven_Gothel
 *  Initial revision
 *
*/

# include "maschine.h"

static double maschEps()
// Berechnet die vom Betrag her kleinste Zahl des Datentyps 'double'
{
  double x = 1.0, res;
  do res = x;
  while (1.0 + (x /= 2) > 1.0);
  return res;
}

const double MASCHINE::epsilon = maschEps();
const double MASCHINE::toleranz = 10*MASCHINE::epsilon;


