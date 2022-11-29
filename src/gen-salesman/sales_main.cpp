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

#include "sales_genparin.hpp"
#include "sales_gen.hpp"
#include "sales.hpp"

using namespace jau::gentech;

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
  nucleotide_t ptrSpliceCode[] = { 1,4,3,0,4,2,3,0 };

#ifndef NDEBUG
  printf("\n\nTraveller Salesman Problem [*]\n");
#else
  printf("\n\nTraveller Salesman Problem [*] (debug version)\n");
#endif
  printf("\n (Gentech version %s)", jau::gentech::VERSION);
  printf("\n");
  std::cout << "\n<BITTE EINE TASTE DRUECKEN>";
  getchar();
  do {
      choosen=Menue();
      switch (choosen) {
          case 1:
              MyGenParameter.Input( SalesGame::CODE_MIN_VAL, SalesGame::CODE_MAX_VAL, ptrSpliceCode );
              if ( MyGenParameter.FileName[0]==0 ) {
                  SalesChromosomen MySalesProblem(
                          MyGenParameter.max_chromosom_count,
                          MyGenParameter.init_chromosom_count,
                          MyGenParameter.init_chromosom_len,
                          MyGenParameter.nucleotide_count,
                          nullptr,
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
                          MyGenParameter.max_chromosom_count,
                          MyGenParameter.FileName,
                          MyGenParameter.nucleotide_count,
                          nullptr,
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
                      MyGenParameter.max_chromosom_count,
                      MyGenParameter.init_chromosom_count,
                      MyGenParameter.init_chromosom_len,
                      MyGenParameter.nucleotide_count,
                      nullptr,
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
