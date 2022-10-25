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

# include "genparin.h"
# include "maschine.h"

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
void Input ( char *question, T &dest, T def )
{
  char input[80];
  double buffer;

  printf("%s", question);
  gets(input);
  if( strlen(input) == 0 ) dest=def;
  else {
    sscanf(input,"%lf", &buffer);
    dest=(T)buffer;
  }
}

const char *FileNameInput ( int mode, char *end )
{
  static char FileName[80];
  static char Name[] = "Test";
  FILE *test=NULL;

  do {
      do {
	if(end!=NULL)
	  printf("\n Dateiname (Automatische Dateiendung: %s) [%s]: ",
				end, Name);
	else
	  printf("\n Dateiname (Automatische Dateiendungen: .ptk .pop .gen ) [%s]: ",
		  Name );
	gets(FileName);
	if (FileName[0] == 0) strcpy (FileName, Name);
	  if(FileName[strlen(FileName)-4]!='.') {
	    if(end!=NULL) strcat(FileName,end);
	    else strcat(FileName,".nop");
	}
      } while ( FileName[strlen(FileName)-4]!='.' );
    if(end!=NULL) {
      if(mode==READ_MODE)
	test=fopen(FileName,"rb");
      else if(mode==WRITE_MODE)
	test=fopen(FileName,"wb");
      else INT_ERR(__LINE__);
    }
  } while(test==NULL&&end!=NULL);
  if(test!=NULL) fclose(test);
  return &FileName[0];
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
  strcpy(FileNameHeader,FileNameInput(WRITE_MODE,NULL));
  FileNameHeader[strlen(FileNameHeader)-4]=0;

    sprintf( input,"\n Maximale Populationsgroesse [%d]:  ",InputMaxChromosom);
  ::Input (input, MaxChromosomen, (int)InputMaxChromosom);

  do {
    printf(" Start-Population aus Datei (j/n) ? [n] : ");
    gets(input);
    if(input[0]==0) strcpy(input,"n");
  } while (strcmp(input,"j")!=0&&strcmp(input,"n")!=0);
  if(input[0]=='j') {
    strcpy(FileName,FileNameInput(READ_MODE,".pop"));
  } else {
    FileName[0]=0;
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
  do {
    printf(" Bigamistische Beziehungen erlauben (j/n) ? [n] : ");
    gets(input);
    if(input[0]==0) strcpy(input,"n");
  } while (strcmp(input,"j")!=0&&strcmp(input,"n")!=0);
  if(input[0]=='j') Bigamie=1;
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
    sprintf( input," Mutation-Frequenz [%d]:  ", InputMutationsFreq);
  ::Input (input, MutationFreq, (long)InputMutationsFreq );
    sprintf( input," Inversions-Frequenz [%d]:  ", InputInversionFreq);
  ::Input (input, InversionFreq, (long)InputInversionFreq);
    sprintf( input," Translocations-Frequenz [%d]:  ", InputTranslocationFreq);
  ::Input (input, TranslocationFreq, (long)InputTranslocationFreq);
//    sprintf( input," AsymXOverFreq-Frequenz [%d]:  ", InputAsymXOverFreq);
//  ::Input (input, AsymXOverFreq, (long)InputAsymXOverFreq);
    AsymXOverFreq = InputAsymXOverFreq;
    sprintf (input," Abbruch nach Anzahl Generationen ohne Verbesserung [%d]:  ", InputMaxNoImprove);
  ::Input (input, MaxNoImprove, InputMaxNoImprove);
  TheSpliceCodeInfo.Length=0;
  TheSpliceCodeInfo.SpliceCode=0;

}
