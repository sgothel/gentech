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

# include <stdio.h>
# include "genparin.h"
# include "salesgen.h"
# include "sales.h"

const char SalesVersion[]="0.4";

int Menue (void) {
  int result;

  printf("\n Menue :\n");
  # ifdef __BORLANDC__
    printf("\n  1 - Erstellung einer Loesung ( Abbruch mit ESC-TASTE )");
  # else
    printf("\n  1 - Erstellung einer Loesung ");
  # endif
  printf("\n      Evt. mit einer gespeicherten Population fortfahren");
  printf("\n  ------------------------------------------------------");
  printf("\n  2 - Loesung durchspielen     ( beliebige Datei )");
  printf("\n  ------------------------------------------------------");
  printf("\n  0 - Ende");
  Input("\n\n Bitte waehlen Sie [1] : ",result,1);
  return result;
}

void main (void) {
  int choosen;
  GenParameter MyGenParameter;
  char FileName[80];
  NukleoTyp ptrSpliceCode[] = { 1,4,3,0,4,2,3,0 };

  # ifdef __BORLANDC__
      clrscr();
  # endif
  printf("\n\nSALESMAN's SOLUTION Version %s [*]\n", SalesVersion);
  # ifndef NDEBUG
    printf("\n geloest mittels Genetischer-Algorithmen  Version %s [*]. DEBUG-VERSION",
      GenTechVersion );
  # else
    printf("\n geloest mittels Genetischer-Algorithmen  Version %s [*].",
      GenTechVersion );
  # endif
  cout << "\n\nFuer 25 deutsche Staedte wird eine moeglichst kurze Rundstrecke ermittelt."
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
  cout << "\n<BITTE EINE TASTE DRUECKEN>";
  # ifdef __BORLANDC__
      getch();
  # else
      getchar();
  # endif
  do {
    choosen=Menue();
    switch (choosen) {
      case 1 : MyGenParameter.Input( SalesGame::CODE_MIN_VAL,
				     SalesGame::CODE_MAX_VAL,
				     ptrSpliceCode );
	       if ( MyGenParameter.FileName[0]==0 ) {
		 SalesChromosomen MySalesProblem( 0, 0,
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
		   strcpy(FileName,MyGenParameter.FileNameHeader);
		   strcat(FileName,".ptk");
		   MySalesProblem.Evolution ( 1, FileName,
					      MyGenParameter.BirthRate,
					      MyGenParameter.Bigamie
					    );
		   strcpy(FileName,MyGenParameter.FileNameHeader);
		   strcat(FileName,".pop");
		   MySalesProblem.Save(FileName);
		   strcpy(FileName,MyGenParameter.FileNameHeader);
		   strcat(FileName,".gen");
		   MySalesProblem.GetTheBestEverChromosom().Save(FileName);
	       } else {
		 SalesChromosomen MySalesProblem( 0, 0,
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
		   strcpy(FileName,MyGenParameter.FileNameHeader);
		   strcat(FileName,".ptk");
		   MySalesProblem.Evolution ( 1, FileName,
					      MyGenParameter.BirthRate,
					      MyGenParameter.Bigamie
					    );
		   strcpy(FileName,MyGenParameter.FileNameHeader);
		   strcat(FileName,".pop");
		   MySalesProblem.Save(FileName);
		   strcpy(FileName,MyGenParameter.FileNameHeader);
		   strcat(FileName,".gen");
		   MySalesProblem.GetTheBestEverChromosom().Save(FileName);
	       }
	       # ifdef __BORLANDC__
		   printf("\007\n\n Fertig. <BITTE EINE TASTE DRUECKEN>");
		   getch();
	       # else
		   printf("\007\n\n Fertig. <BITTE RETURN DRUECKEN>");
		   getchar();
	       # endif
	       break;
     case 2 :  strcpy(FileName,FileNameInput(READ_MODE,".gen"));
	       if (1) {
		   SalesChromosomen MySalesProblem( 0, 0,
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
		   SalesChromosom TheBest ( MySalesProblem,
				       FileName
				     );
		   MySalesProblem.TheGame.Play(TheBest,TRUE);
	       }
	       break;
    } // switch
  } while (choosen!=0);
  exit(0);
}
