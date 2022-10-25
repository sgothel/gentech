// gentech.cpp                                                  18.2.1994
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

#include "gentech.h"
#include "random.h"
#include "maschine.h"

#include <climits>

// GLOBALE VARIABLEN !!!
char GenTechVersion[]="2.3";

std::ostream& operator<< (std::ostream& OS, Liste<NukleoTyp>& List)
{
	List.Ausgabe (OS);
	return OS;
}

// *******************************************************************
// *******************************************************************
//  C H R O M O S O M
// *******************************************************************
// *******************************************************************

// STATISCHE-KLASSEN-GLOBALE VARIABLEN !!!

Chromosom::Chromosom ( Chromosomen & env,
		       int StartChromosomLength
		     )
: Liste<NukleoTyp>(), env(env), Fitness(0)
// Zufaellig erzeugte Chromosomen
{
  for ( int i=StartChromosomLength; i > 0; i--)
  {
    fuegeEin( Random(env.UserNukleoMinVal,env.UserNukleoMaxVal) );
  }
  assert (laenge() == StartChromosomLength);
}

Chromosom::Chromosom ( Chromosomen & env, const std::string& FileName )
: Liste<NukleoTyp>(), env(env), Fitness(0)
{
  FILE *file;
  int i, len;

  if ((file=fopen(FileName.c_str(), "rb")) == nullptr) INT_ERR (__LINE__);

  // 4-7-94, 3:3 PM
  // Liest als erstes die Groesse des abgespeicherten NukleoTyps
  // aus dem File
  short NukleoLen;
  if (fread (&NukleoLen, sizeof (NukleoLen), 1, file) != 1) INT_ERR (__LINE__);
  if (NukleoLen <= (short)sizeof(double) && NukleoLen > (short)sizeof(NukleoTyp)) {
	std::cerr << "!! WARNUNG !!\n"
	     << "   Laenge des Datentyps der gespeicherten Nuklotide\n"
	     << "   ist kleiner als das abgespeicherte Format\n" 
	     << "   Weitermachen ? (j/n)   ";

	int c = std::cin.get();
	if (tolower (c) != 'j') exit (0);
  } else {
    // Wahrscheinlich altes Dateiformat:
    // 'NukleoLen' auf aktuelle Laenge von 'NukleoTyp' setzen
    // und den Dateizeiger zuruecksetzen
    if (NukleoLen >= (short)sizeof(double)) {
        NukleoLen = sizeof (NukleoTyp);
        std::cerr << "\nWahrscheinlich altes Dateiformat\nSetze Datei zurueck\n";
        fseek (file, 0, SEEK_SET);
    }
  }
  // 4-7-94, 3:3 PM

  // Puffer der richtigen Laenge bereit stellen
  char *save= new char[NukleoLen];
  if (fread (&len, sizeof (len), 1, file) == 0) INT_ERR (__LINE__);
  for(i=0; i<len; i++) {
    if (fread (save, NukleoLen, 1, file) == 0) INT_ERR (__LINE__);
    fuegeEin ( (NukleoTyp)*save, i);
  }
  delete [] save;
  fclose(file);
}

void Chromosom::Copy (const Chromosom &a)
{
  Fitness          = a.Fitness;
  assert (laenge() == a.laenge());
}

Chromosom& Chromosom::operator=(const Chromosom& m)
{
  if (this == &m) return *this;

  // Neue Liste der Nukleotide anlegen. 
  // In Liste<NukleoTyp> wird der *this-Zeiger auf Gleichheit ueberprueft
  // Das Environment env (Chromosomen) bleibt !
  Liste<NukleoTyp>::operator=(m);
  Copy(m);
  return *this;
}

int Chromosom::operator==(const Chromosom& ch) const
{
  if (this == &ch) return 1;             // gleiches Chromosom
  if (THIS.laenge() != ch.laenge())      // garantiert Ungleich
     return 0;

  int i;

  for (i=0; i < laenge() && THIS[i] == ch[i]; i++); // Ueberprufung

  if (i == laenge())    return 1;       // Ziel       erreicht => this == ch
  else                  return 0;       // Ziel nicht erreicht => this != ch
}

int Chromosom::Save (const std::string& FileName) const
{
  FILE *file;
  int i;
  long len;

  if ((file=fopen(FileName.c_str(),"wb")) == nullptr) INT_ERR (__LINE__);
// 4-7-94, 2:51 PM
// Speichert die Groesse des NukleoDatenTyps als erstes in der Datei
  short NukleoLen = sizeof (NukleoTyp);
  if (fwrite (&NukleoLen, sizeof (NukleoLen), 1, file) != 1) INT_ERR (__LINE__);
// 4-7-94, 2:51 PM
  len=laenge();
  if (fwrite (&len, sizeof(len), 1, file) != 1) INT_ERR (__LINE__);
  for(i=0; i<len; i++)
    if (fwrite (&((*this)[i]), sizeof (NukleoTyp), 1, file) != 1)
	INT_ERR (__LINE__);
  fclose(file);
  return 1;
}

int Chromosom::GetNukleotidNumber (int i) const
{
  NukleoTyp UserNukleotidValue=(*this)[i];

  if  (env.UserNukleoValScale * (env.Nukleotide-1)  <= UserNukleotidValue &&
			     UserNukleotidValue     <= env.UserNukleoMaxVal)
  {
    // Trifft auch fuer Nukleotide gleich nullptr zu !!!
    return env.Nukleotide;
  }

  for (int n=env.Nukleotide-1; n > 0; n--) {
    if  (env.UserNukleoValScale * (n-1) <= UserNukleotidValue &&
	 UserNukleotidValue             <  env.UserNukleoValScale * n )
      return n;
  }

  INT_ERR(__LINE__);
  return 0;
}

NukleoTyp Chromosom::GetUserNukleotidValue (int i) const
{
  return env.UserNukleoValScale*(i-1);
}

int Chromosom::FindIntron (int &von, int &bis) const
{
  int done=0, i, i2, i3;

  if (bis-von+1 >= env.ptrSpliceCodeInfo->Length)
  {
    // Spleissen..Anfang suchen
    i=von; i2=0;
    while (i <= bis && env.ptrSpliceCodeInfo->SpliceCode[i2] != 0) {
      if (GetNukleotidNumber (i) == env.ptrSpliceCodeInfo->SpliceCode[i2]) {
	  i2++;
	  i++;
      }
      else { i2=0; von=++i; }
    }
    if (i < bis) {
      assert (env.ptrSpliceCodeInfo->SpliceCode[i2] == 0) ;
      // von gefunden...
      // Spleissen..Ende suchen
      i3=i;
      i=++i2;
      while (i3 <= bis && env.ptrSpliceCodeInfo->SpliceCode[i2] != 0) {
	if (GetNukleotidNumber(i3) == env.ptrSpliceCodeInfo->SpliceCode[i2])
	     i2++;
	else i2=i;
	i3++;
      }
      if (env.ptrSpliceCodeInfo->SpliceCode[i2] == 0) {
	// bis gefunden...
	bis=i3-1;
	done=1;
      }
    }
  }
  return done;
}

int Chromosom::Splicing (void)
{
  int i, NNukleotide=0, IntronStart=0, IntronEnd=laenge()-1;
  # ifndef NDEBUG
    int len_old;
  # endif

  // Die Bedingung fuer den SpliceCode ist Nukleotide > 1 !
  // Nukleotide wird in Chromosomen::CalcParameter gesetzt !
  if (env.Nukleotide <= 1) {
    // Kein Splicing , da kein SpliceCode !!!
    return 0;
  }

  while(FindIntron (IntronStart, IntronEnd) == 1) {
    # ifndef NDEBUG
      len_old=laenge();
    # endif
    assert(IntronStart <= IntronEnd);
    for (i = IntronEnd; i >= IntronStart; i--) {
      assert (laenge() > 0);
      loesche(i);
    }
    assert (len_old-(IntronEnd-IntronStart+1) == laenge()) ;
    NNukleotide+=IntronEnd-IntronStart+1;
    IntronEnd=laenge()-1;
  }
  return NNukleotide;
}

int Chromosom::InsertIntrons (int von, int length)
{
  int done=0, i, iRumpf;

  if (env.ptrSpliceCodeInfo != nullptr &&
      env.ptrSpliceCodeInfo->SpliceCode[0] != 0 &&
      von <= laenge()           && 
      length >= env.ptrSpliceCodeInfo->Length)
  {
    // StartSequenz einfuegen.
    for (i=0; env.ptrSpliceCodeInfo->SpliceCode[i] != 0; i++, von++, length--)
      fuegeEin ( GetUserNukleotidValue(env.ptrSpliceCodeInfo->SpliceCode[i]),
                 von);
    // Stelle fuer Rumpf merken.
    iRumpf=von;
    // EndSequenz einfuegen.
    for (i++; env.ptrSpliceCodeInfo->SpliceCode[i] != 0; i++, von++, length--)
      fuegeEin ( GetUserNukleotidValue (env.ptrSpliceCodeInfo->SpliceCode[i]),
                 von);
    // Rumpf fuellen.
    while (length > 0) {
      fuegeEin (Random (env.UserNukleoMinVal, env.UserNukleoMaxVal),
			iRumpf);
      length--;
    }
    done=1;
  }
  return done;
}

int Chromosom::Inversion (void)
{
  Chromosom save(env);
  int start, end, i;

  assert (save.laenge()==0);
  start = Random (0, laenge()-2);
  assert (start < laenge()-1);
  end = Random (start+1, laenge()-1) ;
  assert (end < laenge());
  assert (end > start);
  for (i=start; i <= end; i++) save.fuegeEin ((*this)[i]);
  assert (save.laenge()==end-start+1);
  for (i=start+1; i <= end; i++)  {
    assert (end-i < save.laenge() && end-i >= 0);
    (*this)[i]=save[end-i];
  }
  return 1;
}

int Chromosom::Translocation (void)
{
  Chromosom save(env);
  int start, end, dest, i;

  assert (save.laenge()==0);
  start = Random (0, laenge()-2);
  assert (start < laenge()-1);
  end = Random (start+1, laenge()-1) ;
  assert (end < laenge());
  assert (end > start);
  while ((end = Random (0, laenge()-1)) <= start) ;
  for (i=start; i <= end; i++) {
    save.fuegeEin((*this)[start]);
    loesche(start);
  }
  assert (save.laenge()==end-start+1);

  dest = Random (0, laenge()-1);
  assert (dest >= 0);
  for (i=0; i<save.laenge(); i++) (*this).fuegeEin (save[i], dest+i);
  return 1;
}

// *******************************************************************
// *******************************************************************
//  C H R O M O S O M E N
// *******************************************************************
// *******************************************************************

// STATISCHE-KLASSEN-GLOBALE VARIABLEN !!!

const long Chromosomen::MutationFreqVar=1000;

Chromosomen::Chromosomen (NukleoTyp UserNukleoMinVal, 
                          NukleoTyp UserNukleoMaxVal,
			  int MaxChromosomen,
			  const std::string& StartGenFile,
			  int Nukleotide,
			  SpliceCodeInfo *PtrSpliceCodeInfo,
			  long InversionFreq,
			  long TranslocationFreq,
			  long AsymXOverFreq,
			  int CrossVal,
			  long MutationFreq
			 )
: Liste<Chromosom>(),
  TheBestEversGeneration(1),
  AverageFitness(0),
  TheBestEversAverageFitness(0),
  BestFitness(0),
  FitnessSum(0),
  Generation(1),
  MaxChromosomen(MaxChromosomen),
  IntroCodeLenSum(0), // FIXME?
  SplicedChromosoms(0), // FIXME?

  TheBestEver(*this),

  fileptrPtk(0),
  EvolutionEnd(0),
  GenerationStart(0),
  EvolutionStart(0),
  GenerationEnd(0),

  ChromosomenLenMin(0), // FIXME
  ChromosomenLenMax(0), // FIXME
  ChromosomenLenAvrg(0), // FIXME
  ChromBetterZeroNumber(0), // FIXME

  MutationsThisGeneration(0),
  InversionsThisGeneration(0),
  TranslocationsThisGeneration(0),
  MutationFreq(MutationFreq),

  InversionFreq(InversionFreq),
  TranslocationFreq(TranslocationFreq),
  AsymXOverFreq(AsymXOverFreq),
  CrossVal(CrossVal),
  XOverNumber(0), // FIXME

  ptrSpliceCodeInfo(PtrSpliceCodeInfo),
  UserNukleoMinVal(UserNukleoMinVal),
  UserNukleoMaxVal(UserNukleoMaxVal),
  UserNukleoValScale(1),
  Nukleotide(Nukleotide)
{
  // StartGene aus dem File holen ! ....
  FILE *file;
  int ChromLen, ChromQuantity, i, j;

  CalcParameter();

  # ifdef __BORLANDC__
    randomize();
  # endif
  if ((file=fopen(StartGenFile.c_str(),"rb") ) == nullptr )
  {
  	fprintf(stderr,"can not open file %s\n", StartGenFile.c_str());
  	INT_ERR(__LINE__);
  }
  // 4-7-94, 3:3 PM
  // Liest als erstes die Groesse des abgespeicherten NukleoTyps
  // aus dem File
  short NukleoLen;
  if (fread (&NukleoLen, sizeof(NukleoLen), 1, file)!=1) INT_ERR(__LINE__);
  if (NukleoLen <= (short)sizeof(double) && NukleoLen > (short)sizeof(NukleoTyp)) {
    std::cerr << "!! WARNUNG !!\n"
	     << "   Laenge des Datentyps der gespeicherten Nuklotide\n"
	     << "   ist kleiner als das abgespeicherte Format\n"
	     << "   Weitermachen ? (j/n)   ";

	int c = std::cin.get();
	if (tolower (c) != 'j') exit (0);
  } else {
    // Wahrscheinlich altes Dateiformat:
    // 'NukleoLen' auf aktuelle Laenge von 'NukleoTyp' setzen
    // und den Dateizeiger zuruecksetzen
    if (NukleoLen >= (short)sizeof(double)) {
        NukleoLen = sizeof (NukleoTyp);
        std::cerr << "\nWahrscheinlich altes Dateiformat\nSetze Datei zurueck\n";
        fseek (file, 0, SEEK_SET);
    }
  }
  // 4-7-94, 3:3 PM

  if (fread(&ChromQuantity, sizeof(ChromQuantity), 1, file)==0)
	INT_ERR(__LINE__);

  // Puffer der richtigen Laenge bereitstellen
  char *save= new char[NukleoLen];
  for (j=0; j<ChromQuantity; j++ ) {
    Chromosom Gamma ( *this, 0 ) ;
    assert (Gamma.laenge()==0);

    if (fread(&ChromLen, sizeof(ChromLen), 1, file) == 0) INT_ERR (__LINE__);
    for(i=0; i<ChromLen; i++) {
      if (fread (save, NukleoLen, 1, file) == 0) INT_ERR(__LINE__);
      Gamma.fuegeEin ((NukleoTyp )*save, i);
    }
    assert (Gamma.laenge()==ChromLen);
    fuegeEin (Gamma, j);
  }
  delete [] save;
  assert (laenge()==ChromQuantity);
  fclose (file);
}

Chromosomen::Chromosomen ( NukleoTyp UserNukleoMinVal, NukleoTyp UserNukleoMaxVal,
			   int MaxChromosomen,
			   int StartChromosomNumber,
			   int StartChromosomLength,
			   int Nukleotide,
			   SpliceCodeInfo *PtrSpliceCodeInfo,
			   long InversionFreq,
			   long TranslocationFreq,
			   long AsymXOverFreq,
			   int CrossVal,
			   long MutationFreq
			)
: Liste<Chromosom>(),
  TheBestEversGeneration(1),
  AverageFitness(0),
  TheBestEversAverageFitness(0),
  BestFitness(0),
  FitnessSum(0),
  Generation(1),
  MaxChromosomen(MaxChromosomen),
  IntroCodeLenSum(0), // FIXME?
  SplicedChromosoms(0), // FIXME?

  TheBestEver(*this),

  fileptrPtk(0),
  EvolutionEnd(0),
  GenerationStart(0),
  EvolutionStart(0),
  GenerationEnd(0),

  ChromosomenLenMin(0), // FIXME
  ChromosomenLenMax(0), // FIXME
  ChromosomenLenAvrg(0), // FIXME
  ChromBetterZeroNumber(0), // FIXME

  MutationsThisGeneration(0),
  InversionsThisGeneration(0),
  TranslocationsThisGeneration(0),
  MutationFreq(MutationFreq),

  InversionFreq(InversionFreq),
  TranslocationFreq(TranslocationFreq),
  AsymXOverFreq(AsymXOverFreq),
  CrossVal(CrossVal),
  XOverNumber(0), // FIXME

  ptrSpliceCodeInfo(PtrSpliceCodeInfo),
  UserNukleoMinVal(UserNukleoMinVal),
  UserNukleoMaxVal(UserNukleoMaxVal),
  UserNukleoValScale(1),
  Nukleotide(Nukleotide)
{
  # ifdef __BORLANDC__
    randomize();
  # endif

  CalcParameter();

  // StartGene zufaellig setzen !
  for (int i=StartChromosomNumber ; i > 0  ; i--) {
    Chromosom Gamma (*this, StartChromosomLength);
    assert (Gamma.laenge() == StartChromosomLength);
    fuegeEin (Gamma);
  }
  assert (laenge()==StartChromosomNumber);
}

int Chromosomen::Save (const std::string& FileName) const
{
  FILE *file;
  int ChromLen, ChromQuantity, i, j;

  if((file=fopen(FileName.c_str(), "wb")) == nullptr) {
      INT_ERR (__LINE__);
  }
  ChromQuantity=laenge();
// 4-7-94, 2:51 PM
// Speichert die Groesse des NukleoTyps als erstes im File ab
  short NukleoLen=sizeof(NukleoTyp);
  if (fwrite (&NukleoLen, sizeof (NukleoLen), 1, file) != 1) INT_ERR (__LINE__);
// 4-7-94, 2:51 PM
  if (fwrite (&ChromQuantity, sizeof (ChromQuantity), 1, file) != 1 )
      INT_ERR (__LINE__);
  for (j=0; j<ChromQuantity; j++ ) {
    NukleoTyp save;
    ChromLen=THIS[j].laenge();
    if (fwrite (&ChromLen, sizeof (ChromLen), 1, file) != 1) INT_ERR (__LINE__);
    for(i=0; i<ChromLen; i++) {
      save=THIS[j][i];
      if (fwrite (&save, sizeof (NukleoTyp), 1, file) != 1) INT_ERR (__LINE__);
    }
  }
  fclose(file);
  return 1;
}

int Chromosomen::Evolution (double GoalFitness, const std::string& chrptrPtkFile,
			    double BirthRate, int Bigamie,
			    int NoImprovingCrossingOvers
			   )
{
  double BestEverAverageFitness=-1,
	 Cut = 0; 	// Der Cut beim Sterben
  int NoImproving=0, stop=0;

  InitFitness() ;
  if( !chrptrPtkFile.empty() ) {
    if ((fileptrPtk=fopen (chrptrPtkFile.c_str(), "wt")) == nullptr) {
      INT_ERR (__LINE__);
    }
  } else {
      fileptrPtk = nullptr;
  }

  EvolutionStart=time(nullptr);
  Generation=1;
  SplicedChromosoms=0;
  IntroCodeLenSum=0;
  Protokoll();
  if(!Echo()) stop=1;

  if (BirthRate <= 0.0 || BirthRate > 1.0) stop=1;

  (void) NoImprovingCrossingOvers;

  while ( BestFitness < GoalFitness &&
	  // NoImproving < NoImprovingCrossingOvers &&
	  stop==0
	)
  {
    Generation++;
    SplicedChromosoms=0;
    IntroCodeLenSum=0;
    if (BirthRate < 1.0)  {
      NewGeneration (BirthRate, Bigamie);
      // Fuer LetDie !!!
      Cut = GetXWorstFitness (laenge()-MaxChromosomen);
    } else {
      NewGeneration (Bigamie);
      Cut = -.5;	// Eltern Fitness auf -1 gesetzt
    }
    // Sterben !!!
    LetDie (Cut);
    // Die Mutation
    Mutation();
    // Fitness berechnen
    CalcWholeFitness();
    // Weitere Mutationen
    InversionsMutation();
    TranslocationsMutation();

    GenerationEnd=time(nullptr);

    if (BestEverAverageFitness < AverageFitness) {
      NoImproving=0;
      BestEverAverageFitness = AverageFitness ;
    } else NoImproving++ ;
    Protokoll();
    if(!Echo()) stop=1;
  }

  EvolutionEnd=time(nullptr);
  Echo();
  Protokoll();

  if (fileptrPtk != nullptr) fclose (fileptrPtk);

  return Generation;
}

void Chromosomen::InitFitness(void)
{
  for (int i = 0; i < laenge(); i++) {
    Chromosom Neu = THIS[i];
    Neu.Splicing();
    THIS[i].SetFitness(Fitness(Neu));
  }
  CalcWholeFitness();
}

int Chromosomen::NewGeneration (double BirthRate, int Bigamie)
{
  Liste<int> ElternPaare;

  int l= (int) ( laenge()*BirthRate );
  int i;

  GenerationStart=time(nullptr);

  // Gerade Anzahl der ElternPaare !!!
  if (l%2 > 0) l-- ;

  // ooops, die letzten beiden Elternpaare sind nicht-bigamistisch SCHLECHT
  // zu finden !!!
  if (ChromBetterZeroNumber<=l) ChromBetterZeroNumber-=2;

  // Neue Partner in die ElternPaare-Menge einfuegen
  // "Bigamistische Beziehung" werden, wenn moeglich, nicht erlaubt.
  while(l>0) {
    // Nur NEUE Partner !!!
    if (!Bigamie && ElternPaare.laenge() < ChromBetterZeroNumber) {
	do {
	  i=RouletteSelect();
	} while (ElternPaare.istElement (i) >= 0) ;
	ElternPaare.fuegeEin(i);
	l--;
    } else {
	if (!Bigamie)   {
	        // the hard way ...
		do {
		  i=Random (0, laenge()-1);
		} while (ElternPaare.istElement (i) >= 0) ;
		ElternPaare.fuegeEin(i);
		l--;
	} else { 
	    // Bigamistische Beziehung JA, aber sexuell !!!
	    int w, m;
	    ElternPaare.fuegeEin ((w = Random (0, laenge()-1))) ;
	    while (w == (m=Random (0, laenge()-1))) ;
	    ElternPaare.fuegeEin (m) ;
	    l-=2;
	}
    }
  };

  l=ElternPaare.laenge();

  # ifndef NDEBUG
    // alles ok ?? GERADE ELTERNPAARE ????
    assert (l%2==0);
    assert (l==(((int) (laenge() * BirthRate) % 2) ?
			(int) ((laenge() * BirthRate ) - 1) :
			(int)(laenge()*BirthRate) )
	   );
  # endif

  // Fortpflanzen !!!
  while (l > 1) {
    CrossingOver (ElternPaare[l-1], ElternPaare[l-2]);
    l -= 2 ;
  }
  assert (l == 0);

  return 1;
}


int Chromosomen::NewGeneration (int Bigamie)
{
  Liste<int> ElternPaare;
  int l=MaxChromosomen, w, i;

  GenerationStart=time(nullptr);

  // Gerade Anzahl der ElternPaare !!!
  if (l%2 > 0) l-- ;

  // ooops, die letzten beiden Elternpaare sind nicht-bigamistisch SCHLECHT
  // zu finden !!!
  if (ChromBetterZeroNumber<=l) ChromBetterZeroNumber-=2;

  // Neue Partner in die ElternPaare-Menge einfuegen
  // "Bigamistische Beziehung" werden, wenn moeglich, nicht erlaubt.
  while(l>0) {
    // Nur NEUE Partner !!!
    if (!Bigamie && ElternPaare.laenge()<ChromBetterZeroNumber) {
	do {
	  i=RouletteSelect();
	} while (ElternPaare.istElement (i) >= 0) ;
	ElternPaare.fuegeEin(i);
	l--;
    } else {
	if (!Bigamie)   {
	do {
	  i=Random (0, laenge()-1);
	} while (ElternPaare.istElement (i) >= 0) ;
	ElternPaare.fuegeEin(i);
	l--;
	} else { // Bigamistische Beziehung JA, aber sexuell !!!
	    int w, m;
	    ElternPaare.fuegeEin ((w = Random (0, laenge()-1))) ;
	    while(w == (m=Random (0, laenge()-1)))  ;
	    ElternPaare.fuegeEin (m) ;
	    l-=2;
	}
    }
  };

  l=ElternPaare.laenge();


  # ifndef NDEBUG
    // alles ok ?? GERADE ANZAHL DER ELTERN !!!
    assert (l%2==0);
  # endif

  // Neue Population...
  l=ElternPaare.laenge();
  while (l > 1) {
    // Beim Crossing Over werden die Nachkommen am Ende der Populations-Liste
    // eingefuegt.
    // Die Indizes fuer die ElternPaare bleiben somit aktuell !!!
    CrossingOver (ElternPaare[l-1], ElternPaare[l-2]);
    l-=2;
  }

  // Eltern markieren, damit sie "getoetet" werden koennen !!!
  // Das geschied in 'LetDie', Aufruf in Evolution
  l=ElternPaare.laenge()-1;
  while (l > 0) {
    w=ElternPaare[l-1];
    // markieren !!!
    THIS[w].SetFitness(-1);
    l--;
  }
  return 1;
}

int Chromosomen::LetDie (double cut)
{
  int i=0;

  while (i < laenge() && laenge() > MaxChromosomen) {
    if (THIS[i].GetFitness() <= cut)
      Kill (i);
    else
      i++;
  }

  // Sicherheitsabfrage
  if (laenge() > MaxChromosomen) {
    cut = GetXWorstFitness (laenge()-MaxChromosomen);
    i = 0;
    while (i < laenge() && laenge() > MaxChromosomen) {
      if (THIS[i].GetFitness() <= cut)
	Kill (i);
      else
	i++;
    }
  }
  assert (laenge() <= MaxChromosomen);
  return 1;
}

int Chromosomen::RouletteSelect (void) const
// Chris: geaendert
{
  int i;

  double Sum       = 0.0;
  double Threshold ;

  Threshold = Random (0.0, 1.0) ;
  Threshold *= GetFitnessSum() ;

  // Aufgrund der Ungenauigkeit der Flieskommaoperatoren
  // muss auch das Ende der Liste abgefragt werden.
  for (i=0; Sum < Threshold && i<laenge(); i++) {
    Sum += THIS[i].GetFitness();
  }
  return ( (i<laenge()) ? i : laenge()-1);
}

int Chromosomen::CreateNewSymChromosom ( Chromosom &dest, int m, int w,
					 SortListe<int> &CrossPoints
				       )
{
  // i          : Indize des Crosspoints
  // von, bis   : Zu Uebertragender Chromosomenabschnitt [von..bis[
  // ch         : Alternierendes Indize zwischen den beiden Chromosomen
  //              mit den Geschlechtern 'm' und 'w'
  // done       : Abbruch Indikator
  int done, i, von, bis, ch;

  // Startwerte !!
  done = i = bis = 0;
  ch = w;

  do {
    // An dem letzten exklusiven Ende fortfahren
    von = bis;
    if ( i < CrossPoints.laenge() ) {
      // Chromosomenabschnitt-Ende holen.
      if( CrossPoints[i] < THIS[ch].laenge() ) bis = CrossPoints[i];
      else                                     bis = THIS[ch].laenge();
      i++;
    } else {
      // ...und den Rest uebertragen
      bis = THIS[ch].laenge();
      done=1;
    }
    assert ( bis <= THIS[ch].laenge() );
    assert ( von == dest.laenge() );
    // Chromosomenabschnitt uebertragen [von..bis[
    for (int n=von; n<bis; n++)
      dest.fuegeEin(THIS[ch][n],n);
    // Indize zwischen den Geschlechtern alternieren
    ch = ( ch==m ? w : m ) ;
  } while (done==0);

  // ...sonst Fehler !!!
  assert( dest.laenge() > 0 );

  return 1;
}

int Chromosomen::CrossingOver (int m, int w)
{
  if ( CrossVal == 0 ) return 1;

  int shortest=( ( THIS[m].laenge() < THIS[w].laenge() ) ? m : w);

  if ( XOverNumber++ < AsymXOverFreq || AsymXOverFreq==0 ) {
    // Symmetrisches XOver !!!
    SortListe<int> CrossPoints;
    assert( CrossPoints.laenge()==0 );

    // Kreuzungspunkte sortiert eintragen.
    for ( int i = CrossVal; i>0; i-- ) {
      CrossPoints.fuegeEin( Random(0 , THIS[shortest].laenge()) );
    }

    Chromosom NeuA (*this, 0);
    Chromosom NeuB (*this, 0);
    int SplicedCode;

    CreateNewSymChromosom ( NeuA, m, w, CrossPoints );
    fuegeEin ( NeuA, laenge() );
    SplicedCode=NeuA.Splicing();
    if(SplicedCode>0) {
      SplicedChromosoms++;
      IntroCodeLenSum+=SplicedCode;
    }
    // Die Fitness des gespleissten Chromosomes in das ungespleisste
    // eingebundene Chromosom einsetzen !!!
    THIS[laenge()-1].SetFitness(Fitness(NeuA));

    CreateNewSymChromosom ( NeuB, w, m, CrossPoints );
    fuegeEin ( NeuB, laenge() );
    SplicedCode=NeuB.Splicing();
    if(SplicedCode>0) {
      SplicedChromosoms++;
      IntroCodeLenSum+=SplicedCode;
    }
    // Die Fitness des gespleissten Chromosomes in das ungespleisste
    // eingebundene Chromosom einsetzen !!!
    THIS[laenge()-1].SetFitness(Fitness(NeuB));

  } else {

    XOverNumber = 0;
    // Assymetrisches XOver : Nur ein neues Element !
    Chromosom Neu (*this, 0);
    int i, n;

    // Kopieren
    Neu=THIS[m];

    // anhaengen
    for (n=0, i=Neu.laenge(); n<THIS[w].laenge(); n++, i++)
      Neu.fuegeEin(THIS[w][n],i);
    Neu.SetFitness(Fitness(Neu));
    fuegeEin ( Neu, laenge() );

  }
  return 1;
}

int Chromosomen::Mutation (void)
{
  static long NukleotidsToPass=0;
  int ChromosomLen, NukleotidsPassedInChromosom, MutationFlag;
  long lokMutationFreqVar;

  if ( MutationFreq > 0 ) {
    assert ( (MutationFreq/MutationFreqVar) < INT_MAX &&
             (MutationFreq/MutationFreqVar) >= 0 ) ;

    MutationsThisGeneration=0;

    lokMutationFreqVar = MutationFreq +
                         Random( 0, (int)(MutationFreq/MutationFreqVar) ) ;
    // NUR zur Initialisierung von NukleotidsToPass
    if ( NukleotidsToPass==0 ) NukleotidsToPass=lokMutationFreqVar;
    // Alle Chromosomen von hinten nach vorne abarbeiten.
    // NukleotidsToPass zaehlt die zu passierten Nukleotide,
    // bis wieder ein Nukleotid Mutiert werden kann.
    // Ist NukleotidsToPass <= 0, so befindet sich das naechste zu
    // mutierende Nukleotid im selben Chromosom !!!
    for ( int i = laenge() - 1; i >= 0; i-- ) {
      MutationFlag=0;
      ChromosomLen=THIS[i].laenge();
      NukleotidsToPass-=ChromosomLen;
      while ( NukleotidsToPass <= 0 ) {
        // NukleotidsPassedInChromosom gibt die Stelle des zu Mutierenden
        // Nukleotids an.
        assert ( (ChromosomLen+NukleotidsToPass) < INT_MAX &&
                 (ChromosomLen+NukleotidsToPass) >= 0 ) ;
        NukleotidsPassedInChromosom = (int)(ChromosomLen+NukleotidsToPass);

        // Die Mutation.
        (THIS[i])[NukleotidsPassedInChromosom-1] =
          Random ( UserNukleoMinVal, UserNukleoMaxVal);
                                                                                        MutationsThisGeneration++;
        MutationFlag=1;
        // Neuberechnung der zu passierenden Nukleotide bis zur naechsten
        // Mutation !
        NukleotidsToPass =
          lokMutationFreqVar - ChromosomLen + NukleotidsPassedInChromosom ;
      }
      if(MutationFlag==1) {
        // Fitness neuberechnen ...
        // ... this is a social event - we are hard folks :-)
        Chromosom Neu (THIS[i]);
        Neu.Splicing();
        THIS[i].SetFitness(Fitness(Neu));
      }
    }
  }
  return 1;                                
}


int Chromosomen::InversionsMutation (void)
{
  static long ChromosomenToPass=InversionFreq;
  int ChromosomenPassed;

  if (InversionFreq > 0) {
    ChromosomenToPass-=laenge();
    InversionsThisGeneration=0;
    while (ChromosomenToPass < 0) {
      // ChromosomenPassed gibt die Stelle des zu Mutierenden
      // Chromosomes an.
      assert (laenge()+ChromosomenToPass < INT_MAX &&
	      laenge()+ChromosomenToPass >= 0) ;
      ChromosomenPassed = (int)(laenge()+ChromosomenToPass);
      THIS[ChromosomenPassed].Inversion();
      InversionsThisGeneration++;
      // Fitness neuberechnen ...
      Chromosom Neu (THIS[ChromosomenPassed]);
      Neu.Splicing();
      THIS[ChromosomenPassed].SetFitness(Fitness(Neu));
      // Neuberechnung der zu passierenden Nukleotide bis zur naechsten
      // Mutation !
      ChromosomenToPass =
	InversionFreq - laenge() + ChromosomenPassed ;
    }
  }
  return 1;
}

int Chromosomen::TranslocationsMutation (void)
{
  static long ChromosomenToPass=TranslocationFreq;
  int ChromosomenPassed;

  if (TranslocationFreq > 0) {
    ChromosomenToPass-=laenge();
    TranslocationsThisGeneration=0;
    while (ChromosomenToPass < 0) {
      // ChromosomenPassed gibt die Stelle des zu Mutierenden
      // Chromosomes an.
      assert ((laenge()+ChromosomenToPass) < INT_MAX &&
	      (laenge()+ChromosomenToPass) >= 0) ;
      ChromosomenPassed = (int)(laenge()+ChromosomenToPass);
      THIS[ChromosomenPassed].Translocation();
      TranslocationsThisGeneration++;
      // Fitness neuberechnen ...
      Chromosom Neu (THIS[ChromosomenPassed]);
      Neu.Splicing();
      THIS[ChromosomenPassed].SetFitness(Fitness(Neu));
      // Neuberechnung der zu passierenden Nukleotide bis zur naechsten
      // Mutation !
      ChromosomenToPass =
	TranslocationFreq - laenge() + ChromosomenPassed ;
    }
  }
  return 1;
}

int Chromosomen::CalcWholeFitness (void)
{
  double Total=0, TempFitness;
  int BestChrom=-1,ChromLen;
  unsigned long ChromLenSum=0;

  ChromosomenLenMin=INT_MAX-1;
  ChromosomenLenMax=0;
  BestFitness=-1;
  ChromBetterZeroNumber=0;

  for (int i = laenge() - 1; i >= 0; i--) {
    ChromLen=THIS[i].laenge();
    ChromLenSum+=ChromLen;
    if(ChromosomenLenMin>ChromLen) ChromosomenLenMin=ChromLen;
    if(ChromosomenLenMax<ChromLen) ChromosomenLenMax=ChromLen;
    if ((TempFitness = THIS[i].GetFitness())>2*MASCHINE::epsilon)
      ChromBetterZeroNumber++ ;
    Total          += TempFitness ;
    if (BestFitness < TempFitness) {
      BestFitness=TempFitness;
      BestChrom=i;
    }
  }
  AverageFitness = Total/laenge();
  FitnessSum = Total;
  ChromosomenLenAvrg = (double)ChromLenSum/(double)laenge();
  if (TheBestEver.GetFitness() < BestFitness) {
    TheBestEver=THIS[BestChrom];
    TheBestEver.Splicing();
  }
  if (TheBestEversAverageFitness < AverageFitness) {
      TheBestEversAverageFitness = AverageFitness;
      TheBestEversGeneration     = Generation;
  }

  return 1;
}

int Chromosomen::GetWorstChromosom (void) const
{
  double WorstFitness=2, TempFitness;
  int iw=-1;

  for (int i=laenge()-1; i>=0; i--) {
    TempFitness=THIS[i].GetFitness();
    if (WorstFitness > TempFitness) {
      WorstFitness = TempFitness ;
      iw=i;
    }
  }
  return iw;
}

double Chromosomen::GetXWorstFitness(int count) const
{
  SortListe<double> Worst;

  if (count <= 0)      return -1;      // Falsche Anzahl uebergeben
  for (int i=0; i<laenge(); i++) {
    if (Worst.laenge()<count) Worst.fuegeEin (THIS[i].GetFitness());
    else {
      if (THIS[i].GetFitness()<Worst[count-1])
	Worst.fuegeEin (THIS[i].GetFitness());
    }
  }
  if (Worst.laenge() >= count)  return Worst[count-1];
  else                         return Worst[Worst.laenge()-1];
}

int Chromosomen::GetBestChromosom (void) const
{
  double BestFitness=-1, TempFitness;
  int ib=-1;

  for (int i=laenge()-1; i>=0; i--) {
    TempFitness=THIS[i].GetFitness();
    if (BestFitness < TempFitness) {
      BestFitness = TempFitness ;
      ib=i;
    }
  }
  return ib;
}

const Chromosom &Chromosomen::GetTheBestEverChromosom (void)
// Hier kein kein Slicing mehr, da beim abspeichern des besten
// Chromosoms in der Variablen 'TheBestEver' der Code schon
// gesplict abgespeichert wird.
{
  return (const Chromosom&) TheBestEver;
}

void Chromosomen::CalcParameter(void)
{
  if (ptrSpliceCodeInfo==nullptr ||
      ptrSpliceCodeInfo->SpliceCode==nullptr ||
      ptrSpliceCodeInfo->SpliceCode[0]==0
     )
  {
    // Ohne Splicing ....
    Nukleotide=0;
    UserNukleoValScale=1;
  } else {
    if (ptrSpliceCodeInfo->Length ==0) {
      // Laenge berechnen ...
      int i;

      for (i=0; ptrSpliceCodeInfo->SpliceCode[i]!=0; i++)
	ptrSpliceCodeInfo->Length ++;
      for (i++; ptrSpliceCodeInfo->SpliceCode[i]!=0; i++)
	ptrSpliceCodeInfo->Length ++;
    }
    if(Nukleotide>0) {
      UserNukleoValScale =
	(UserNukleoMaxVal- UserNukleoMinVal + 1) / Nukleotide ;
    } else UserNukleoValScale=1;
  }
}

void Chromosomen::Protokoll (void)
{
  double SpliceCodePerChrom ;

  SpliceCodePerChrom = (SplicedChromosoms > 0) ?
		       ((double)IntroCodeLenSum/(double)SplicedChromosoms)
		       : 0.0
		     ;

  if (fileptrPtk != nullptr) {
    if (EvolutionEnd == 0) {
       fprintf (fileptrPtk, "=======================================================\n\n\n");
	 fprintf (fileptrPtk, "\nGeneration / Generierungsdauer        : %3d  /  %ld s\n",
	     Generation, GenerationEnd-GenerationStart );
	   fprintf (fileptrPtk, "Populationsgroesse                    : %3d\n",
	     laenge());
	   fprintf (fileptrPtk, "Chromosomen Laenge Minimum            : %3d\n",
	     ChromosomenLenMin);
	   fprintf (fileptrPtk, "Chromosomen Laenge Maximum            : %3d\n",
	     ChromosomenLenMax);
	   fprintf (fileptrPtk, "Chromosomen Laenge Durchschnitt       : %10.6lf\n",
	     ChromosomenLenAvrg);
	   fprintf (fileptrPtk, "Gespleisste Chromosomen               : %3d\n",
	     SplicedChromosoms);
	   fprintf (fileptrPtk, "Gespleisstes Code pro Chromosom       : %10.6lf\n",
	     SpliceCodePerChrom);

	 fprintf (fileptrPtk, "\nBestFitness                           : %10.6lf\n",
	     BestFitness);
	   fprintf (fileptrPtk, "Average Fitness                       : %10.6lf\n",
	     AverageFitness);

	 fprintf (fileptrPtk, "\nMutationen dieser Generation          : %3d\n",
	    MutationsThisGeneration);
	   fprintf (fileptrPtk, "Inversionen dieser Generation         : %3d\n",
	    InversionsThisGeneration);
	   fprintf (fileptrPtk, "Translokationen dieser Generation     : %3d\n",
	    TranslocationsThisGeneration);
	 fprintf (fileptrPtk, "\nTheBestEverFitness                    : %10.6lf\n",
	     TheBestEver.GetFitness());
	   fprintf (fileptrPtk, "TheBestEversAverageFitness            : %10.6lf\n",
	     TheBestEversAverageFitness);
	   fprintf (fileptrPtk, "TheBestEversGeneration                : %3d\n\n",
	     TheBestEversGeneration);
    } else {
      if(Generation>1) {
	 fprintf (fileptrPtk, "Avrg. Generationsdauer                  : %f s / Generation\n",
	   ((double) (EvolutionEnd-EvolutionStart))/((double)(GetGeneration()-1)) );
	     }
	 fprintf (fileptrPtk, "\nGenerationen / Evolutionsdauer        : %3d /  %3ld s\n",
	     Generation, EvolutionEnd-EvolutionStart);
    }
  }
}

int Chromosomen::Echo (void) const
{
    if (Generation == 1) {
        printf(" Generation: BestGeneration: AverageFitness: BestFitness:"
                " TheBestLength:\n");
    }
    printf("\r%11d%16d%16.9lf%13.9lf%15d",
            GetGeneration(), TheBestEversGeneration,
            GetAverageFitness(), GetBestFitness(),
            TheBestEver.laenge() );

    if (EvolutionEnd > 0) {
        if (Generation > 1) {
            printf ("\n\nAvrg. Generationsdauer                : %f s / Generation\n",
                    ((double)(EvolutionEnd-EvolutionStart))/((double)(GetGeneration()-1)) );

        }
        printf ("\n\nGenerationen / Evolutionsdauer        : %3d  /  %3ld s\n",
                Generation, EvolutionEnd-EvolutionStart);
    }
    return 1;
}

void Chromosom::Ausgabe (std::ostream& OS)
{
  const int Zeilenlaenge = 10;

  OS << "( " << laenge() << " ) < ";
  for (int i=0; i<laenge(); i++) {
    if (i % Zeilenlaenge == 0 &&  i)   OS << "\n\t";

    double wandler = (*this)[i];
    OS << wandler;
    if (i % Zeilenlaenge != Zeilenlaenge-1 && i != laenge()-1)
	OS << ", ";
  }
  OS << " >";
}

void Chromosomen::Ausgabe (std::ostream& OS)
{
  const int Zeilenlaenge = 7;

  OS << "( " << laenge() << " ) < ";
  for (int i=0; i<laenge(); i++) {
    if (i % Zeilenlaenge == 0 &&  i)   OS << "\n\t";

    OS << (((*this)[i].GetFitness()< MASCHINE::epsilon) ?
	(double)(0) : (*this)[i].GetFitness());
    if (i % Zeilenlaenge != Zeilenlaenge-1 && i != laenge()-1)
	OS << ", ";
  }
  OS << " >";
}

/*
std::ostream& operator << (std::ostream& OS, Liste<NukleoTyp>& Ch)
{
  Ch.Ausgabe (OS);
  return OS;
}
*/
# ifdef TEST
    // Der Testteil
    # include "error.cpp"
    # include "maschine.cpp"

    int main()
    {
      Chromosom test1(0, 4, 5, nullptr, 50), test2(0, 4, 5, nullptr, 60);

      randomize();
      if (test1 == test1)
	cout << "\ntest2\n" << test2;
      else
	cerr << "\nopps: Operator \"==\" arbeitet nicht korekt !!";

      if (test1 != test2)
	cout << "\ntest1\n" << test1;
      else
	cerr << "\nopps: Operator \"!=\" arbeitet nicht korekt !!";

      test2 = test1;
      getch();

      if (test2 == test1)
	cout << "\ntest2 nach Zuweisung\n" << test2;
      else
	cerr << "\nopps: Operator \"==\" arbeitet nicht korekt !!";

      while (!kbhit())  {
	Chromosom test3(0, Random (0, 10), 5, nullptr, Random (30, 50));
	cout << endl << test3;
	getch();
      }
      return 0;
    }
# endif

