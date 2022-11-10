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
// Sven Goethel * http://www.jausoft.com - mailto:info@jausoft.com
// Bielefeld, den 11.3.1994.

#include "river_genparin.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

// Symbolische Konstanten fuer die Eingabe der Parameter
const int InputCrossVal=2;
const long InputMutationsFreq=811;
const int InputMaxChromosom=100;
const int InputMaxNoImprove=10000;
const int InputChromStartLength=50;
const double InputBirthRate=0.66;
const long InputInversionFreq=13;
const long InputTranslocationFreq=17;
const long InputAsymXOverFreq=473;

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
    char input[80];

    GenParameter::UserNukleoMinVal=NukleoMinVal;
    GenParameter::UserNukleoMaxVal=NukleoMaxVal;

    printf("\n\n Parameter-Eingabe fuer den Genetischen Algorithmus :");
    printf("\n { Der [DefaultWert] ist mit Return einzugeben ! }\n");

    printf("\n Bitte geben Sie den Namen der Ziel-Dateien ( *.ptk, *.pop, *.gen )");
    printf("\n ohne Dateiendung an.");
    FileNameHeader = FileNameInput(WRITE_MODE, "");
    FileNameHeader.resize(FileNameHeader.length() - 4);

    sprintf( input,"\n Maximale Populationsgroesse [%d]:  ",InputMaxChromosom);
    ::Input (input, MaxChromosomen, (int)InputMaxChromosom);

    std::string reply = stdin_input("\n Start-Population from file (y/n) ?", "n", { "y", "n" } );
    if(reply[0]=='y') {
        FileName = FileNameInput(READ_MODE,".pop");
    } else {
        FileName.clear();
        sprintf( input,"\n Start-Chromosomen Anzahl [%d] : ",
                MaxChromosomen );
        ::Input (input, StartChromosomNumber, MaxChromosomen);
        sprintf( input,"\n Start-Chromosomen Laenge [%d] (Minimum ist 2) :  ", InputChromStartLength);
        ::Input (input, StartChromosomLength, InputChromStartLength);
        if (StartChromosomLength < 2) StartChromosomLength=2;
    }
    do {
        sprintf( input,"\n Geburtenrate [%0.3lf] (0 < x <= 1): ", InputBirthRate);
        ::Input (input, BirthRate, (double)InputBirthRate);
    } while ( BirthRate <= 0 || BirthRate > 1 );
    reply = stdin_input("\n Bigamistic relationships allowed (y/n) ?", "n", { "y", "n" } );
    if(reply[0]=='y') Bigamie=1;
    else Bigamie=0;

    do {
        sprintf( input,"\n Nukleotid-Anzahl [%d] ( 0 <= x <= %d ) : ",
                NukleoMaxVal-NukleoMinVal+1, NukleoMaxVal-NukleoMinVal+1  );
        ::Input (input, Nukleotide, NukleoMaxVal-NukleoMinVal+1 );
    } while ( Nukleotide<0 || Nukleotide>NukleoMaxVal-NukleoMinVal+1 );
    sprintf( input,"\n Crossing-Over Punkte [%d]:  ", InputCrossVal);
    ::Input (input, CrossVal, InputCrossVal);
    sprintf( input,"\n Mutation-Frequenz [%ld]:  ", InputMutationsFreq);
    ::Input (input, MutationFreq, InputMutationsFreq );
    sprintf( input,"\n Inversions-Frequenz [%ld]:  ", InputInversionFreq);
    ::Input (input, InversionFreq, InputInversionFreq);
    sprintf( input,"\n Translocations-Frequenz [%ld]:  ", InputTranslocationFreq);
    ::Input (input, TranslocationFreq, InputTranslocationFreq);
    sprintf( input,"\n AsymXOverFreq-Frequenz [%ld]:  ", InputAsymXOverFreq);
    ::Input (input, AsymXOverFreq, InputAsymXOverFreq);
    sprintf (input,"\n Abbruch nach Anzahl Generationen ohne Verbesserung [%d]:  ", InputMaxNoImprove);
    ::Input (input, MaxNoImprove, InputMaxNoImprove);
    reply = stdin_input("\n Edit splice code (y/n) ?", "n", { "y", "n" } );
    if(reply[0]=='y') {
        int StopSig=0;
        int j=0;
        int i=0;
        do {
            if( StopSig==0 && i==0 )
                printf("\n Intron Start-Sequenz :\n");
            if( StopSig==1 && TheSpliceCode[i-1]==0 && i>0 )
                printf("\n Intron End-Sequenz   :\n");
            do {
                if(ptrSpliceCode!=nullptr) {
                    printf(" %2d.Nukleotid [%2d] : ", i-j+1, ptrSpliceCode[i] );
                    fgets(input, sizeof(input), stdin);
                    if(input[0]==0) sprintf(input,"%d",ptrSpliceCode[i]);
                } else {
                    printf(" %2d.Nukleotid [0] : ", i-j+1 );
                    fgets(input, sizeof(input), stdin);
                }
            } while ( atoi ( input ) < 0 || atoi ( input ) > Nukleotide );
            if( ( TheSpliceCode[i++] = atoi(input) ) == 0 ) {
                StopSig++;
                j=i;
            }
        } while(StopSig<2);
    } else {
        if ( ptrSpliceCode!=nullptr ) {
            int StopSig=0;
            int i=0;
            printf("\n Intron Start-Sequenz : ");
            while(ptrSpliceCode[i]!=0) printf(" %2d", ptrSpliceCode[i++]);
            printf("\n Intron End-Sequenz   : ");
            while(ptrSpliceCode[++i]!=0) printf(" %2d", ptrSpliceCode[i]);
            // SpliceCode Kopieren ...
            for(i=0; StopSig<2; i++) {
                TheSpliceCode[i]=ptrSpliceCode[i];
                if(TheSpliceCode[i]==0) StopSig++;
            }
        } else printf("\n\n Kein SpliceCode vorhanden .");
        printf("\n\n Bitte Return druecken !");
        getchar();
    }
    TheSpliceCodeInfo.Length=0;
    TheSpliceCodeInfo.SpliceCode=TheSpliceCode;
}
