// genparin.h                                                    7.3.1994
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

# ifndef _GENPARIN_H
  # define _GENPARIN_H
  # include <stdio.h>
  # include <string.h>
  # include "gentech.h"

  template <class T>
  extern void Input ( char *question, T &dest, T def );
  enum { WRITE_MODE, READ_MODE };
  const char *FileNameInput ( int mode, char *end );

  class GenParameter {
  public:
      void Input( int NukleoMinVal, int NukleoMaxVal,
		  NukleoTyp *ptrSpliceCode=NULL );
      char FileName[80];
      char FileNameHeader[80];
      SpliceCodeInfo TheSpliceCodeInfo;
      double BirthRate;
      int Bigamie;
      int UserNukleoMinVal;
      int UserNukleoMaxVal;
      int MaxChromosomen;
      int StartChromosomNumber;
      int StartChromosomLength;
      int Nukleotide;
      long InversionFreq;
      long TranslocationFreq;
      long AsymXOverFreq;
      int CrossVal;
      long MutationFreq;
      int MaxNoImprove;
  private:
      NukleoTyp TheSpliceCode[100];
  };
# endif
