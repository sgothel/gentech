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

#ifndef SALES_GENPARIN_H
  #define SALES_GENPARIN_H

  #include <string>

  #include "gentech.h"

  // external template function definition in .cpp
  template <class T>
  extern void Input ( const std::string& question, T &dest, T def );

  enum { WRITE_MODE, READ_MODE };

  std::string FileNameInput ( int mode, const std::string& end );

  class GenParameter {
  public:
      void Input( int NukleoMinVal, int NukleoMaxVal,
		  NukleoTyp *ptrSpliceCode=NULL );
      std::string FileName;
      std::string FileNameHeader;
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
