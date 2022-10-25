// salesgen.h							 6.3.1994
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

# ifndef _SALESGEN_H
  # define _SALESGEN_H

  # include <stdio.h>
  #ifdef __MSDOS__ 
	  # include <conio.h>
  #endif
  # include "gentech.h"
  # include "sales.h"
  # include "maschine.h"
  # include "random.h"

  class SalesChromosomen ;

  class SalesChromosom : public Chromosom {

  public:
    // Der Copykonstruktor ...
    SalesChromosom (const Chromosom& a ) 
    : Chromosom(a) {}

    // Der Konstruktor fuer die Konstruktion eines Chromosomes
    SalesChromosom ( SalesChromosomen & env,
		int StartChromosomLength=0
	      ) ;

    // Der Konstruktor zum einlesen eines gespeicherten Chromosomes !
    // Die Fitness wird hier nicht berechnet !!!
    SalesChromosom ( SalesChromosomen & env,
		char *FileName
	      );
 };

  class SalesChromosomen : public Chromosomen {
  
  friend class SalesChromosom;

  public:
    // Der Konstruktor mit allen Einstellungen zum Loesen des Problems.
    SalesChromosomen( int Monk, int Cannibal,
		  int MaxChromosomen,
		  int StartChromosomNumber,
		  int StartChromosomLength,
		  int Nukleotide=3,
		  SpliceCodeInfo *ptrSpliceCodeInfo=NULL,
		  long InversionFreq=0,
		  long TranslocationFreq=0,
		  long AsymXOverFreq=0,
		  int CrossVal=1,
		  long MutationFreq=10000, // [1]
		  int NoImprovingCrossingOvers=100
		) ;
    SalesChromosomen( int Monk, int Cannibal,
		  int MaxChromosomen,
		  char *StartGenFile,
		  int Nukleotide=3,
		  SpliceCodeInfo *ptrSpliceCodeInfo=NULL,
		  long InversionFreq=0,
		  long TranslocationFreq=0,
		  long AsymXOverFreq=0,
		  int CrossVal=1,
		  long MutationFreq=10000,
		  int NoImprovingCrossingOvers=100
		) ;
    // Die Protokoll-Funktion zur Laufzeit
    virtual int Echo (void);

    // Protokolliere
    void Protokoll ( void ) ;

    // Beschreibung der Parameter s. "gentech.h"
    virtual int Evolution (
			    double GoalFitness, char *chrptrPtkFile=NULL,
			    double BirthRate=0.6, int Bigamie=0
			  );
    // Fitness fuer das uebergebene Chromosom.
    // Wert [0..1] !
    double Fitness (const Chromosom&);
    int CalcWholeFitness ( void ) ;
    double GetWorstDistance (void) const	{ return WorstDistance; }
    double GetBestDistance (void)  const	{ return BestDistance; }

    // Und das Spiel selbst als Element ...
    SalesGame TheGame;

  protected:
    // Erste Fitness-Initialisierung fuer Evolution,
    // da der Konstruktor die virtuale Fitnessfunktion noch nicht kennt !
    void InitFitness(void)
    {
      AssignWorstDistance();
      Chromosomen::InitFitness();
    }

    // Ueberladen der SterbeFunktion. Ausser dem Loeschen des 'Chromosom's
    // wird noch ueberpreft, ob dieses Chromosom die Loesung der laengsten
    // Wegstrecke darstellt
    void Kill (int i)
    {
      if ((*this)[i].GetFitness() < MASCHINE::eps)
	Flag |= 1;	// Chromosom mit WorstDistance wird geloescht
      loesche (i);
    }

    virtual int CrossingOver (int m, int w);

  int Mutation (void);
  int NoImproving;
  int NoImprovingCrossingOvers;

    virtual int CreateNewSymChromosom ( Chromosom &dest, int m, int w,
				        SortListe<int> &CrossPoints
			              ) ;
    // Zeigt im 0-ten Bit, ob sich WorstDistance veraendert hat.
    // Dann muss die Fitness der gesammten Population neu berechnet
    // werden
    long Flag;
    // Speichert die Laenge der laengsten Strecke
    double WorstDistance;
    // Speichert die Laenge der kuerzesten Strecke
    double BestDistance;
    // Bestimmen des laengsten geschlossenen Pfades
    void AssignWorstDistance (void)
    {
      int i = 0;
      double Distance;

      // Variablen zuruecksetzen
      WorstDistance = 0;
      // und neu berechnen
      while (i < laenge())	{
	if ((Distance = TheGame.Play ((*this)[i], FALSE)) > WorstDistance)
	    WorstDistance = Distance;
	i++;
      }
      Flag &= ~1;	// 0-te Bit wird geloescht
    }

  };

# endif

