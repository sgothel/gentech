// genparin.cpp                                                  7.3.1994
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

#include "sales_genparin.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

// Symbolische Konstanten fuer die Eingabe der Parameter
const int InputCrossVal=4;
const long InputMutationsFreq=111;
const int InputMaxChromosom=40;
const int InputMaxNoImprove=100;
const int InputChromStartLength=25;
const double InputBirthRate=0.8;
const long InputInversionFreq=243;
const long InputTranslocationFreq=71;
const long InputAsymXOverFreq=0;

template < class T>
void Input ( const std::string& question, T &dest, T def )
{
  std::string input;
  double buffer;

  printf("%s", question.c_str());
  std::getline(std::cin, input);
  if( input.length() == 0 ) {
      dest = def;
  } else {
    sscanf(input.c_str(),"%lf", &buffer);
    dest=(T) buffer;
  }
}

// explicit template function instantiation
template void Input ( const std::string& question, int &dest, int def );

std::string stdin_input(const std::string& msg, const std::string& default_value, const std::vector<std::string_view>& ok_values) {
    std::string input;
    bool ok = false;
    do {
      printf("%s [%s] : ", msg.c_str(), default_value.c_str());
      std::getline(std::cin, input);
      if( input.length() == 0 ) {
          return default_value;
      }
      ok = ok_values.end() != std::find(ok_values.begin(), ok_values.end(), input);
    } while ( !ok );
    return input;
}


std::string FileNameInput( int mode, const std::string& end )
{
  std::string FileName;
  FILE *test=nullptr;

  do {
      do {
          if( !end.empty() ) {
              printf("\n Dateiname (Automatische Dateiendung: %s) : ", end.c_str());
          } else {
              printf("\n Dateiname (Automatische Dateiendungen: .ptk .pop .gen ) : ");
          }
          std::getline(std::cin, FileName);
          if( FileName.length() < 5 || FileName[FileName.length()-4]!='.' ) {
              if( !end.empty() ) {
                  FileName.append(end);
              } else {
                  FileName.append(".nop");
              }
          }
      } while ( FileName.length() < 5 || FileName[FileName.length()-4]!='.' );
      if( !end.empty() ) {
          if(mode==READ_MODE) {
              test=fopen(FileName.c_str(),"rb");
          } else if(mode==WRITE_MODE) {
              test=fopen(FileName.c_str(),"wb");
          } else {
              INT_ERR(__LINE__);
          }
      }
  } while( test==nullptr && !end.empty() );
  if(test!=nullptr) fclose(test);
  return FileName;
}

void GenParameter::Input( int NukleoMinVal, int NukleoMaxVal,
			  NukleoTyp *ptrSpliceCode
			)
{
    (void) ptrSpliceCode;

    char input[80];

    GenParameter::UserNukleoMinVal=NukleoMinVal;
    GenParameter::UserNukleoMaxVal=NukleoMaxVal;

    printf("\n\n Parameter-Eingabe fuer den Genetischen Algorithmus :");
    printf("\n { Der [DefaultWert] ist mit Return einzugeben ! }\n");

    printf("\n Bitte geben Sie den Namen der Ziel-Dateien ( *.ptk, *.pop, *.gen )");
    printf("\n ohne Dateiendung an.");
    FileNameHeader = FileNameInput(WRITE_MODE,"");
    FileNameHeader.resize(FileNameHeader.length() - 4);

    sprintf( input,"\n Maximale Populationsgroesse [%d]:  ",InputMaxChromosom);
    ::Input (input, MaxChromosomen, (int)InputMaxChromosom);

    std::string reply = stdin_input("\n Start-Population from file (y/n) ?", "n", { "y", "n" } );
    if(reply[0]=='y') {
        FileName = FileNameInput(READ_MODE,".pop");
    } else {
        FileName.clear();
        sprintf( input," Start-Chromosomen Anzahl [%d] : ", MaxChromosomen );
        ::Input (input, StartChromosomNumber, MaxChromosomen);
        //    sprintf( input," Start-Chromosomen Laenge [%d] (Minimum ist 2) :  ", InputChromStartLength);
        //    ::Input (input, StartChromosomLength, InputChromStartLength);
        StartChromosomLength = InputChromStartLength;
    }
    do {
        sprintf( input," Geburtenrate [%0.3lf] (0 < x <= 1): ", InputBirthRate);
        ::Input (input, BirthRate, (double)InputBirthRate);
    } while ( BirthRate <= 0 || BirthRate > 1 );
    reply = stdin_input("\n Bigamistic relationships allowed (y/n) ?", "n", { "y", "n" } );
    if(reply[0]=='y') Bigamie=1;
    else Bigamie=0;

    /*
  do {
    sprintf( input," Nukleotid-Anzahl [%d] ( 0 <= x <= %d ) : ",
	     NukleoMaxVal-NukleoMinVal+1, NukleoMaxVal-NukleoMinVal+1  );
    ::Input (input, Nukleotide, NukleoMaxVal-NukleoMinVal+1 );
  } while ( Nukleotide<0 || Nukleotide>NukleoMaxVal-NukleoMinVal+1 );
     */
    Nukleotide = NukleoMaxVal-NukleoMinVal+1;
    sprintf( input," Crossing-Over Punkte [%d]:  ", InputCrossVal);
    ::Input (input, CrossVal, InputCrossVal);
    sprintf( input," Mutation-Frequenz [%ld]:  ", InputMutationsFreq);
    ::Input (input, MutationFreq, InputMutationsFreq );
    sprintf( input," Inversions-Frequenz [%ld]:  ", InputInversionFreq);
    ::Input (input, InversionFreq, InputInversionFreq);
    sprintf( input," Translocations-Frequenz [%ld]:  ", InputTranslocationFreq);
    ::Input (input, TranslocationFreq, InputTranslocationFreq);
    //    sprintf( input," AsymXOverFreq-Frequenz [%d]:  ", InputAsymXOverFreq);
    //  ::Input (input, AsymXOverFreq, (long)InputAsymXOverFreq);
    AsymXOverFreq = InputAsymXOverFreq;
    sprintf (input," Abbruch nach Anzahl Generationen ohne Verbesserung [%d]:  ", InputMaxNoImprove);
    ::Input (input, MaxNoImprove, InputMaxNoImprove);
    TheSpliceCodeInfo.Length=0;
    TheSpliceCodeInfo.SpliceCode=0;

}
