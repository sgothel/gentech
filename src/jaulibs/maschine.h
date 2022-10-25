// author : Sven Gothel - Stapenhorststrasse 35a - 33615 Bielefeld
//
// maschine.h    
//
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
//
// JETZT EINE KLASSE !!!!
/*
 *
 *  $Log: maschine.h $
 *  Revision 1.4  1995/04/07 16:54:52  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.3  1995/04/04 10:37:26  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.2  1994/12/15 15:28:14  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.1  1994/12/15 15:18:41  Sven_Gothel
 *  Initial revision
 *
*/

# ifndef _MYMASCHINE
  # define _MYMASCHINE

  // Enthaelt die vom Betrag her kleinste Zahl des Datentyps 'double'
  class MASCHINE {
  public:
	static const double toleranz;
	static const double epsilon;
  };

# endif
