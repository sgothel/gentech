/*
 * Author: Sven Gothel <sgothel@jausoft.com>
 * Copyright (c) 1994-2022 Gothel Software e.K.
 * Copyright (c) 1994 Christian Mueller
 *
 * Proprietary licenses are available
 * via Gothel Software e.K. <sgothel@jausoft.com>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA  02111-1307, USA.
 *
 * You can also check the GNU General Public License
 * in the internet at http://www.gnu.org/copyleft/gpl.html !
 */

#ifndef JAU_GENTECH_H
#define JAU_GENTECH_H

#include <string>
#include <gentech/version.hpp>

#include <jau/darray_sorted.hpp>
#include <jau/debug.hpp>

namespace jau::gentech {

    typedef uint8_t nucleotide_t; // muss ein ATOM'arer Typ sein ....

    typedef uint_fast32_t gen_size_type;

    typedef jau::darray<nucleotide_t, gen_size_type /* size_type */> nucleotide_array_t;

    typedef jau::darray<gen_size_type, gen_size_type /* size_type */> indexlist_t;

    typedef jau::darray_sorted<gen_size_type, gen_size_type /* size_type */> crosspoints_t;

    typedef jau::darray_sorted<double> doubles_sorted_t;

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
    // von Nukleotide sein

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
    // der Start und Ende Sequenzen ohne die Trennenden NULL'en.
    // Diese 'Length' ist entweder vom Benutzer einzusetzten,
    // oder aber gleich NULL zu setzen, wobei Chromosom selbst 'Length'
    // berechnet (DIE NULL am Ende der Start und Ende Sequenz nicht vergessen).
    // Ist der Zeiger 'SpliceCodeInfo', oder der elementzeiger 'SpliceCode',
    // oder die Nukleotideanzahl gleich NULL,
    // so signalisiert der Benutzer die Nichtverwendung des Spleissens
    // i.d. Chromosomen Klasse.

    typedef struct {
        gen_size_type Length;
        nucleotide_t *SpliceCode;
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
    // vollstaendig definiert

    // Die Basis - Klasse Chromosom
    // Es koennen der Standard Copykonstruktor und Zuweisungsoperator
    // verwendet werden, da Chromosom keine Elemente auf den Heap ablegt [5]
    //
    // Das Speicherformat ist Binaer und wie folgt sequentiell gegliedert :
    //   1.) int laenge d. Chromosoms
    //   2.) NukleoTyp element1, element2,.....

    class Chromosomen;     // Vorrausdeklaration einer friend class

    class Chromosom : public nucleotide_array_t {
        public:
            // Der Copykonstruktor ...
            Chromosom(const Chromosom& a )
            : nucleotide_array_t(a), m_env(a.m_env) { Copy (a); }

            // Der Konstruktor fuer die Konstruktion eines Chromosomes
            Chromosom( Chromosomen & env, size_type initial_length=0 ) ;

            // Der Konstruktor zum einlesen eines gespeicherten Chromosomes
            // Die Fitness wird hier nicht berechnet
            Chromosom( Chromosomen & env, const std::string& FileName ) ;

            // Der Destruktor ...
            virtual ~Chromosom() noexcept = default;

            // Der Zuweisungsoperator
            Chromosom& operator=(const Chromosom& );

            // Der Vergleichsoperator
            bool operator==(const Chromosom&) const noexcept;

            // Die Funktion zum speichern eines Chromosomes
            void Save( const std::string& FileName ) const;

            // Returns the nucleotide_number from the nuckleotide_value @ nuckleotide_idx.
            size_type GetNukleotidNumber(size_type nuckleotide_idx) const noexcept;

            // Berechnet aus der Nukleotid-Nummer, 'i' ist hier der Index dafuer,
            // fuer die Chromosomen-Schicht die UserNukleotidValue.
            nucleotide_t GetUserNukleotidValue(size_type i) const noexcept;

            // Such im Chromosm innerhalb [von..bis] nach ein Intron und liefere
            // die Intronposition [von..bis] zurueck.
            bool FindIntron(size_type &von, size_type &bis) const noexcept;

            // Aus dem Chromosom werden die Introns herausgefiltert.
            // RETURN : Anzahl der herausgefilterten Nukleotide.
            size_type Splicing();

            // Ein Intron wird eingesetzt gemaess des SpliceCodes.
            // Der Rumpf eines Introns besteht zur Zeit aus Zufaellig gewaehlte
            // Zahlen Die Laenge muss >= der Laenge der Spleiss-Sequenz haben.
            void InsertIntrons( size_type von, size_type length );

            // Ein zufaellig gewaehltes Exon, bzw. Genabschnitt wird umgekehrt.
            void Inversion();

            // Ein zufaellig gewaehltes Exon, bzw. Genabschnitt wird umpositioniert.
            void Translocation();

            void SetFitness(double Fitness) noexcept { Chromosom::m_fitness=Fitness; }

            double GetFitness() const noexcept { return m_fitness; }

            // Ausgabe-Funktion
            virtual void Ausgabe(std::ostream&) const noexcept;

        private:
            Chromosomen & m_env;
            double m_fitness;

            void Copy(const Chromosom& ) ;
            void Load(const std::string& FileName);
    };

    typedef jau::darray<Chromosom, gen_size_type /* size_type */> chromosom_array_t;

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
    //               Hier: nach der naechste Reifeteilung das 9000. Chromosom
    //
    // Diese Umschichtung ist vor allem mit Spleiss-Code [2], d.h.
    // gestueckelten Genen, siehe oben, interessant. Hierbei wird ein
    // ganzes Exon verschoben, manchmal sogar ueberlappend
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
    // verwendet werden, da Chromosom keine Elemente auf den Heap ablegt [5]
    //
    // Das Speicherformat ist Binaer und wie folgt sequentiell gegliedert :
    //   1.) int anzahl d. Chromosomen
    //     2.) int laenge d. Chromosom1
    //     3.) NukleoTyp element1_chromosom1, element2_chromosom1,.....,
    //     4.) int laenge d. Chromosom2
    //     5.) NukleoTyp element1_chromosom2, element2_chromosom2,.....,
    //     etc........

    class Chromosomen : protected chromosom_array_t {
        friend class Chromosom;

        public:
            using chromosom_array_t::size_type;

            /**
             * Special value representing maximal value of size_type, used to denote an invalid index position return value, i.e. `no position`.
             */
            static constexpr const size_type npos = std::numeric_limits<size_type>::max();


            // Dieser Konstruktor liest die StartChromosomen aus einem File.
            // Die Fitness der einzelnen Chromosomen, und AverageFitness, etc...
            // kann hier noch nicht definiert werden (compiler ?!)
            Chromosomen (nucleotide_t UserNukleoMinVal, nucleotide_t UserNukleoMaxVal,
                    size_type MaxChromosomen,
                    const std::string& StartGenFile,
                    size_type Nukleotide=4,
                    SpliceCodeInfo *PtrSpliceCodeInfo=nullptr,
                    size_type InversionFreq=0,
                    size_type TranslocationFreq=0,
                    size_type AsymXOverFreq=0,
                    size_type CrossVal=1,
                    size_type MutationFreq=10000, // [1]
                                                      size_type max_no_improving_gens=100
            );

            // Dieser Konstruktor konstruiert die StartChromosomen
            // zufaellig.
            // Die Fitness der einzelnen Chromosomen, und AverageFitness, etc...
            // kann hier noch nicht definiert werden (compiler ?!)
            Chromosomen (nucleotide_t UserNukleoMinVal, nucleotide_t UserNukleoMaxVal,
                    size_type MaxChromosomen,
                    size_type init_chromosom_count,
                    size_type init_chromosom_len,
                    size_type Nukleotide=4,
                    SpliceCodeInfo *PtrSpliceCodeInfo=nullptr,
                    size_type InversionFreq=0,
                    size_type TranslocationFreq=0,
                    size_type AsymXOverFreq=0,
                    size_type CrossVal=1,
                    size_type MutationFreq=10000, // [1]
                                                      size_type max_no_improving_gens=100
            );

            // Der Destruktor ...
            virtual ~Chromosomen() = default ;

            // Die Chromosomen abspeichern..
            void Save( const std::string& fname ) const;

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
            virtual bool Echo() const;

            // Evolution ...
            // DARF NICHT VON EINEM KONSTRUKTOR AUFGERUFEN WERDEN!
            // ... mit Gesamtfitness-Berechnung & Mutationen
            // GoalFitness              : ZielFitness ...
            // BirthRate                : Die Geburtenrate fuer die MixedGeneration
            //                            Ist BirthRate==1.0, so besteht die Nachwelt
            //                            nur aus Nachkommen.
            //                            BirthRate ca. 0.6 => 60 % ist ok.
            // chrptrPtkFile            : Das Protokollfile ...
            // return                   : Die Nummer der EndGeneration
            virtual size_type Evolution(double GoalFitness, const std::string& chrptrPtkFile,
                                        double BirthRate=0.6, int Bigamie=0);

            // Die Mittlere-Fitness der Generation einholen ....
            double GetAverageFitness() const { return m_avrg_fitness; }

            // Die Fitness-Summe der Generation einholen ....
            double GetFitnessSum() const { return m_fitness_sum; }

            // das Indize des Chromosomes mit der schlechtesten Fitness einholen.
            size_type GetWorstChromosom() const ;

            // die X-WorstFitness herausfinden.
            // Uebergeben wird, die wievielte WorstFitness herausgefunden werden soll
            // Zurueckgegeben wird der Wert der X-WorstFitness
            double GetXWorstFitness(size_type) const ;

            // das Indize des Chromosomes mit der besten Fitness einholen.
            size_type GetBestChromosom() const ;

            // Die Generationsnummer einholen.
            size_type GetGeneration() const { return m_generation; }

            double GetTheBestEversAverageFitness() const
            { return m_all_time_best_avrg_fitness; }

            // das Beste Chromosom Ueberhaupt ... ge-splice't
            const Chromosom &GetTheBestEverChromosom();

            virtual void Ausgabe(std::ostream&) const noexcept;

            // Vom Benutzer zu Definieren :
            // Fitness fuer das uebergebene Chromosom.
            // Wert [0..1] !
            virtual double Fitness(const Chromosom&) = 0 ;

            // Schnittstelle zu 'private' Elemente :
            nucleotide_t GetUserNukleoMinVal() { return m_min_nucleotide_value; }
            nucleotide_t GetUserNukleoMaxVal() { return m_max_nucleotide_value; }

            size_type GetNukleotide() { return m_nucleotide_count; }

            SpliceCodeInfo *GetPtrSpliceCodeInfo() { return m_splice_code_ptr; }

        protected:
            // Protokolliere
            virtual void Protokoll() ;

            // Sucht ein Chromosom aus. Nach dem Rouletteprinzip [4] !
            virtual size_type RouletteSelect() const;

            // die Gesamtfitness ...
            virtual void CalcWholeFitness() ;

            double GetBestFitness() const { return m_best_fitness; };

            // Erste Fitness-Initialisierung fuer Evolution,
            // da der Konstruktor die virtuale Fitnessfunktion noch nicht kennt !
            virtual void InitFitness() ;

            // Erzeugt Nachkommen und gliedert sie am Ende d. Chromosomen-Liste ein
            // incl. Fitnessberechnung, aber ohne Gesamtfitnessberechnung
            virtual void CrossingOver (size_type m, size_type w);

            // die Mutationen ... [1] eines Nukleotids ...
            virtual void Mutation();

            // die Mutationen ... [2] eines Chromosomenabschnittes
            virtual void InversionsMutation();

            // die Mutationen ... [6] eines Chromosomenabschnittes
            virtual void TranslocationsMutation();

            // Die Neue Generation Besteht aus Kindern und fast guten Eltern ... [1]
            // ... mit Gesamtfitness & Mutationen
            virtual void NewGeneration (double BirthRate, bool Bigamie) ;

            // Die Neue Generation Besteht nur aus Kindern .... [4]
            // ... mit Gesamtfitness & Mutationen
            virtual void NewGeneration (bool Bigamie) ;

            // Der Funktion LetDie wird die Fitness uebergeben, ab der 'Chromosom'-en
            // mit einer schlechteren Fitness ausselektiert werden.
            virtual void LetDie (double cut);

            // Diese Funktion existiert nur zum Ueberladen bei kuenftigen
            // Nachfahren
            virtual void Kill(const size_type i) { erase(i); }

            // mittels der 'CrossPoints' in 'dest' EINE Kreuzung aus 'm' & 'w'
            // erzeugen
            // Fuer das 2.Kind muss die Routine mit vertauschten Geschlechtern
            // aufgerufen werden.
            // 'm'      : Indize des maennlichen Chromosoms
            // 'w'      : Indize des weiblichen Chromosoms
            virtual void CreateNewSymChromosom ( Chromosom &dest, size_type m, size_type w,
                    crosspoints_t &CrossPoints ) ;

            // Berechnen einiger Parameter ....
            void CalcParameter() ;

            // STATISTISCHE VARIABLEN FUER DAS PROTOKOLL
            size_type m_all_time_best_gen;
            double m_all_time_best_avrg_fitness;
            Chromosom m_all_time_best;

            double m_avrg_fitness;
            double m_best_fitness;
            double m_fitness_sum;

            size_type m_max_no_improving_gens;
            size_type m_no_improving_gens;

            size_type m_generation;
            size_type m_max_chromosom_count;
            size_type m_intro_code_len_sum;
            size_type m_spliced_chromosom_count;

            FILE *m_file_ptk_ptr;

            // Die Zeiten...
            time_t m_generation_start;
            time_t m_generation_end;
            time_t m_evolution_start;
            time_t m_evolution_end;

            // STATISTISCHE VARIABLEN FUER DAS PROTOKOLL
            size_type m_min_chromosom_len;
            size_type m_max_chromosom_len;
            double m_avrg_chromosom_len;
            // Speichert Anzahl der Chromosomen, deren Fitness groesser Null ist
            size_type m_chromosom_above_zero_count;

            // STATISTISCHE VARIABLEN FUER DAS PROTOKOLL
            size_type m_mutations_this_gen;
            size_type m_inversions_this_gen;
            size_type m_translocations_this_gen;

            // FIXME: Change to percentages, allowing proportional varying to MutationFreq
            static const size_type m_mutation_freq_variance;
            size_type m_mutation_freq;
            size_type m_inversion_freq;
            size_type m_translocation_freq;
            size_type m_asymxover_freq;
            size_type m_cross_val;
            size_type m_xover_number;

            SpliceCodeInfo *m_splice_code_ptr;
            nucleotide_t m_min_nucleotide_value;
            nucleotide_t m_max_nucleotide_value;
            nucleotide_t m_nucleotide_value_scale;
            size_type m_nucleotide_count;
    };

} /* namespace jau::gentech */

namespace std {
    std::ostream& operator<< (std::ostream& os, const jau::gentech::nucleotide_array_t& list);
}

#endif
