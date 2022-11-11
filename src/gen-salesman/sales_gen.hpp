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

#ifndef SALES_GEN_H
  #define SALES_GEN_H

  #include <string>
  #include <limits>

  #include <gentech/gentech.hpp>
  #include "sales.hpp"

  class SalesChromosomen ;

  class SalesChromosom : public Chromosom {
      public:
          // Der Copykonstruktor ...
          SalesChromosom (const Chromosom& a )
          : Chromosom(a) {}

          // Der Konstruktor fuer die Konstruktion eines Chromosomes
          SalesChromosom ( SalesChromosomen & env, size_type StartChromosomLength=0 ) ;

          // Der Konstruktor zum einlesen eines gespeicherten Chromosomes !
          // Die Fitness wird hier nicht berechnet !!!
          SalesChromosom ( SalesChromosomen & env, const std::string& FileName );
  };

  class SalesChromosomen : public Chromosomen {
          friend class SalesChromosom;

      public:
          using Chromosomen::size_type;

          // Der Konstruktor mit allen Einstellungen zum Loesen des Problems.
          SalesChromosomen(
                  size_type MaxChromosomen,
                  size_type StartChromosomNumber,
                  size_type StartChromosomLength,
                  size_type Nukleotide=3,
                  SpliceCodeInfo *ptrSpliceCodeInfo=nullptr,
                  size_type InversionFreq=0,
                  size_type TranslocationFreq=0,
                  size_type AsymXOverFreq=0,
                  size_type CrossVal=1,
                  size_type MutationFreq=10000, // [1]
                  size_type NoImprovingCrossingOvers=100
          ) ;
          SalesChromosomen(
                  size_type MaxChromosomen,
                  const std::string& StartGenFile,
                  size_type Nukleotide=3,
                  SpliceCodeInfo *ptrSpliceCodeInfo=nullptr,
                  size_type InversionFreq=0,
                  size_type TranslocationFreq=0,
                  size_type AsymXOverFreq=0,
                  size_type CrossVal=1,
                  size_type MutationFreq=10000,
                  size_type NoImprovingCrossingOvers=100
          ) ;
          // Die Protokoll-Funktion zur Laufzeit
          bool Echo() const override;

          // Beschreibung der Parameter s. "gentech.h"
          int Evolution(double GoalFitness, const std::string& chrptrPtkFile,
                        double BirthRate=0.6, int Bigamie=0,
                        size_type NoImprovingCrossingOvers = 100 ) override;

          // Protokolliere
          void Protokoll( void ) override;

          // Fitness fuer das uebergebene Chromosom.
          // Wert [0..1] !
          double Fitness(const Chromosom&) override;
          void CalcWholeFitness() override;
          double GetWorstDistance() const { return WorstDistance; }
          double GetBestDistance()  const { return BestDistance; }

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
          void Kill(size_type i) noexcept override {
              if ((*this)[i].GetFitness() < std::numeric_limits<double>::epsilon()) {
                  Flag |= 1;	// Chromosom mit WorstDistance wird geloescht
              }
              erase(i);
          }

          void CrossingOver(size_type m, size_type w) noexcept override;

          void Mutation() override;

          size_type NoImproving;
          size_type NoImprovingCrossingOvers;

          void CreateNewSymChromosom( Chromosom &dest, size_type m, size_type w,
                  SortListe<size_type, size_type>& CrossPoints ) override;

          void validate(const Chromosom &c);

          // Zeigt im 0-ten Bit, ob sich WorstDistance veraendert hat.
          // Dann muss die Fitness der gesammten Population neu berechnet
          // werden
          long Flag;
          // Speichert die Laenge der laengsten Strecke
          double WorstDistance;
          // Speichert die Laenge der kuerzesten Strecke
          double BestDistance;
          // Bestimmen des laengsten geschlossenen Pfades
          void AssignWorstDistance () {
              size_type i = 0;
              double Distance;

              // Variablen zuruecksetzen
              WorstDistance = 0;
              // und neu berechnen
              while (i < size())	{
                  if ((Distance = TheGame.Play ((*this)[i], false)) > WorstDistance)
                      WorstDistance = Distance;
                  i++;
              }
              Flag &= ~1;	// 0-te Bit wird geloescht
          }
  };

# endif

