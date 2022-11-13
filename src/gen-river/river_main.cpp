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

#include <cstdio>
#include <iostream>

#include "river_genparin.hpp"
#include "river_gen.hpp"
#include "river.hpp"

using namespace jau::gentech;

int Menue() {
  int result;

#ifndef NDEBUG
  printf("\n\n River Problem (Cannibal/Monk) [*]");
#else
  printf("\n\n River Problem (Cannibal/Monk) [*] (debug version)");
#endif
  printf("\n (Gentech version %s)", jau::gentech::VERSION);
  printf("\n");
  printf("\n Menue :\n");
  printf("\n  1 - Erstellung einer Loesung ");
  printf("\n      Evt. mit einer gespeicherten Population fortfahren");
  printf("\n  ------------------------------------------------------");
  printf("\n  2 - Loesung durchspielen     ( beliebige Datei )");
  printf("\n  ------------------------------------------------------");
  printf("\n  3 - Informationen");
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
  int Monk, Cannibal;
  nucleotide_t ptrSpliceCode[] = { 1,4,3,0,4,2,3,0 };

  do {
      choosen=Menue();
      if(choosen==1) {
          MyGenParameter.Input( RiverGame::CODE_MIN_VAL, RiverGame::CODE_MAX_VAL, ptrSpliceCode );
          Input("\nMissionare [8] : ", Monk, 8 );
          Input("\nKannibalen [4] : ", Cannibal, 4 );
          printf("\n\n");
          RiverProblem* MyRiverProblem;
          if ( MyGenParameter.FileName.empty() ) {
              MyRiverProblem = new RiverProblem( Monk, Cannibal,
                      MyGenParameter.max_chromosom_count,
                      MyGenParameter.init_chromosom_count,
                      MyGenParameter.init_chromosom_len,
                      MyGenParameter.nucleotide_count,
                      &MyGenParameter.TheSpliceCodeInfo,
                      MyGenParameter.InversionFreq,
                      MyGenParameter.TranslocationFreq,
                      MyGenParameter.AsymXOverFreq,
                      MyGenParameter.CrossVal,
                      MyGenParameter.MutationFreq,
                      MyGenParameter.MaxNoImprove
              ) ;
          } else {
              MyRiverProblem = new RiverProblem( Monk, Cannibal,
                      MyGenParameter.max_chromosom_count,
                      MyGenParameter.FileName,
                      MyGenParameter.nucleotide_count,
                      &MyGenParameter.TheSpliceCodeInfo,
                      MyGenParameter.InversionFreq,
                      MyGenParameter.TranslocationFreq,
                      MyGenParameter.AsymXOverFreq,
                      MyGenParameter.CrossVal,
                      MyGenParameter.MutationFreq,
                      MyGenParameter.MaxNoImprove
              ) ;
          }
          MyRiverProblem->Evolution ( 1, MyGenParameter.FileNameHeader + ".ptk",
                  MyGenParameter.BirthRate,
                  MyGenParameter.Bigamie
          );
          MyRiverProblem->Save(MyGenParameter.FileNameHeader + ".pop");
          const Chromosom& theBest = MyRiverProblem->GetTheBestEverChromosom();
          theBest.Save(MyGenParameter.FileNameHeader + ".gen");
          {
              std::string fname = MyGenParameter.FileNameHeader + ".gen";
              Chromosom test( *MyRiverProblem, fname);
              if( theBest != test ) {
                  printf("Error saving '%s'.\nTheBest:\n", fname.c_str());
                  theBest.Ausgabe(std::cout);
                  printf("\nReadBack:\n");
                  test.Ausgabe(std::cout);
                  printf("\n");
              } else {
                  printf("Validated '%s'.\n", fname.c_str());
                  theBest.Ausgabe(std::cout);
                  printf("\n");
              }
          }
          MyRiverProblem->TheGame.Play(theBest, true);
          delete MyRiverProblem;
          printf("\007\n\n Fertig. <BITTE RETURN DRUECKEN>");
          getchar();
      } else if(choosen==2) {
          std::string FileName = FileNameInput(READ_MODE,".gen");
          Input("\nMissionare [8] : ", Monk, 8 );
          Input("\nKannibalen [4] : ", Cannibal, 4 );
          printf("\n\n");
          RiverProblem dummyProb( Monk, Cannibal,
                  10000 /* MyGenParameter.MaxChromosomen */,
                  0 /* MyGenParameter.StartChromosomNumber */,
                  0 /* MyGenParameter.StartChromosomLength */,
                  MyGenParameter.nucleotide_count,
                  &MyGenParameter.TheSpliceCodeInfo,
                  MyGenParameter.InversionFreq,
                  MyGenParameter.TranslocationFreq,
                  MyGenParameter.AsymXOverFreq,
                  MyGenParameter.CrossVal,
                  MyGenParameter.MutationFreq
          ) ;
          Chromosom TheBest ( dummyProb, FileName);
          dummyProb.TheGame.Play(TheBest, true);
      } else if(choosen==3) {
          printf("\n\n\n\n Informationen :");
          printf("\n\n Das Flussproblem :\n");
          printf("\n m Missionare und k Kannibalen befinden sich am Ufer eines Flusses.");
          printf("\n Es gibt nur ein Boot mit zwei Plaetzen zum Uebersetzen.");
          printf("\n Zu keiner Zeit duerfen an einem Ufer des Flusses mehr");
          printf("\n Kannibalen als Missionare sein ( ausser an einem Ufer ist kein Missionar ).");
          printf("\n Das Boot kann nicht leer uebersetzen ( wohl aber mit nur einer Person ).");
          printf("\n Ab vier Kannibalen am Zielufer mehr,\n wird der verursachende Zug als falsch gewertet.");
          printf("\n Hier gaebe es keine Moeglichkeit mehr Missionare ans Zielufer zu uebersetzen.\n");
          printf("\n Nach Beendigung oder Abbruch der Erstellung einer Loesung [1]");
          printf("\n werden die Ergebnisse in folgenden Dateien abgespeicher :");
          printf("\n   (ausgewaehlter Dateiname).pop : Die Endpopulation");
          printf("\n   (ausgewaehlter Dateiname).gen : Das beste Chromosom (evt. die Loesung)");
          printf("\n   (ausgewaehlter Dateiname).ptk : Das Protokoll des genetischen Algorithmus\n");
          printf("\n Die Erlauterung der Arbeitsweise des genetischen Algorithmus");
          printf("\n ist in der Datei 'GENTECH.DOC' zu finden.\n");
          printf("\n < WEITER BITTE RETURN DRUECKEN >");
          getchar();
      }
  } while (choosen!=0);
  return 0;
}
