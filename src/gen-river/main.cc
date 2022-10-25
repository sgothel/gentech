// main.cpp
//
// Diese Modul wurde fuer eine Hausarbeit im Fach
// Objektorietierter Programierung (OOP) an der FH Bielefeld
// unter der Betreuung von Prof. Dr. Bunse erstellt.
//
// Autoren der Hausarbeit : Sven Goethel und Christian Mueller
//
// Jegliches Copyright aller Dateien ist im Besitz der Autoren.
// Sven Goethel * http://www.jausoft.com - mailto:info@jausoft.com
// Bielefeld, den 11.3.1994.

# include <stdio.h>
# include "genparin.h"
# include "rivergen.h"
# include "river.h"

int Menue (void) {
  int result;

  printf("\n\n Das Kannibalen/Missionare - Flussproblem V 2.40 [*],");
  # ifndef NDEBUG
    printf("\n geloest mittels Genetischer-Algorithmen  V 2.40 [*]. DEBUG-VERSION");
  # else
    printf("\n geloest mittels Genetischer-Algorithmen  [*].");
  # endif
  printf("\n\n Eine Hausarbeit im Fach Objektorientierter Programierung (OOP)");
  printf("\n an der FH Bielefeld unter der Betreuung von Prof. Dr. Bunse.");
  printf("\n\n Autoren : Sven Goethel und Christian Mueller\n");
  printf("\n [*] Dieses Ausfuehrbare File ist FREEWARE.");
  printf("\n     Jegliches Copyright aller Dateien ist im Besitz der Autoren.");
  printf("\n     Sven Goethel * http://www.jausoft.com - mailto:info@jausoft.com");
  printf("\n     Bielefeld, den 11.3.1994.\n");
  printf("\n     Bielefeld, den 08.3.1998 (last update).\n");
  printf("\n Menue :\n");
  #if ( defined __MSDOS__ || defined __BORLANDC__ )
    printf("\n  1 - Erstellung einer Loesung ( Abbruch mit ESC-TASTE )");
  #else
    printf("\n  1 - Erstellung einer Loesung ");
  #endif
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

void main (void) {
  int choosen;
  GenParameter MyGenParameter;
  int Monk, Cannibal;
  char FileName[80];
  NukleoTyp ptrSpliceCode[] = { 1,4,3,0,4,2,3,0 };

  do {
    choosen=Menue();
    if(choosen==1) {
       MyGenParameter.Input( RiverGame::CODE_MIN_VAL,
       RiverGame::CODE_MAX_VAL,
       ptrSpliceCode );
       Input("\nMissionare [8] : ", Monk, 8 );
       Input("\nKannibalen [4] : ", Cannibal, 4 );
       printf("\n\n");
       if ( MyGenParameter.FileName[0]==0 ) {
         RiverProblem MyRiverProblem( Monk, Cannibal,
		 MyGenParameter.MaxChromosomen,
		 MyGenParameter.StartChromosomNumber,
		 MyGenParameter.StartChromosomLength,
		 MyGenParameter.Nukleotide,
		 &MyGenParameter.TheSpliceCodeInfo,
		 MyGenParameter.InversionFreq,
		 MyGenParameter.TranslocationFreq,
		 MyGenParameter.AsymXOverFreq,
		 MyGenParameter.CrossVal,
		 MyGenParameter.MutationFreq
         ) ;
         strcpy(FileName,MyGenParameter.FileNameHeader);
         strcat(FileName,".ptk");
         MyRiverProblem.Evolution ( 1, FileName,
                MyGenParameter.BirthRate,
                MyGenParameter.Bigamie,
                MyGenParameter.MaxNoImprove
              );
         strcpy(FileName,MyGenParameter.FileNameHeader);
         strcat(FileName,".pop");
         MyRiverProblem.Save(FileName);
         strcpy(FileName,MyGenParameter.FileNameHeader);
         strcat(FileName,".gen");
         MyRiverProblem.GetTheBestEverChromosom().Save(FileName);
       } else {
         RiverProblem MyRiverProblem( Monk, Cannibal,
           MyGenParameter.MaxChromosomen,
           MyGenParameter.FileName,
           MyGenParameter.Nukleotide,
           &MyGenParameter.TheSpliceCodeInfo,
           MyGenParameter.InversionFreq,
           MyGenParameter.TranslocationFreq,
           MyGenParameter.AsymXOverFreq,
           MyGenParameter.CrossVal,
           MyGenParameter.MutationFreq
         ) ;
         strcpy(FileName,MyGenParameter.FileNameHeader);
         strcat(FileName,".ptk");
         MyRiverProblem.Evolution ( 1, FileName,
                  MyGenParameter.BirthRate,
                  MyGenParameter.Bigamie,
                  MyGenParameter.MaxNoImprove
                );
         strcpy(FileName,MyGenParameter.FileNameHeader);
         strcat(FileName,".pop");
         MyRiverProblem.Save(FileName);
         strcpy(FileName,MyGenParameter.FileNameHeader);
         strcat(FileName,".gen");
         MyRiverProblem.GetTheBestEverChromosom().Save(FileName);
       }
       printf("\007\n\n Fertig. <BITTE RETURN DRUECKEN>");
       getchar();
     } else if(choosen==2) {
         strcpy(FileName,FileNameInput(READ_MODE,".gen"));
         Input("\nMissionare [8] : ", Monk, 8 );
         Input("\nKannibalen [4] : ", Cannibal, 4 );
         printf("\n\n");
         RiverProblem dummyProb( Monk, Cannibal,
		 10000 /* MyGenParameter.MaxChromosomen */,
		 0 /* MyGenParameter.StartChromosomNumber */,
		 0 /* MyGenParameter.StartChromosomLength */,
		 MyGenParameter.Nukleotide,
		 &MyGenParameter.TheSpliceCodeInfo,
		 MyGenParameter.InversionFreq,
		 MyGenParameter.TranslocationFreq,
		 MyGenParameter.AsymXOverFreq,
		 MyGenParameter.CrossVal,
		 MyGenParameter.MutationFreq
         ) ;
         Chromosom TheBest ( dummyProb, FileName);
         dummyProb.TheGame.Play(TheBest,TRUE);
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
  exit(0);
}
