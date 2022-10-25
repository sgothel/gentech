// salesgen.cpp                                                  6.3.1994
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

#include <climits>

#include "sales_gen.h"

#include "random.h"

#ifndef NDEBUG
    #include "menge.h"
#endif

SalesChromosom::SalesChromosom ( SalesChromosomen & env, const std::string& FileName )
: Chromosom(env, FileName) {}

SalesChromosom::SalesChromosom ( SalesChromosomen & env, int StartChromosomLength)
: Chromosom(env, 0)
// Zufaellig erzeugte Chromosomen
{
  /* UNIQUE NUKLEONS */
   if (StartChromosomLength > env.UserNukleoMaxVal - env.UserNukleoMinVal + 1)
       std::cerr << "\nFehler !! "
               << "Chromsomenlaenge groesser als Nukleotideauswahl !!";
    for (int i=StartChromosomLength; i > 0; i--) {
      long Rand;
      while (istElement (Rand = Random (env.UserNukleoMinVal, env.UserNukleoMaxVal)) >= 0
         );
      fuegeEin (Rand);
    }
    assert (laenge() == StartChromosomLength);        
}

SalesChromosomen::SalesChromosomen(
			    int MaxChromosomen,
			    int StartChromosomNumber,
			    int StartChromosomLength,
			    int Nukleotide,
			    SpliceCodeInfo *ptrSpliceCodeInfo,
			    long InversionFreq,
			    long TranslocationFreq,
			    long AsymXOverFreq,
			    int CrossVal,
			    long MutationFreq,
			    int NoImprovingCrossingOvers
			  )
: Chromosomen ( SalesGame::CODE_MIN_VAL, SalesGame::CODE_MAX_VAL,
		MaxChromosomen, 0 /* use SalesChromosomes here */,
		StartChromosomLength, Nukleotide, ptrSpliceCodeInfo,
		InversionFreq, TranslocationFreq, AsymXOverFreq,
		CrossVal, MutationFreq
	      ),
  TheGame( 1 ),
  NoImproving(1),
  NoImprovingCrossingOvers(NoImprovingCrossingOvers),
  Flag (0),
  WorstDistance (0),
  BestDistance (-1)
{ 
  // StartGene zufaellig setzen !
  for (int i=StartChromosomNumber ; i > 0  ; i--) {
    SalesChromosom Gamma (*this, StartChromosomLength);
    assert (Gamma.laenge() == StartChromosomLength);
    fuegeEin (Gamma);
  }
  assert (laenge()==StartChromosomNumber);
}

SalesChromosomen::SalesChromosomen(
			    int MaxChromosomen,
			    const std::string& StartGenFile,
			    int Nukleotide,
			    SpliceCodeInfo *ptrSpliceCodeInfo,
			    long InversionFreq,
			    long TranslocationFreq,
			    long AsymXOverFreq,
			    int CrossVal,
			    long MutationFreq,
			    int NoImprovingCrossingOvers
			  )
: Chromosomen ( SalesGame::CODE_MIN_VAL, SalesGame::CODE_MAX_VAL,
		MaxChromosomen, StartGenFile,
		Nukleotide, ptrSpliceCodeInfo,
		InversionFreq, TranslocationFreq, AsymXOverFreq,
		CrossVal, MutationFreq
	      ),
  TheGame( 1 ),
  NoImproving(1),
  NoImprovingCrossingOvers(NoImprovingCrossingOvers),
  Flag (0),
  WorstDistance (0),
  BestDistance (-1)
{ }

double SalesChromosomen::Fitness (const Chromosom &Lsg)
{
  double Distance = TheGame.Play (Lsg, false);

  if (Distance > WorstDistance)	{
    WorstDistance = Distance;
    Flag |= 1;
  }
  assert (WorstDistance != 0);

  return (WorstDistance - Distance) / WorstDistance;
}

int SalesChromosomen::Evolution (double GoalFitness, const std::string& chrptrPtkFile,
			    double BirthRate, int Bigamie
			   )
{
  (void) GoalFitness;
  double CutFitness = 0; 	// Der Cut beim Sterben
  int stop=0;

  InitFitness() ;
  if( !chrptrPtkFile.empty() ) {
    if ((fileptrPtk=fopen (chrptrPtkFile.c_str(), "wt")) == nullptr)
      INT_ERR (__LINE__);
  } else fileptrPtk = nullptr;

  EvolutionStart=time(nullptr);
  Generation=1;
  SplicedChromosoms=0;
  IntroCodeLenSum=0;
  Protokoll();
  if(!Echo()) stop=1;

  if (BirthRate <= 0.0 || BirthRate > 1.0) stop=1;

  while ( NoImproving < NoImprovingCrossingOvers && stop==0 ) 
  {
    Generation++;
    SplicedChromosoms=0;
    IntroCodeLenSum=0;
    if (BirthRate < 1.0)  {
      NewGeneration (BirthRate, Bigamie);
      // Fuer LetDie !!!
      CutFitness = GetXWorstFitness (laenge()-MaxChromosomen);
    } else {
      NewGeneration (Bigamie);
      CutFitness = -.5;	// Eltern Fitness auf -1 gesetzt
    }
    // Sterben !!!
    LetDie (CutFitness);

    // Die Mutation
    Mutation();

    // Fitness berechnen. Vorher Abfrage ob sich 'WorstDistance' geaendert
    // hat. Wenn ja, muss die Fitness fuer die gesammte Population berechnet
    // werden
    if (Flag&1) { InitFitness(); Flag&=~1; }
    else CalcWholeFitness();

    // Weitere Mutationen
    InversionsMutation();
    TranslocationsMutation();

    GenerationEnd=time(nullptr);

    if (Flag & 2) { NoImproving=0; Flag&=~2; } 
    else NoImproving++ ;
    Protokoll();
    if(!Echo()) stop=1;
  }

  EvolutionEnd=time(nullptr);
  Echo();
  Protokoll();

  if (fileptrPtk != nullptr) fclose (fileptrPtk);

  TheGame.Play (TheBestEver, true);

  return Generation;
}

/* UNIQUE NUKLEOTIDS */
void SalesChromosomen::CreateNewSymChromosom (Chromosom &dest, int m, int w,
					                          SortListe<int> &CrossPoints)
// Programiert nach Pseudocode aus:
// Eberhard Schoeneburg, Frank Heinzmann, Sven Feddersen:
// Genetische Algorithmen und Evolutionsstrategien,
// Addison-Wesley, ISBN 3-89319-493-2,
// Seite 274/275
{
  // i          : Indize des Crosspoints
  // von, bis   : Zu Uebertragender Chromosomenabschnitt [von..bis[
  // ch         : Alternierendes Indize zwischen den beiden Chromosomen
  //              mit den Geschlechtern 'm' und 'w'
  // done       : Abbruch Indikator
  int done, i, von, bis;

  // Startwerte !!
  done = i = bis = 0;
  dest = THIS[m];

  # ifndef NDEBUG
      if (THIS[w].laenge() != THIS[m].laenge() ||
	  THIS[w].laenge() != dest.laenge())
	  std::cout << std::endl << "THIS[w].laenge() = " << THIS[w].laenge()
	       << std::endl << "THIS[m].laenge() = " << THIS[m].laenge()
	       << std::endl << "dest.laenge()    = " << dest.laenge();
  # endif

  do {
    // An dem letzten exklusiven Ende fortfahren
    von = bis;
    if (i < CrossPoints.laenge()) {
      // Chromosomenabschnitt-Ende holen.
      bis = CrossPoints[i];
      i++;
    } else {
      // ...und den Rest uebertragen
      bis = THIS[m].laenge();
      done=1;
    }

    if (i % 2) {
      int n;
      Liste<NukleoTyp> intervall;
      assert (intervall.laenge() == 0);

      // Gene im Intervall puffern
      for (n = von; n < bis; n++)
	intervall.fuegeEin (THIS[w][n], intervall.laenge());

      // Gene aus 'w1' in 'dest' suchen und loeschen
      for (n = 0; n < dest.laenge() && intervall.laenge() > 0; n++)
      {
	for (int l = 0; l < intervall.laenge(); l++)
	{
	    if (intervall[l] == dest[n])        {
	      dest.loesche (n);
	      intervall.loesche (l);
	      l=-1;	// Neuen Suchdurchlauf initiieren
	    }
        }
      }

      assert (intervall.laenge() == 0);

      // Intervall von 'dest' mit Genen von 'w' fuellen
      for (n = von; n < bis; n++)
	  dest.fuegeEin (THIS[w][n], n);
    }
  } while (done==0);

  #ifndef NDEBUG
      // Ueberpruefung, ob jedes Element nur einmal vorhanden ist
      Menge<NukleoTyp> test;

      for (int j = 0; j < dest.laenge(); j++) {
          test.fuegeEin (dest[j]);
      }
      if (dest.laenge() != THIS[w].laenge() || test.card() != dest.laenge()){

          std::cout << "\ndest:\n" << dest
                  << std::endl << "THIS[m].laenge() = " << THIS[m].laenge()
                  << std::endl << "THIS[w].laenge() = " << THIS[w].laenge()
                  << std::endl << "test.laenge()    = " << test.card()
                  << std::endl;
      }
      assert (test.card() == dest.laenge());
      assert (
              dest.laenge() == THIS[m].laenge() &&
              dest.laenge() == THIS[w].laenge()
      );
  # endif
}

/* UNIQUE NUKLEOTIDS */
void SalesChromosomen::CrossingOver (int m, int w)
// Order Crossing Over
//
{

  if (CrossVal == 0) { return; }

  // Symmetrisches XOver !!!
  SortListe<int> CrossPoints;
  assert (CrossPoints.laenge()==0);

  // Kreuzungspunkte sortiert eintragen.
  for (int i = CrossVal; i>0; i--) {
    CrossPoints.fuegeEin( Random (0 , THIS[w].laenge()));
  }

  SalesChromosom NeuA (*this);
  SalesChromosom NeuB (*this);
  int SplicedCode;

  CreateNewSymChromosom (NeuA, m, w, CrossPoints);
  fuegeEin (NeuA, laenge());
  SplicedCode=NeuA.Splicing();
  if(SplicedCode>0) {
    SplicedChromosoms++;
    IntroCodeLenSum+=SplicedCode;
  }
  // Die Fitness des gespleissten Chromosomes in das ungespleisste
  // eingebundene Chromosom einsetzen !!!
  THIS[laenge()-1].SetFitness(Fitness(NeuA));
  CreateNewSymChromosom (NeuB, w, m, CrossPoints);
  fuegeEin (NeuB, laenge());
  SplicedCode=NeuB.Splicing();
  if(SplicedCode>0) {
    SplicedChromosoms++;
    IntroCodeLenSum+=SplicedCode;
  }
  THIS[laenge()-1].SetFitness(Fitness(NeuB));
}

void SalesChromosomen::Mutation (void)
{
  static long NukleotidsToPass=0;
  int ChromosomLen, NukleotidsPassedInChromosom;
  long lokMutationFreqVar;

  if (MutationFreq > 0) {
    assert ((MutationFreq/MutationFreqVar) < INT_MAX &&
	     (MutationFreq/MutationFreqVar) >= 0) ;

    MutationsThisGeneration=0;

    lokMutationFreqVar = MutationFreq +
			 Random (0, (MutationFreq/MutationFreqVar) ) ;
    // NUR zur Initialisierung von NukleotidsToPass
    if (NukleotidsToPass == 0) NukleotidsToPass = lokMutationFreqVar;
    // Alle Chromosomen von hinten nach vorne abarbeiten.
    // NukleotidsToPass zaehlt die zu passierten Nukleotide,
    // bis wieder ein Nukleotid Mutiert werden kann.
    // Ist NukleotidsToPass <= 0, so befindet sich das naechste zu
    // mutierende Nukleotid im selben Chromosom !!!
    for (int i = laenge() - 1; i >= 0; i--) {
      ChromosomLen=THIS[i].laenge();
      NukleotidsToPass-=ChromosomLen;
      while (NukleotidsToPass <= 0) {
	Chromosom NewFitness(*this);
	// NukleotidsPassedInChromosom gibt die Stelle des zu Mutierenden
	// Nukleotids an.
	assert ((ChromosomLen+NukleotidsToPass) < INT_MAX &&
		(ChromosomLen+NukleotidsToPass) >= 0) ;
	NukleotidsPassedInChromosom = (int)(ChromosomLen+NukleotidsToPass);
	// Die Move-Mutation.
	int Rand;
/*
	// Ungewichtete Mutation
	while ((Rand = Random (0, THIS[i].laenge()))
		  == NukleotidsPassedInChromosom-1);
	THIS[i].fuegeEin (THIS[i][NukleotidsPassedInChromosom-1], Rand);
	if (Rand > NukleotidsPassedInChromosom)
	  THIS[i].loesche (NukleotidsPassedInChromosom-1);
	else
	  THIS[i].loesche (NukleotidsPassedInChromosom);
*/
	// Gewichtete Mutation
	// MutationsStaerke proportional zur Stagnation der Besten Fitness !!!
	int MutationStrength= (int)
	                      ( (double)NoImproving * THIS[i].laenge() /
				(double)(NoImprovingCrossingOvers << 1) ) ;
	// mindestens einen...
	MutationStrength=(MutationStrength>3) ? MutationStrength : 3;
	do {
	  Rand =  Random (-MutationStrength, MutationStrength);
	} while (Rand==0);
	Rand += NukleotidsPassedInChromosom;
	int pos = ((Rand < 0) ? THIS[i].laenge()-1 + Rand
			     : ((Rand > THIS[i].laenge()-1)
			     ? 	Rand - THIS[i].laenge()
			     :  Rand));
	THIS[i].fuegeEin (THIS[i][NukleotidsPassedInChromosom-1], pos);
	(pos > NukleotidsPassedInChromosom) ?
	     THIS[i].loesche (NukleotidsPassedInChromosom-1) :
	     THIS[i].loesche (NukleotidsPassedInChromosom);

	assert (THIS[i].laenge() == 25);

	MutationsThisGeneration++;
	// Neue Fitness berechnen
	NewFitness = THIS[i];
	NewFitness.Splicing();
	THIS[i].SetFitness (Fitness (NewFitness));
	// Neuberechnung der zu passierenden Nukleotide bis zur naechsten
	// Mutation !
	NukleotidsToPass =
	  lokMutationFreqVar - ChromosomLen + NukleotidsPassedInChromosom ;
      }
    }
  }
}

int SalesChromosomen::Echo()
{
  if (Generation == 1 )
	printf(" Generation: BestGeneration: AverageDistance: BestDistance:"
	       " WorstDistance:\n");

  printf("\r%11d%16d%16ld%16ld%16ld",
    GetGeneration(), TheBestEversGeneration,
    (long)(GetWorstDistance()-GetAverageFitness()*GetWorstDistance()),
    (long)GetBestDistance(), (long)GetWorstDistance() );
  
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

int SalesChromosomen::CalcWholeFitness (void)
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
    if ((TempFitness = THIS[i].GetFitness())>2*std::numeric_limits<double>::epsilon()) {
        ChromBetterZeroNumber++ ;
    }
    Total          += TempFitness ;
    if (GetBestDistance() > .5 + WorstDistance - TempFitness * WorstDistance
        || GetBestDistance()<0.0 )
    {
      BestFitness=TempFitness;
      BestChrom=i;
      if (! (TheBestEver == THIS[BestChrom])) {
	TheBestEver=THIS[BestChrom];
	TheBestEversGeneration = Generation;
	TheBestEver.Splicing();
	Flag |= 2;	// NoImproving to ZERO
      }
      BestDistance = WorstDistance - TempFitness * WorstDistance;
    }
  }
  AverageFitness = Total/laenge();
  FitnessSum = Total;
  ChromosomenLenAvrg = (double)ChromLenSum/(double)laenge();

  return 1;
}

void SalesChromosomen::Protokoll()
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

	 fprintf (fileptrPtk, "\nBest Distance                         : %10.0lf\n",
	     GetBestDistance());
	   fprintf (fileptrPtk, "Average Distance                      : %10ld\n",
	   (long)(GetWorstDistance()-GetAverageFitness()*GetWorstDistance()));
	   fprintf (fileptrPtk, "Worst Distance                        : %10.0lf\n",
		GetWorstDistance());
	 fprintf (fileptrPtk, "\nMutationen dieser Generation          : %3d\n",
	    MutationsThisGeneration);
	   fprintf (fileptrPtk, "Inversionen dieser Generation         : %3d\n",
	    InversionsThisGeneration);
	   fprintf (fileptrPtk, "Translokationen dieser Generation     : %3d\n",
	    TranslocationsThisGeneration);
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

