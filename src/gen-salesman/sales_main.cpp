// main.cpp
//
// Diese Modul wurde fuer eine Hausarbeit im Fach
// Objektorietierter Programierung (OOP) an der FH Bielefeld
// unter der Betreuung von Prof. Dr. Bunse erstellt.
//
// Autoren der Hausarbeit : Sven Goethel und Christian Mueller
//
// Jegliches Copyright aller Dateien ist im Besitz der Autoren.
// Sven Goethel * Stapenhorststr.35a * 33615 Bielefeld * 0521/139228
// Bielefeld, den 11.3.1994.

#include <cstdio>
#include <iostream>

#include "sales_genparin.hpp"
#include "sales_gen.hpp"
#include "sales.hpp"

const char SalesVersion[]="0.4";

int Menue() {
  int result;

  printf("\n Menue :\n");
  printf("\n  1 - Erstellung einer Loesung ");
  printf("\n      Evt. mit einer gespeicherten Population fortfahren");
  printf("\n  ------------------------------------------------------");
  printf("\n  2 - Loesung durchspielen     ( beliebige Datei )");
  printf("\n  ------------------------------------------------------");
  printf("\n  0 - Ende");
  Input("\n\n Bitte waehlen Sie [1] : ",result,1);
  return result;
}

int main (int argc, const char* argv[]) {
  (void)argc;
  (void)argv;

  int choosen;
  GenParameter MyGenParameter;
  NukleoTyp ptrSpliceCode[] = { 1,4,3,0,4,2,3,0 };

#ifndef NDEBUG
  printf("\n\nSALESMAN's SOLUTION Version %s [*]\n", SalesVersion);
#else
  printf("\n\nSALESMAN's SOLUTION Version %s [*] (debug version)\n", SalesVersion);
#endif
  printf("\n (utilizing a genetic-algorithm, version %s)", GenTechVersion);
  std::cout << "\n\nFuer 25 deutsche Staedte wird eine moeglichst kurze Rundstrecke ermittelt."
       << "\nDabei sind sowohl der Startpunkt der Rundreise, als auch die Drehrichtung "
       << "\nvariabel."
       << "\nDie bis jetzt gefundene kuerzeste Rundreise hat eine Laenge von 3332 Km"
       << "\nund fuehrt in folgender Reihenfoge durch die 25 Staedte:"
       << "\nBerlin-Dresden-Leipzig-Nuernberg-Muenchen-Stuttgart-Freiburg/Breisgau-"
       << "\nKarlsruhe-Saarbruecken-Wiesbaden-Frankfurt/Main-Koeln-Aachen-Duesseldorf-"
       << "\nWuppertal-Essen-Dortmund-Kassel-Bielefeld-Bremen-Hamburg-Kiel-Hannover-"
       << "\nBraunschweig-Magdeburg-Berlin";
  printf("\n\n Autoren : Sven Goethel und Christian Mueller\n");
  printf("\n [*] Dieses ausfuehrbare File ist FREEWARE.");
  printf("\n     Jegliches Copyright aller Dateien ist im Besitz der Autoren.");
  printf("\n     Sven Goethel *  www.jausoft.com ");
  printf("\n     Christian Mueller");
  printf("\n     Bielefeld, den 28.05.1994.\n");
  printf("\n     ");
  printf("\n     Bielefeld, den 23.03.1998 (last changes).\n");
  std::cout << "\n<BITTE EINE TASTE DRUECKEN>";
  getchar();
  do {
      choosen=Menue();
      switch (choosen) {
          case 1:
              MyGenParameter.Input( SalesGame::CODE_MIN_VAL, SalesGame::CODE_MAX_VAL, ptrSpliceCode );
              if ( MyGenParameter.FileName[0]==0 ) {
                  SalesChromosomen MySalesProblem(
                          MyGenParameter.MaxChromosomen,
                          MyGenParameter.StartChromosomNumber,
                          MyGenParameter.StartChromosomLength,
                          MyGenParameter.Nukleotide,
                          0,
                          MyGenParameter.InversionFreq,
                          MyGenParameter.TranslocationFreq,
                          MyGenParameter.AsymXOverFreq,
                          MyGenParameter.CrossVal,
                          MyGenParameter.MutationFreq,
                          MyGenParameter.MaxNoImprove
                  ) ;
                  MySalesProblem.Evolution ( 1, MyGenParameter.FileNameHeader + ".ptk",
                          MyGenParameter.BirthRate,
                          MyGenParameter.Bigamie
                  );
                  MySalesProblem.Save( MyGenParameter.FileNameHeader + ".pop" );
                  MySalesProblem.GetTheBestEverChromosom().Save( MyGenParameter.FileNameHeader + ".gen" );
              } else {
                  SalesChromosomen MySalesProblem(
                          MyGenParameter.MaxChromosomen,
                          MyGenParameter.FileName,
                          MyGenParameter.Nukleotide,
                          0,
                          MyGenParameter.InversionFreq,
                          MyGenParameter.TranslocationFreq,
                          MyGenParameter.AsymXOverFreq,
                          MyGenParameter.CrossVal,
                          MyGenParameter.MutationFreq,
                          MyGenParameter.MaxNoImprove
                  ) ;
                  MySalesProblem.Evolution ( 1, MyGenParameter.FileNameHeader + ".ptk",
                          MyGenParameter.BirthRate,
                          MyGenParameter.Bigamie
                  );
                  MySalesProblem.Save( MyGenParameter.FileNameHeader + ".pop" );
                  MySalesProblem.GetTheBestEverChromosom().Save( MyGenParameter.FileNameHeader + ".gen" );
              }
              printf("\007\n\n Fertig. <BITTE RETURN DRUECKEN>");
              getchar();
              break;
          case 2:
              std::string FileName = FileNameInput(READ_MODE,".gen");
              SalesChromosomen MySalesProblem(
                      MyGenParameter.MaxChromosomen,
                      MyGenParameter.StartChromosomNumber,
                      MyGenParameter.StartChromosomLength,
                      MyGenParameter.Nukleotide,
                      0,
                      MyGenParameter.InversionFreq,
                      MyGenParameter.TranslocationFreq,
                      MyGenParameter.AsymXOverFreq,
                      MyGenParameter.CrossVal,
                      MyGenParameter.MutationFreq,
                      MyGenParameter.MaxNoImprove
              ) ;
              SalesChromosom TheBest ( MySalesProblem, FileName );
              MySalesProblem.TheGame.Play(TheBest, true);
              break;
      } // switch
  } while (choosen!=0);
  return 0;
}
