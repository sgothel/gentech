// gentech.h                                                    18.2.1994
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
//
// LESEN SIE AUCH 'GENTECH.DOC' im ASCII-Format !!!

# ifndef _GENTECH_H
  # define _GENTECH_H

  # ifdef  __BORLANDC__
    # include <conio.h>
  # endif
  # include <assert.h>
  # include <stdlib.h>
  # include <stdio.h>
  # include <time.h>
  # include <ctype.h>
  # include "bool.h"
  # include "liste.h"
  # include "menge.h"
  # include "std.h"
  # include "sortlist.h"
  # include "maschine.h"
  # include <limits.h>

  extern char GenTechVersion[];

  // THIS Makro, um leichter zu dereferenzieren.
  # define THIS (*this)

  typedef unsigned char NukleoTyp; // muss ein ATOM'arer Typ sein ....
  

  // Ein Nukleotid soll mittels Integer dargestellt werden.
  // Es bleibt dem Benutzer ueberlassen, wie gross der Wertebereich
  // eines Nukleotids sein soll.
  // Dies ist der Benutzer-Nukleotid-Wertebereich,
  // User-Nukleotid-Value-Scale ( [UserNukleoMinVal..UserNukleoMaxVal] ).
  //
  // Der Benutzer-Nukleotid-Wertebereich ist eine Obermenge des
  // Chromosomen-Wertebereichs. Zum Beispiel ist der natuerliche
  // Chromosomen-Wertebereichs von 1 bis 4 gueltig ( [1..4] ), analog zu
  // den vier Basen Adenin, Guanin, Cytosin, Thymin.
  //
  // Der Benutzer kann fuer die Chromosomen-Schicht die Anzahl
  // der verschiedenen Nukleotide (Basen) selbst bestimmen.
  // Somit ist er in der Lage auf Mechanismen waehrend der Reifeteilung,
  // Meiose, Einfluss zu nehmen.
  // Diese Mechanismen werden in der Chromosomen-Schicht abgewickelt und
  // sind hier vollstaendig in der Klasse 'Chromosomen' implementiert.
  //
  // Die Bestimmung der Nukleotide fuer die Chromosomen-Schicht werden
  // wie folgt bestimmt :
  //    w := ( UserNukleoMaxVal - UserNukleoMinVal ) / Nukleotide
  //    IF  w*0 <= Wert AND Wert <w*1 THEN 1. Nukleotid ; etc....
  //
  // Der User-Nukleotid-Wertebereich sollte ein ganzes Vielfaches
  // von Nukleotide sein !!!

  // In der Natur gibt es sogenannte zerstueckelte Gene [2].
  // D.h. Gene aufgeteilt in Exons, Genabschnitt, und Introns,
  // sogenannte 'Lueckenfueller'.
  // Um daraus wiederum vollstaendige Gene zu gestalten,
  // muss das Chromosom gespleisst werden, d.h. die Introns zu entfernen.
  // Fuer das Spleissen gibt es eine sogenannte Spleiss-Sequenz.
  // I.d. Natur beginnt das Intron i.d. Regel mit Guanin und Uracil,
  // Uracil ist das RNA Pedant zu Thymin, und endet mit Adenin und Guanin.
  //
  // Der Benutzer kann selber diese Spleiss-Sequenz bestimmen,
  // bzw. ganz weglassen ( wie z.B. kernlosen Zellen... ).
  // Hierfuer kann er einen Zeiger auf die unten deklarierte
  // 'SpliceCodeInfo' Struktur uebergeben.
  // Das darin enthaltene Array 'SpliceCode' enthaelt die Start und die
  // Ende Sequenz eines Introns.
  // Start und Ende Sequenz werden mit einer NULL terminiert.
  // Die Sequenzen geben die Reihenfolge der Nummer (>0)
  // des gewuenschten Nukleotids an, die im Chromosom zu finden sein soll
  // um selbiege Sequenz zu erkennen.
  // Der UserNukleotid-Wert des Chromosomes wird in die entsprechende
  // Nukleotid-Nummer umgerechnet und dann verglichen.
  // Die 'Length' des 'SpliceCode' in 'SpliceCodeInfo' ist die Summe
  // der Start und Ende Sequenzen ohne die Trennenden NULL'en !!!!!!
  // Diese 'Length' ist entweder vom Benutzer einzusetzten,
  // oder aber gleich NULL zu setzen, wobei Chromosom selbst 'Length'
  // berechnet (DIE NULL am Ende der Start und Ende Sequenz nicht vergessen).
  // Ist der Zeiger 'SpliceCodeInfo', oder der elementzeiger 'SpliceCode',
  // oder die Nukleotideanzahl gleich NULL,
  // so signalisiert der Benutzer die Nichtverwendung des Spleissens
  // i.d. Chromosomen Klasse.

  typedef struct {
    int Length;
    NukleoTyp *SpliceCode;
  } SpliceCodeInfo;


  // Die Fitness-Funktion ist eine ganz virtuelle Funktion
  // und ist vom Benutzer fuer die jeweilige Anwendeung der Chromosomen
  // zu Programmieren. Sie hat einen normierten Wert zwischen 0 und 1
  // ( [0..1] ) zurueckzugeben.

  // Literaturverzeichniss :
  //
  // [1] Holland, John H. : Genetische Algorithmen,
  //     Spektrum der Wissenschaft, September 1992, S.44-51
  // [2] Chambon, Pierre  : Gestueckelte Gene - ein Informations-Mosaik,
  //     Spektrum der Wissenschaft, Juli 1981, S.104-117
  // [3] Dewdney, A.K.    : Genetische Algorithmen,
  //     Computer-Kurzweil, Spektrum-der-Wissenschaft-Verlagsgesellschaft
  //     1988, ISBN 3-922508-50-2, S.68-72
  // [4] Weckwerth, Guido : Zeugende Zahlen,
  //     MC, Oktober 1993, S.54-58, Januar 1994, S.72-75
  // [5] Schader, Kuhlins : Programmieren in C++,
  //     Springer-Verlag Heidelberg 1993, ISBN 3-540-56524-8
  // [6] Schoeneburg, Eberhard : Genetische Algorithmen und Evolutionsstrategien
  //     Addison-Wesley 1994, ISBN 3-89319-495-2

  // Die Chromosomen-Schicht ist mit den Klassen 'Chromosom' und 'Chromosomen'
  // vollstaendig definiert !!!

  // Die Basis - Klasse Chromosom
  // Es koennen der Standard Copykonstruktor und Zuweisungsoperator
  // verwendet werden, da Chromosom keine Elemente auf den Heap ablegt [5] !!!
  //
  // Das Speicherformat ist Binaer und wie folgt sequentiell gegliedert :
  //   1.) int laenge d. Chromosoms
  //   2.) NukleoTyp element1, element2,.....

  class Chromosomen;     // Vorrausdeklaration einer friend class



  class Chromosom : public virtual Liste<NukleoTyp> {

  friend ostream& operator << (ostream& OS, Liste<NukleoTyp>&);

  public:
    // Der Copykonstruktor ...
    Chromosom (const Chromosom& a ) 
    : env(a.env), Liste<NukleoTyp>(a) { Copy (a); }

    // Der Konstruktor fuer die Konstruktion eines Chromosomes
    Chromosom ( Chromosomen & env,
		int StartChromosomLength=0
	      ) ;

    // Der Konstruktor zum einlesen eines gespeicherten Chromosomes !
    // Die Fitness wird hier nicht berechnet !!!
    Chromosom ( Chromosomen & env,
		char *FileName
	      ) ;

    // Der Destruktor ...
    virtual ~Chromosom (void) {}

    // Der Zuweisungsoperator
    Chromosom& operator=(const Chromosom& );

    // Der Vergleichsoperator
    int operator==(const Chromosom&) const;

    // Die Funktion zum speichern eines Chromosomes !
    int Save ( char *FileName ) const;

    // Berechnet aus der UserNukleotidValue, 'i' ist hier der Index dafuer,
    // die Nukleotid-Nummer fuer die Chromosomen-Schicht.
    int GetNukleotidNumber (int i) const;

    // Berechnet aus der Nukleotid-Nummer, 'i' ist hier der Index dafuer,
    // fuer die Chromosomen-Schicht die UserNukleotidValue.
    NukleoTyp GetUserNukleotidValue (int i) const;

    // Such im Chromosm innerhalb [von..bis] nach ein Intron und liefere
    // die Intronposition [von..bis] zurueck.
    int FindIntron (int &von, int &bis) const;

    // Aus dem Chromosom werden die Introns herausgefiltert.
    // RETURN : Anzahl der herausgefilterten Nukleotide.
    virtual int Splicing (void) ;

    // Ein Intron wird eingesetzt gemaess des SpliceCodes.
    // Der Rumpf eines Introns besteht zur Zeit aus Zufaellig gewaehlte
    // Zahlen !!! Die Laenge muss >= der Laenge der Spleiss-Sequenz haben.
    virtual int InsertIntrons ( int von, int length );

    // Ein zufaellig gewaehltes Exon, bzw. Genabschnitt wird umgekehrt.
    virtual int Inversion (void) ;

    // Ein zufaellig gewaehltes Exon, bzw. Genabschnitt wird umpositioniert.
    virtual int Translocation (void) ;

    void SetFitness (double Fitness) { Chromosom::Fitness=Fitness; }

    double GetFitness (void) const { return Fitness; }

    // Ausgabe-Funktion
    virtual void Ausgabe (ostream&);

    Chromosomen & env;
    double Fitness;

  private:

    void Copy ( const Chromosom& ) ;
  };

  // Waehrend der Reifeteilung, Meiose, geschehen folgende Dinge :
  //    o Evt. Umschichtung(Inversion[2]/Translocation[6]) der Elterngene fuer
  //    o Crossing Over ( symetrisch [1,2,3,4] / asymetrisch [2] )
  //    o Mutation [1,3,4]
  //
  // Die Haeufigkeit der Umschichtung [2] (Inversion/Translocation) wird
  // mittels der Inversion/TranslocationFreq(uency) angegeben.
  // Dieser Wert ist die Umschichtungsfrequenz der Chromosomenpopulation.
  // Die Umschichtung erfolgt nach der Reifeteilung.
  // D.h.: z.B. Chromosomenpopulation = 11000, Umschichtungsfrequenz = 10000
  //            => Das 10000. Chromosom wird umgeschichtet.
  //            => Das naechste 10000. Chromosom wird umgeschichtet
  //               Hier: nach der naechste Reifeteilung das 9000. Chromosom !
  //
  // Diese Umschichtung ist vor allem mit Spleiss-Code [2], d.h.
  // gestueckelten Genen, siehe oben, interessant. Hierbei wird ein
  // ganzes Exon verschoben, manchmal sogar ueberlappend !!!!
  //
  // Mittels Asymmetrischen [2] Crossing Overs wird nur ein Nachfolger erzeugt.
  // Derart, das alle Gene im Nachfolger verdoppelt sind.
  //
  // Die Haeufigkeit des asymetrischen Crossing Overs wird mittels der
  // AsymXOverFreq(uency) angegeben. Dieser Wert gibt an, nach wievielen
  // Reifeteilungen ein asymetrisches Crossing Over ausgefuehrt werden soll.
  //
  // Die Anzahl der Schnittpunkte beim Crossing Over wird mit CrossVal
  // angegeben. In der Regel 1 x .
  //
  // Die Haeufigkeit der Mutationen wird mittels MutationFreq(uency)
  // angegeben. Der Wert gibt an, nach wievielen gereiften/kopierten
  // Nukleotiden ein Nukleotid einen zufaelligen Wert im Wertebereichs
  // des Benutzer annimmt. '0' bedeutet keine Mutation.
  //
  // Die abstrakte (Basis-) Klasse Chromosomen
  // Es koennen der Standard Copykonstruktor und Zuweisungsoperator
  // verwendet werden, da Chromosom keine Elemente auf den Heap ablegt [5] !!!
  //
  // Das Speicherformat ist Binaer und wie folgt sequentiell gegliedert :
  //   1.) int anzahl d. Chromosomen
  //     2.) int laenge d. Chromosom1
  //     3.) NukleoTyp element1_chromosom1, element2_chromosom1,.....,
  //     4.) int laenge d. Chromosom2
  //     5.) NukleoTyp element1_chromosom2, element2_chromosom2,.....,
  //     etc........

  class Chromosomen : protected virtual Liste<Chromosom> {

  friend class Chromosom;

  public:

    // Dieser Konstruktor liest die StartChromosomen aus einem File.
    // Die Fitness der einzelnen Chromosomen, und AverageFitness, etc...
    // kann hier noch nicht definiert werden (compiler ?!) !!!
    Chromosomen (NukleoTyp UserNukleoMinVal, NukleoTyp UserNukleoMaxVal,
		  int MaxChromosomen,
		  char *StartGenFile,
		  int Nukleotide=4,
		  SpliceCodeInfo *PtrSpliceCodeInfo=0,
		  long InversionFreq=0,
		  long TranslocationFreq=0,
		  long AsymXOverFreq=0,
		  int CrossVal=1,
		  long MutationFreq=10000 // [1]
		 );

    // Dieser Konstruktor konstruiert die StartChromosomen
    // zufaellig.
    // Die Fitness der einzelnen Chromosomen, und AverageFitness, etc...
    // kann hier noch nicht definiert werden (compiler ?!) !!!
    Chromosomen (NukleoTyp UserNukleoMinVal, NukleoTyp UserNukleoMaxVal,
		  int MaxChromosomen,
		  int StartChromosomNumber,
		  int StartChromosomLength,
		  int Nukleotide=4,
		  SpliceCodeInfo *PtrSpliceCodeInfo=0,
		  long InversionFreq=0,
		  long TranslocationFreq=0,
		  long AsymXOverFreq=0,
		  int CrossVal=1,
		  long MutationFreq=10000 // [1]
		) ;

    // Der Destruktor ...
    virtual ~Chromosomen ( void ) {} ;

    // Die Chromosomen abspeichern..
    int Save ( char* ) const;

    // Ein vom Benutzer evt. zu selbst definierendes Echo....
    // Dies wird nach jeder neu generierten Population aufgerufen.
    // Der Benutzer kann sich somit gewuenschte Informationen holen,
    // wie sich auch vor allem von einem 'nichtabsturz'
    // des Rechners ueberzeugen und den Optimierablauf unterbrechen.
    // Ist der Rueckgabewert 1, so wird fortgefahren.
    // Ist der Rueckgabewert 0, so wird abgebrochen.
    // Die hier implementierte Funktion gibt auf 'stdout' die
    // Generationsnummer, die best Fitness und die average Fitness aus.
    // Dabei wird die ESC Taste abgefragt, falls abgebrochen werden soll
    // und falls __MSDOS__ oder __BORLANDC__ definiert wurden.
    virtual int Echo ( void ) const;

    // Evolution ...
    // DARF NICHT VON EINEM KONSTRUKTOR AUFGERUFEN WERDEN !!!!
    // ... mit Gesamtfitness-Berechnung & Mutationen
    // GoalFitness              : ZielFitness ...
    // BirthRate                : Die Geburtenrate fuer die MixedGeneration
    //                            Ist BirthRate==1.0, so besteht die Nachwelt
    //                            nur aus Nachkommen.
    //                            BirthRate ca. 0.6 => 60 % ist ok.
    // NoImprovingCrossingOvers : Die Anzahl der Generationen,
    //                            bei denen keine durchschnittliche
    //                            Verbesserung der Nachkommen eintrat,
    //                            als Abbruchbedingung !
    // chrptrPtkFile            : Das Protokollfile ...
    // return                   : Die Nummer der EndGeneration !!!
    virtual int Evolution (
			    double GoalFitness, char *chrptrPtkFile=NULL,
			    double BirthRate=0.6, int Bigamie=0,
			    int NoImprovingCrossingOvers = 100
			  );

    // Die Mittlere-Fitness der Generation einholen ....
    double GetAverageFitness (void) const { return AverageFitness; }

    // Die Fitness-Summe der Generation einholen ....
    double GetFitnessSum (void) const { return FitnessSum; }

    // das Indize des Chromosomes mit der schlechtesten Fitness einholen.
    int GetWorstChromosom ( void ) const ;

    // die X-WorstFitness herausfinden.
    // Uebergeben wird, die wievielte WorstFitness herausgefunden werden soll
    // Zurueckgegeben wird der Wert der X-WorstFitness
    double GetXWorstFitness (int) const ;

    // das Indize des Chromosomes mit der besten Fitness einholen.
    int GetBestChromosom ( void ) const ;

    // Die Generationsnummer einholen.
    int GetGeneration ( void ) const { return Generation; }

    double GetTheBestEversAverageFitness ( void ) const
      { return TheBestEversAverageFitness; }

    // das Beste Chromosom Ueberhaupt ... ge-splice't
    const Chromosom &GetTheBestEverChromosom ( void );

    virtual void Ausgabe (ostream&);

    // Vom Benutzer zu Definieren :
    // Fitness fuer das uebergebene Chromosom.
    // Wert [0..1] !
    virtual double Fitness (const Chromosom&) = NULL ;

    // Schnittstelle zu 'private' Elemente :
    NukleoTyp GetUserNukleoMinVal(void) { return UserNukleoMinVal; }
    NukleoTyp GetUserNukleoMaxVal(void) { return UserNukleoMaxVal; }

    int GetNukleotide(void) { return Nukleotide; }

    SpliceCodeInfo *GetPtrSpliceCodeInfo(void) { return ptrSpliceCodeInfo; }

  protected:

    // Protokolliere
    virtual void Protokoll ( void ) ;

    // Sucht ein Chromosom aus. Nach dem Rouletteprinzip [4] !
    virtual int RouletteSelect ( void ) const;

    // die Gesamtfitness ...
    virtual int CalcWholeFitness ( void ) ;

    double GetBestFitness ( void ) const { return BestFitness; };

    // Erste Fitness-Initialisierung fuer Evolution,
    // da der Konstruktor die virtuale Fitnessfunktion noch nicht kennt !
    virtual void InitFitness(void) ;

    // Erzeugt Nachkommen und gliedert sie am Ende d. Chromosomen-Liste ein
    // incl. Fitnessberechnung, aber ohne Gesamtfitnessberechnung !!!
    virtual int CrossingOver (int m, int w);

    // die Mutationen ... [1] eines Nukleotids ...
    virtual int Mutation (void);

    // die Mutationen ... [2] eines Chromosomenabschnittes
    virtual int InversionsMutation (void);

    // die Mutationen ... [6] eines Chromosomenabschnittes
    virtual int TranslocationsMutation (void);

    // Die Neue Generation Besteht aus Kindern und fast guten Eltern ... [1]
    // ... mit Gesamtfitness & Mutationen
    virtual int NewGeneration (double BirthRate, int Bigamie) ;

    // Die Neue Generation Besteht nur aus Kindern .... [4]
    // ... mit Gesamtfitness & Mutationen
    virtual int NewGeneration (int Bigamie) ;

    // Der Funktion LetDie wird die Fitness uebergeben, ab der 'Chromosom'-en
    // mit einer schlechteren Fitness ausselektiert werden.
    virtual int LetDie (double cut);

    // Diese Funktion existiert nur zum Ueberladen bei kuenftigen
    // Nachfahren
    virtual void Kill (int i)	{ loesche (i); }


    // STATISTISCHE VARIABLEN FUER DAS PROTOKOLL !!!
    int TheBestEversGeneration;
    double AverageFitness;
    double TheBestEversAverageFitness;
    double BestFitness;
    double FitnessSum;
    int Generation;
    int MaxChromosomen;
    int IntroCodeLenSum;
    int SplicedChromosoms;

    Chromosom TheBestEver;

    FILE *fileptrPtk;
    // Die Zeiten...
    time_t EvolutionEnd;
    time_t GenerationStart;
    time_t EvolutionStart;
    time_t GenerationEnd;

    // STATISTISCHE VARIABLEN FUER DAS PROTOKOLL !!!
    int ChromosomenLenMin;
    int ChromosomenLenMax;
    double ChromosomenLenAvrg;
    // Speichert Anzahl der Chromosomen, deren Fitness groesser Null ist
    int ChromBetterZeroNumber;



    // STATISTISCHE VARIABLEN FUER DAS PROTOKOLL !!!
    int MutationsThisGeneration;
    int InversionsThisGeneration;
    int TranslocationsThisGeneration;
    long MutationFreq;
    static const long MutationFreqVar;

    // mittels der 'CrossPoints' in 'dest' EINE Kreuzung aus 'm' & 'w'
    // erzeugen !!!
    // Fuer das 2.Kind muss die Routine mit vertauschten Geschlechtern
    // aufgerufen werden.
    // 'm'      : Indize des maennlichen Chromosoms
    // 'w'      : Indize des weiblichen Chromosoms
    virtual int CreateNewSymChromosom ( Chromosom &dest, int m, int w,
				        SortListe<int> &CrossPoints
			              ) ;

    // Berechnen einiger Parameter ....
    void CalcParameter(void) ;

    long InversionFreq;
    long TranslocationFreq;
    long AsymXOverFreq;
    int CrossVal;
    long XOverNumber;

    SpliceCodeInfo *ptrSpliceCodeInfo;
    NukleoTyp UserNukleoMinVal;
    NukleoTyp UserNukleoMaxVal;
    NukleoTyp UserNukleoValScale;
    int Nukleotide;
  };


# endif
