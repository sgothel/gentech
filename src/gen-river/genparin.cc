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

# include "genparin.h"
# include "maschine.h"

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
void Input ( char *question, T &dest, T def )
{
  char input[80];
  double buffer;

  printf("%s", question);
  gets(input);
  if( strlen(input) == 0 ) dest=def;
  else {
    sscanf(input,"%lf", &buffer);
    dest=(T) buffer;
  }
}

const char *FileNameInput ( int mode, char *end )
{
  static char FileName[80];
  FILE *test=NULL;

  do {
      do {
	if(end!=NULL)
	  printf("\n Dateiname (Automatische Dateiendung: %s) : ", end);
	else printf("\n Dateiname (Automatische Dateiendungen: .ptk .pop .gen ) : ");
	gets(FileName);
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
    printf("\n Start-Population aus Datei (j/n) ? [n] : ");
    gets(input);
    if(input[0]==0) strcpy(input,"n");
  } while (strcmp(input,"j")!=0&&strcmp(input,"n")!=0);
  if(input[0]=='j') {
    strcpy(FileName,FileNameInput(READ_MODE,".pop"));
  } else {
    FileName[0]=0;
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
  do {
    printf("\n Bigamistische Beziehungen erlauben (j/n) ? [n] : ");
    gets(input);
    if(input[0]==0) strcpy(input,"n");
  } while (strcmp(input,"j")!=0&&strcmp(input,"n")!=0);
  if(input[0]=='j') Bigamie=1;
  else Bigamie=0;

  do {
    sprintf( input,"\n Nukleotid-Anzahl [%d] ( 0 <= x <= %d ) : ",
	     NukleoMaxVal-NukleoMinVal+1, NukleoMaxVal-NukleoMinVal+1  );
    ::Input (input, Nukleotide, NukleoMaxVal-NukleoMinVal+1 );
  } while ( Nukleotide<0 || Nukleotide>NukleoMaxVal-NukleoMinVal+1 );
    sprintf( input,"\n Crossing-Over Punkte [%d]:  ", InputCrossVal);
  ::Input (input, CrossVal, InputCrossVal);
    sprintf( input,"\n Mutation-Frequenz [%d]:  ", InputMutationsFreq);
  ::Input (input, MutationFreq, (long)InputMutationsFreq );
    sprintf( input,"\n Inversions-Frequenz [%d]:  ", InputInversionFreq);
  ::Input (input, InversionFreq, (long)InputInversionFreq);
    sprintf( input,"\n Translocations-Frequenz [%d]:  ", InputTranslocationFreq);
  ::Input (input, TranslocationFreq, (long)InputTranslocationFreq);
    sprintf( input,"\n AsymXOverFreq-Frequenz [%d]:  ", InputAsymXOverFreq);
  ::Input (input, AsymXOverFreq, (long)InputAsymXOverFreq);
    sprintf (input,"\n Abbruch nach Anzahl Generationen ohne Verbesserung [%d]:  ", InputMaxNoImprove);
  ::Input (input, MaxNoImprove, InputMaxNoImprove);
  do {
    printf("\n Wollen Sie den SpleissCode Editieren (j/n) ? [n] : ");
    gets(input);
    if(input[0]==0) strcpy(input,"n");
  } while (strcmp(input,"j")!=0&&strcmp(input,"n")!=0);
  if(input[0]=='j') {
    int StopSig=0;
    int j=0;
    int i=0;
    do {
      if( StopSig==0 && i==0 )
	printf("\n Intron Start-Sequenz :\n");
      if( StopSig==1 && TheSpliceCode[i-1]==0 && i>0 )
	printf("\n Intron End-Sequenz   :\n");
      do {
	if(ptrSpliceCode!=NULL) {
	  printf(" %2d.Nukleotid [%2d] : ", i-j+1, ptrSpliceCode[i] );
	  gets(input);
	  if(input[0]==0) sprintf(input,"%d",ptrSpliceCode[i]);
	} else {
	  printf(" %2d.Nukleotid [0] : ", i-j+1 );
	  gets(input);
	}
      } while ( atoi ( input ) < 0 || atoi ( input ) > Nukleotide );
      if( ( TheSpliceCode[i++] = atoi(input) ) == 0 ) {
	StopSig++;
	j=i;
      }
    } while(StopSig<2);
  } else {
    if ( ptrSpliceCode!=NULL ) {
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
