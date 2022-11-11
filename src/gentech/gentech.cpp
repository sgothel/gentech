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

#include <gentech/gentech.hpp>
#include <gentech/random.hpp>

#include <climits>
#include <limits>

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

Chromosom::Chromosom ( Chromosomen & env, size_type StartChromosomLength )
: Liste<NukleoTyp>(), env(env), Fitness(0)
// Zufaellig erzeugte Chromosomen
{
    for ( Chromosom::size_type i=StartChromosomLength; i > 0; --i) {
        push_back( Random(env.UserNukleoMinVal,env.UserNukleoMaxVal) );
    }
    assert (size() == StartChromosomLength);
}

Chromosom::Chromosom ( Chromosomen & env, const std::string& FileName )
: Liste<NukleoTyp>(), env(env), Fitness(0)
{
    Load(FileName);
}

void Chromosom::Copy (const Chromosom &a)
{
    Fitness = a.Fitness;
    assert (size() == a.size());
}

Chromosom& Chromosom::operator=(const Chromosom& m)
{
    if (this == &m) { return *this; }

    // Neue Liste der Nukleotide anlegen.
    // In Liste<NukleoTyp> wird der *this-Zeiger auf Gleichheit ueberprueft
    // Das Environment env (Chromosomen) bleibt !
    Liste<NukleoTyp>::operator=(m);
    Copy(m);
    return *this;
}

bool Chromosom::operator==(const Chromosom& ch) const
{
    if (this == &ch) { return true; };     // gleiches Chromosom
    if (THIS.size() != ch.size()) {    // garantiert Ungleich
        return false;
    }
    size_type i;
    for(i=0; i < size() && THIS[i] == ch[i]; ++i ) ; // Ueberprufung
    return i == size();
}

void Chromosom::Save(const std::string& FileName) const
{
    FILE *file;
    if( ( file=fopen(FileName.c_str(),"wb") ) == nullptr ) { INT_ERR (__LINE__); }

    {
        // 4-7-94, 2:51 PM
        // Speichert die Groesse des NukleoDatenTyps als erstes in der Datei
        const uint16_t NukleoLen = sizeof(NukleoTyp);
        if( fwrite (&NukleoLen, sizeof (NukleoLen), 1, file) != 1 ) { INT_ERR (__LINE__); }
        // 4-7-94, 2:51 PM
    }
    const size_type len=size();
    if( fwrite (&len, sizeof(len), 1, file) != 1 ) { INT_ERR (__LINE__); }
    for(size_type i=0; i<len; ++i) {
        if( fwrite (&((*this)[i]), sizeof (NukleoTyp), 1, file) != 1 ) {
            INT_ERR (__LINE__);
        }
    }
    fclose(file);
}

void Chromosom::Load(const std::string& FileName) {
    FILE *file;
    if ((file=fopen(FileName.c_str(), "rb")) == nullptr) { INT_ERR (__LINE__); }

    {
        // 4-7-94, 3:3 PM
        // Liest als erstes die Groesse des abgespeicherten NukleoTyps
        // aus dem File
        uint16_t NukleoLen;
        if( fread (&NukleoLen, sizeof (NukleoLen), 1, file) != 1 ) { INT_ERR (__LINE__); }
        if( NukleoLen != sizeof(NukleoTyp) ) {
            std::cerr << "!! ERROR !!\n"
                    << "   Laenge des Datentyps der gespeicherten Nuklotide\n"
                    << "   ist kleiner als das abgespeicherte Format\n";
            exit (0);
        }
        // 4-7-94, 3:3 PM
    }
    // Puffer der richtigen Laenge bereit stellen
    NukleoTyp value;
    size_type len;
    if (fread (&len, sizeof (len), 1, file) == 0) { INT_ERR (__LINE__); }
    for(size_type i=0; i<len; ++i) {
        if( fread (&value, sizeof(value), 1, file) == 0 ) { INT_ERR (__LINE__); }
        insert(i, value);
    }
    fclose(file);
}

int Chromosom::GetNukleotidNumber (size_type i) const
{
    NukleoTyp UserNukleotidValue=(*this)[i];

    if( env.UserNukleoValScale * (env.Nukleotide-1) <= UserNukleotidValue &&
        UserNukleotidValue <= env.UserNukleoMaxVal )
    {
        // Trifft auch fuer Nukleotide gleich nullptr zu !!!
        return env.Nukleotide;
    }

    for (size_type n=env.Nukleotide-1; n > 0; --n) { // FIXME: Double '-1' here and on 'n'?
        if( env.UserNukleoValScale * (n-1) <= UserNukleotidValue &&
            UserNukleotidValue < env.UserNukleoValScale * n ) {
            return n;
        }
    }

    INT_ERR(__LINE__);
    return 0;
}

NukleoTyp Chromosom::GetUserNukleotidValue (size_type i) const
{
    return env.UserNukleoValScale*(i-1);
}

bool Chromosom::FindIntron (size_type &von, size_type &bis) const
{
    bool done = false;
    size_type i, i2, i3;

    if (bis-von+1 >= env.ptrSpliceCodeInfo->Length)
    {
        // Spleissen..Anfang suchen
        i=von; i2=0;
        while (i <= bis && env.ptrSpliceCodeInfo->SpliceCode[i2] != 0) {
            if (GetNukleotidNumber (i) == env.ptrSpliceCodeInfo->SpliceCode[i2]) {
                ++i2;
                ++i;
            } else {
                i2=0;
                von=++i;
            }
        }
        if (i < bis) {
            assert (env.ptrSpliceCodeInfo->SpliceCode[i2] == 0) ;
            // von gefunden...
            // Spleissen..Ende suchen
            i3=i;
            i=++i2;
            while (i3 <= bis && env.ptrSpliceCodeInfo->SpliceCode[i2] != 0) {
                if (GetNukleotidNumber(i3) == env.ptrSpliceCodeInfo->SpliceCode[i2]) {
                    i2++;
                } else {
                    i2=i;
                }
                i3++;
            }
            if (env.ptrSpliceCodeInfo->SpliceCode[i2] == 0) {
                // bis gefunden...
                bis=i3-1;
                done=true;
            }
        }
    }
    return done;
}

Chromosom::size_type Chromosom::Splicing()
{
  size_type NNukleotide=0, IntronStart=0, IntronEnd=size()-1;
  #ifndef NDEBUG
      size_type len_old;
  #endif

  // Die Bedingung fuer den SpliceCode ist Nukleotide > 1 !
  // Nukleotide wird in Chromosomen::CalcParameter gesetzt !
  if (env.Nukleotide <= 1) {
      // Kein Splicing , da kein SpliceCode !!!
      return 0;
  }

  while( FindIntron(IntronStart, IntronEnd) ) {
      #ifndef NDEBUG
          len_old=size();
      #endif
      assert( IntronStart <= IntronEnd );
      erase( IntronStart, IntronEnd+1 );
      assert ( len_old - ( IntronEnd - IntronStart + 1 ) == size() );
      NNukleotide+=IntronEnd-IntronStart+1;
      IntronEnd=size()-1;
  }
  return NNukleotide;
}

void Chromosom::InsertIntrons (size_type von, size_type length)
{
    size_type i, iRumpf;

    if (env.ptrSpliceCodeInfo != nullptr &&
            env.ptrSpliceCodeInfo->SpliceCode[0] != 0 &&
            von <= size()           &&
            length >= env.ptrSpliceCodeInfo->Length)
    {
        // StartSequenz einfuegen.
        for (i=0; env.ptrSpliceCodeInfo->SpliceCode[i] != 0; ++i, ++von, --length) {
            insert(von, GetUserNukleotidValue(env.ptrSpliceCodeInfo->SpliceCode[i]));
        }
        // Stelle fuer Rumpf merken.
        iRumpf=von;
        // EndSequenz einfuegen.
        for (++i; env.ptrSpliceCodeInfo->SpliceCode[i] != 0; ++i, ++von, --length) {
            insert(von, GetUserNukleotidValue (env.ptrSpliceCodeInfo->SpliceCode[i]));
        }
        // Rumpf fuellen.
        while (length > 0) {
            insert(iRumpf, Random (env.UserNukleoMinVal, env.UserNukleoMaxVal));
            length--;
        }
    }
}

void Chromosom::Inversion()
{
  Chromosom save(env);
  size_type start, end, i;

  assert (save.size()==0);
  start = Random(0, size()-2);
  assert (start < size()-1);
  end = Random (start+1, size()-1) ;
  assert (end < size());
  assert (end > start);
  for (i=start; i <= end; ++i) { save.push_front((*this)[i]); }
  assert (save.size()==end-start+1);
  for (i=start+1; i <= end; ++i)  {
    assert( end-i < save.size() );
    (*this)[i]=save[end-i];
  }
}

void Chromosom::Translocation()
{
  Chromosom save(env);
  size_type start, end, dest, i;

  assert (save.size()==0);
  start = Random (0, size()-2);
  assert (start < size()-1);
  end = Random (start+1, size()-1) ;
  assert (end < size());
  assert (end > start);
  // while( ( end = Random(0, size()-1) ) <= start) ; // FIXME: ????
  for (i=start; i <= end; ++i) {
    save.push_back((*this)[i]);
  }
  erase(start, end+1);
  assert( save.size() == end-start+1 );

  dest = Random( 0, size()-1 );
  for (i=0; i<save.size(); ++i) {
      (*this).insert(dest+i, save[i]);
  }
}

// *******************************************************************
// *******************************************************************
//  C H R O M O S O M E N
// *******************************************************************
// *******************************************************************

// STATISCHE-KLASSEN-GLOBALE VARIABLEN !!!

const Chromosomen::size_type Chromosomen::MutationFreqVar=1000;

Chromosomen::Chromosomen (NukleoTyp UserNukleoMinVal, 
                          NukleoTyp UserNukleoMaxVal,
                          size_type MaxChromosomen,
                          const std::string& StartGenFile,
                          size_type Nukleotide,
                          SpliceCodeInfo *PtrSpliceCodeInfo,
                          size_type InversionFreq,
                          size_type TranslocationFreq,
                          size_type AsymXOverFreq,
                          size_type CrossVal,
                          size_type MutationFreq )
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

  ChromosomenLenMin(0),
  ChromosomenLenMax(0),
  ChromosomenLenAvrg(0),
  ChromBetterZeroNumber(0),

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

    CalcParameter();

    if ((file=fopen(StartGenFile.c_str(),"rb") ) == nullptr ) {
        fprintf(stderr,"can not open file %s\n", StartGenFile.c_str());
        INT_ERR(__LINE__);
    }
    {
        // 4-7-94, 3:3 PM
        // Liest als erstes die Groesse des abgespeicherten NukleoTyps
        // aus dem File
        uint16_t NukleoLen;
        if (fread (&NukleoLen, sizeof(NukleoLen), 1, file)!=1) { INT_ERR(__LINE__); }
        if( NukleoLen != sizeof(NukleoTyp) ) {
            std::cerr << "!! ERROR !!\n"
                    << "   Laenge des Datentyps der gespeicherten Nuklotide\n"
                    << "   ist kleiner als das abgespeicherte Format\n";
            exit (0);
        }
        // 4-7-94, 3:3 PM
    }

    // int ChromLen, ChromQuantity, i, j;
    size_type ChromQuantity;
    if (fread(&ChromQuantity, sizeof(ChromQuantity), 1, file)==0) { // FIXME: Use fixed width type
        INT_ERR(__LINE__);
    }

    // Puffer der richtigen Laenge bereitstellen
    NukleoTyp value;
    for(size_type j=0; j<ChromQuantity; ++j ) {
        Chromosom Gamma( *this, 0 );
        assert (Gamma.size()==0);

        size_type ChromLen;
        if (fread(&ChromLen, sizeof(ChromLen), 1, file) == 0) { INT_ERR (__LINE__); } // FIXME: Use fixed width type
        for(size_type i=0; i<ChromLen; ++i) {
            if (fread(&value, sizeof(value), 1, file) == 0) { INT_ERR(__LINE__); }
            Gamma.push_back(value);
        }
        assert (Gamma.size()==ChromLen);
        push_back(Gamma);
    }
    assert (size()==ChromQuantity);
    fclose (file);
}

Chromosomen::Chromosomen ( NukleoTyp UserNukleoMinVal, NukleoTyp UserNukleoMaxVal,
                           size_type MaxChromosomen,
                           size_type StartChromosomNumber,
                           size_type StartChromosomLength,
                           size_type Nukleotide,
                           SpliceCodeInfo *PtrSpliceCodeInfo,
                           size_type InversionFreq,
                           size_type TranslocationFreq,
                           size_type AsymXOverFreq,
                           size_type CrossVal,
                           size_type MutationFreq )
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

  ChromosomenLenMin(0),
  ChromosomenLenMax(0),
  ChromosomenLenAvrg(0),
  ChromBetterZeroNumber(0),

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
    CalcParameter();

    // StartGene zufaellig setzen !
    for (size_type i=0; i<StartChromosomNumber ; ++i) {
        Chromosom Gamma (*this, StartChromosomLength);
        assert (Gamma.size() == StartChromosomLength);
        push_back(Gamma);
    }
    assert (size()==StartChromosomNumber);
}

void Chromosomen::Save (const std::string& FileName) const
{
    FILE *file;

    if((file=fopen(FileName.c_str(), "wb")) == nullptr) {
        INT_ERR (__LINE__);
    }
    {
        // 4-7-94, 2:51 PM
        // Speichert die Groesse des NukleoTyps als erstes im File ab
        const uint16_t NukleoLen=sizeof(NukleoTyp);
        if (fwrite (&NukleoLen, sizeof (NukleoLen), 1, file) != 1) { INT_ERR (__LINE__); }
        // 4-7-94, 2:51 PM
    }
    int ChromQuantity=size();
    if (fwrite (&ChromQuantity, sizeof (ChromQuantity), 1, file) != 1 ) { // FIXME: Use fixed width type
        INT_ERR (__LINE__);
    }
    for (int j=0; j<ChromQuantity; j++ ) {
        const int ChromLen = THIS[j].size();
        if (fwrite (&ChromLen, sizeof (ChromLen), 1, file) != 1) { INT_ERR (__LINE__); } // FIXME: Use fixed width type
        for(int i=0; i<ChromLen; ++i) {
            NukleoTyp value = THIS[j][i];
            if (fwrite (&value, sizeof (NukleoTyp), 1, file) != 1) { INT_ERR (__LINE__); }
        }
    }
    fclose(file);
}

int Chromosomen::Evolution (double GoalFitness, const std::string& chrptrPtkFile,
			                double BirthRate, int Bigamie,
			                size_type NoImprovingCrossingOvers )
{
    double BestEverAverageFitness = -1,
           Cut = 0; // Der Cut beim Sterben
    size_type NoImproving = 0;
    bool stop = false;

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
    if( !Echo() ) { stop = true; }

    if( BirthRate <= 0.0 || BirthRate > 1.0 ) { stop = true; }

    while( BestFitness < GoalFitness &&
           NoImproving < NoImprovingCrossingOvers &&
           !stop )
    {
        Generation++;
        SplicedChromosoms=0;
        IntroCodeLenSum=0;
        if( BirthRate < 1.0 ) {
            NewGeneration(BirthRate, Bigamie);
            // Fuer LetDie !!!
            Cut = GetXWorstFitness( size() - MaxChromosomen );
        } else {
            NewGeneration(Bigamie);
            Cut = -.5;	// Eltern Fitness auf -1 gesetzt
        }
        // Sterben !!!
        LetDie(Cut);
        // Die Mutation
        Mutation();
        // Fitness berechnen
        CalcWholeFitness();
        // Weitere Mutationen
        InversionsMutation();
        TranslocationsMutation();

        GenerationEnd=time(nullptr);

        if( BestEverAverageFitness < AverageFitness ) {
            NoImproving=0;
            BestEverAverageFitness = AverageFitness ;
        } else {
            NoImproving++ ;
        }
        Protokoll();
        if(!Echo()) { stop = true; }
    }

    EvolutionEnd=time(nullptr);
    Echo();
    Protokoll();

    if (fileptrPtk != nullptr) { fclose (fileptrPtk); }

    return Generation;
}

void Chromosomen::InitFitness(void)
{
    for (size_type i = 0; i < size(); ++i) {
        Chromosom Neu = THIS[i];
        Neu.Splicing();
        THIS[i].SetFitness(Fitness(Neu));
    }
    CalcWholeFitness();
}

void Chromosomen::NewGeneration(double BirthRate, bool Bigamie)
{
    Liste<size_type, size_type> ElternPaare;

    size_type l = (size_type) ( size() * BirthRate );
    size_type i;

    GenerationStart=time(nullptr);

    // Gerade Anzahl der ElternPaare !!!
    if( l%2 > 0 ) {
        --l;
    }

    // ooops, die letzten beiden Elternpaare sind nicht-bigamistisch SCHLECHT
    // zu finden !!!
    if( 2 <= ChromBetterZeroNumber && ChromBetterZeroNumber <= l ) {
        ChromBetterZeroNumber -= 2;
    }

    // Neue Partner in die ElternPaare-Menge einfuegen
    // "Bigamistische Beziehung" werden, wenn moeglich, nicht erlaubt.
    while(l>0) {
        // Nur NEUE Partner !!!
        if (!Bigamie && ElternPaare.size() < ChromBetterZeroNumber) {
            do {
                i=RouletteSelect();
            } while( ElternPaare.contains(i) ) ;
            ElternPaare.push_back(i);
            l--;
        } else {
            if (!Bigamie)   {
                // the hard way ...
                do {
                    i = Random(0, size()-1);
                } while( ElternPaare.contains(i) ) ;
                ElternPaare.push_back(i);
                l--;
            } else {
                // Bigamistische Beziehung JA, aber sexuell !!!
                int w, m;
                ElternPaare.push_back((w = Random (0, size()-1))) ;
                while (w == (m=Random (0, size()-1))) ;
                ElternPaare.push_back(m) ;
                l-=2;
            }
        }
    };

    l=ElternPaare.size();

    #ifndef NDEBUG
        // alles ok ?? GERADE ELTERNPAARE ????
        assert( l%2==0 );
        assert( l== ( (size_type)( size() * BirthRate ) % 2 ) ?
                    (size_type)( size() * BirthRate ) - 1 :
                    (size_type)( size() * BirthRate )
        );
    #endif

    // Fortpflanzen !!!
    while (l > 1) {
        CrossingOver (ElternPaare[l-1], ElternPaare[l-2]);
        l -= 2 ;
    }
    assert (l == 0);
}


void Chromosomen::NewGeneration(bool Bigamie)
{
    Liste<size_type, size_type> ElternPaare;
    size_type l = MaxChromosomen, w, i;

    GenerationStart=time(nullptr);

    // Gerade Anzahl der ElternPaare !!!
    if( l%2 > 0 ) {
        --l;
    }

    // ooops, die letzten beiden Elternpaare sind nicht-bigamistisch SCHLECHT
    // zu finden !!!
    if( 2 <= ChromBetterZeroNumber && ChromBetterZeroNumber <= l ) {
        ChromBetterZeroNumber -= 2;
    }

    // Neue Partner in die ElternPaare-Menge einfuegen
    // "Bigamistische Beziehung" werden, wenn moeglich, nicht erlaubt.
    while( l > 0 ) {
        // Nur NEUE Partner !!!
        if( !Bigamie && ElternPaare.size() < ChromBetterZeroNumber ) {
            do {
                i = RouletteSelect();
            } while ( ElternPaare.contains(i) ) ;
            ElternPaare.push_back(i);
            l--;
        } else {
            if( !Bigamie ) {
                do {
                    i = Random(0, size()-1);
                } while( ElternPaare.contains(i) ) ;
                ElternPaare.push_back(i);
                l--;
            } else { // Bigamistische Beziehung JA, aber sexuell !!!
                size_type w, m;
                ElternPaare.push_back( ( w = Random(0, size()-1) ) );
                while( w == ( m = Random(0, size()-1) ) ) ;
                ElternPaare.push_back(m);
                l-=2;
            }
        }
    };

    l=ElternPaare.size();


    // alles ok ?? GERADE ANZAHL DER ELTERN !!!
    assert (l%2==0);

    // Neue Population...
    l=ElternPaare.size();
    while (l > 1) {
        // Beim Crossing Over werden die Nachkommen am Ende der Populations-Liste
        // eingefuegt.
        // Die Indizes fuer die ElternPaare bleiben somit aktuell !!!
        CrossingOver (ElternPaare[l-1], ElternPaare[l-2]);
        l-=2;
    }

    // Eltern markieren, damit sie "getoetet" werden koennen !!!
    // Das geschied in 'LetDie', Aufruf in Evolution
    l=ElternPaare.size()-1;
    while (l > 0) {
        w=ElternPaare[l-1];
        // markieren !!!
        THIS[w].SetFitness(-1);
        l--;
    }
}

void Chromosomen::LetDie(double cut)
{
    size_type i=0;

    while (i < size() && size() > MaxChromosomen) {
        if (THIS[i].GetFitness() <= cut) {
            Kill (i);
        } else {
            ++i;
        }
    }

    // Sicherheitsabfrage
    if (size() > MaxChromosomen) {
        cut = GetXWorstFitness (size()-MaxChromosomen);
        i = 0;
        while (i < size() && size() > MaxChromosomen) {
            if (THIS[i].GetFitness() <= cut) {
                Kill (i);
            } else {
                ++i;
            }
        }
    }
    assert (size() <= MaxChromosomen);
}

Chromosomen::size_type Chromosomen::RouletteSelect() const
// Chris: geaendert
{
    size_type i;

    double Sum       = 0.0;
    const double Threshold = Random (0.0, 1.0) * GetFitnessSum();

    // Aufgrund der Ungenauigkeit der Flieskommaoperatoren
    // muss auch das Ende der Liste abgefragt werden.
    for (i=0; Sum < Threshold && i<size(); ++i) {
        Sum += THIS[i].GetFitness();
    }
    return ( i < size() ) ? i : size()-1;
}

void Chromosomen::CreateNewSymChromosom ( Chromosom &dest, size_type m, size_type w,
					                      SortListe<size_type, size_type> &CrossPoints )
{
    // i          : Indize des Crosspoints
    // von, bis   : Zu Uebertragender Chromosomenabschnitt [von..bis[
    // ch         : Alternierendes Indize zwischen den beiden Chromosomen
    //              mit den Geschlechtern 'm' und 'w'
    // done       : Abbruch Indikator
    bool done = false;
    size_type i, von, bis, ch;

    // Startwerte !!
    i = bis = 0;
    ch = w;

    do {
        // An dem letzten exklusiven Ende fortfahren
        von = bis;
        if ( i < CrossPoints.size() ) {
            // Chromosomenabschnitt-Ende holen.
            if( CrossPoints[i] < THIS[ch].size() ) bis = CrossPoints[i];
            else                                   bis = THIS[ch].size();
            ++i;
        } else {
            // ...und den Rest uebertragen
            bis = THIS[ch].size();
            done = true;
        }
        assert ( bis <= THIS[ch].size() );
        assert ( von == dest.size() );
        // Chromosomenabschnitt uebertragen [von..bis[
        for (size_type n=von; n<bis; n++) {
            dest.insert(n, THIS[ch][n]);
        }
        // Indize zwischen den Geschlechtern alternieren
        ch = ( ch==m ? w : m ) ;
    } while ( !done );

    // ...sonst Fehler !!!
    assert( dest.size() > 0 );
}

void Chromosomen::CrossingOver (size_type m, size_type w)
{
    if ( CrossVal == 0 ) { return; }

    size_type shortest=( ( THIS[m].size() < THIS[w].size() ) ? m : w);

    if ( XOverNumber++ < AsymXOverFreq || AsymXOverFreq==0 ) {
        // Symmetrisches XOver !!!
        SortListe<size_type, size_type> CrossPoints;
        assert( CrossPoints.size()==0 );

        // Kreuzungspunkte sortiert eintragen.
        for ( size_type i = 0; i < CrossVal; ++i ) {
            CrossPoints.insert( Random(0 , THIS[shortest].size()) );
        }

        Chromosom NeuA (*this, 0);
        Chromosom NeuB (*this, 0);
        size_type SplicedCode;

        CreateNewSymChromosom ( NeuA, m, w, CrossPoints );
        push_back( NeuA );
        SplicedCode=NeuA.Splicing();
        if(SplicedCode>0) {
            SplicedChromosoms++;
            IntroCodeLenSum+=SplicedCode;
        }
        // Die Fitness des gespleissten Chromosomes in das ungespleisste
        // eingebundene Chromosom einsetzen !!!
        THIS[size()-1].SetFitness(Fitness(NeuA));

        CreateNewSymChromosom ( NeuB, w, m, CrossPoints );
        push_back( NeuB );
        SplicedCode=NeuB.Splicing();
        if(SplicedCode>0) {
            SplicedChromosoms++;
            IntroCodeLenSum+=SplicedCode;
        }
        // Die Fitness des gespleissten Chromosomes in das ungespleisste
        // eingebundene Chromosom einsetzen !!!
        THIS[size()-1].SetFitness(Fitness(NeuB));

    } else {

        XOverNumber = 0;
        // Assymetrisches XOver : Nur ein neues Element !
        Chromosom Neu (*this, 0);

        // Kopieren
        Neu=THIS[m];

        // anhaengen
        for (size_type n=0; n<THIS[w].size(); n++) {
            Neu.push_back( THIS[w][n] );
        }
        Neu.SetFitness(Fitness(Neu));
        push_back( Neu );

    }
}

void Chromosomen::Mutation()
{
    static size_type next_nukleotide_idx = MutationFreq + Random( 0, (size_type)(MutationFreq/MutationFreqVar) ) ;
    MutationsThisGeneration=0;

    if ( MutationFreq > 0 ) {
        size_type nukleotide_idx = next_nukleotide_idx;
        for(size_type chromosom_idx = 0; chromosom_idx < size(); ++chromosom_idx ) {
            bool mutated = false;
            const size_type chromosom_len = THIS[chromosom_idx].size();
            while( nukleotide_idx < chromosom_len ) {
                // Die Mutation.
                (THIS[chromosom_idx])[nukleotide_idx] = Random ( UserNukleoMinVal, UserNukleoMaxVal);
                MutationsThisGeneration++;
                mutated=true;
                nukleotide_idx += MutationFreq + Random( 0, (size_type)(MutationFreq/MutationFreqVar) );
            }
            if( nukleotide_idx >= chromosom_len ) {
                nukleotide_idx -= chromosom_len;
            }
            if(mutated) {
                // Fitness neuberechnen ...
                // ... this is a social event - we are hard folks :-)
                Chromosom Neu (THIS[chromosom_idx]);
                Neu.Splicing();
                THIS[chromosom_idx].SetFitness(Fitness(Neu));
            }
        }
        next_nukleotide_idx = nukleotide_idx;
    }
}


void Chromosomen::InversionsMutation()
{
    static size_type next_chromosomen_idx = InversionFreq;
    InversionsThisGeneration=0;
    size_type chromosomen_idx = next_chromosomen_idx;
    while( chromosomen_idx < size() ) {
        InversionsThisGeneration++;
        THIS[chromosomen_idx].Inversion();
        // Fitness neuberechnen ...
        Chromosom Neu (THIS[chromosomen_idx]);
        Neu.Splicing();
        THIS[chromosomen_idx].SetFitness(Fitness(Neu));
        chromosomen_idx += InversionFreq;
    }
    if( chromosomen_idx >= size() ) {
        chromosomen_idx -= size();
    }
    next_chromosomen_idx = chromosomen_idx;
}

void Chromosomen::TranslocationsMutation()
{
    static size_type next_chromosomen_idx = TranslocationFreq;
    TranslocationsThisGeneration=0;
    size_type chromosomen_idx = next_chromosomen_idx;
    while( chromosomen_idx < size() ) {
        TranslocationsThisGeneration++;
        THIS[chromosomen_idx].Translocation();
        // Fitness neuberechnen ...
        Chromosom Neu (THIS[chromosomen_idx]);
        Neu.Splicing();
        THIS[chromosomen_idx].SetFitness(Fitness(Neu));
        chromosomen_idx += TranslocationFreq;
    }
    if( chromosomen_idx >= size() ) {
        chromosomen_idx -= size();
    }
    next_chromosomen_idx = chromosomen_idx;
}

void Chromosomen::CalcWholeFitness()
{
    double Total=0, TempFitness;
    size_type BestChrom=npos, ChromLen;
    size_type ChromLenSum=0;

    ChromosomenLenMin = std::numeric_limits<size_type>::max();
    ChromosomenLenMax = std::numeric_limits<size_type>::min();
    BestFitness = -1;
    ChromBetterZeroNumber = 0;

    for (size_type i = 0; i < size(); ++i) {
        ChromLen = THIS[i].size();
        ChromLenSum += ChromLen;
        if( ChromosomenLenMin > ChromLen ) { ChromosomenLenMin=ChromLen; }
        if( ChromosomenLenMax < ChromLen ) { ChromosomenLenMax=ChromLen; }
        if( ( TempFitness = THIS[i].GetFitness() ) > 2*std::numeric_limits<double>::epsilon() ) {
            ChromBetterZeroNumber++ ;
        }
        Total += TempFitness ;
        if( BestFitness < TempFitness ) {
            BestFitness = TempFitness;
            BestChrom = i;
        }
    }
    AverageFitness = Total/size();
    FitnessSum = Total;
    ChromosomenLenAvrg = (double)ChromLenSum/(double)size();
    if( TheBestEver.GetFitness() < BestFitness && npos != BestChrom) {
        TheBestEver=THIS[BestChrom];
        TheBestEver.Splicing();
    }
    if (TheBestEversAverageFitness < AverageFitness) {
        TheBestEversAverageFitness = AverageFitness;
        TheBestEversGeneration     = Generation;
    }
}

Chromosomen::size_type Chromosomen::GetWorstChromosom() const
{
    double WorstFitness=2, TempFitness;
    size_type iw = npos;

    for (size_type i=0; i<size(); ++i) {
        TempFitness=THIS[i].GetFitness();
        if (WorstFitness > TempFitness) {
            WorstFitness = TempFitness ;
            iw=i;
        }
    }
    return iw;
}

double Chromosomen::GetXWorstFitness(size_type count) const
{
    SortListe<double> Worst;

    if (count == 0) { return -1; } // Falsche Anzahl uebergeben
    for (size_type i=0; i<size(); ++i) {
        if( Worst.size() < count ) {
            Worst.insert(THIS[i].GetFitness());
        } else if( THIS[i].GetFitness() < Worst[count-1] ) {
            Worst.insert(THIS[i].GetFitness());
        }
    }
    if (Worst.size() >= count) {
        return Worst[count-1];
    } else {
        return Worst[Worst.size()-1];
    }
}

Chromosomen::size_type Chromosomen::GetBestChromosom() const
{
    double BestFitness=-1, TempFitness;
    size_type ib = npos;

    for (size_type i=0; i<size(); ++i) {
        TempFitness=THIS[i].GetFitness();
        if (BestFitness < TempFitness) {
            BestFitness = TempFitness ;
            ib=i;
        }
    }
    return ib;
}

const Chromosom &Chromosomen::GetTheBestEverChromosom()
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
        ptrSpliceCodeInfo->SpliceCode[0]==0 )
    {
        // Ohne Splicing ....
        Nukleotide=0;
        UserNukleoValScale=1;
    } else {
        if (ptrSpliceCodeInfo->Length ==0) {
            // Laenge berechnen ...
            int i;

            for (i=0; ptrSpliceCodeInfo->SpliceCode[i]!=0; ++i) {
                ptrSpliceCodeInfo->Length ++;
            }
            for (++i; ptrSpliceCodeInfo->SpliceCode[i]!=0; ++i) {
                ptrSpliceCodeInfo->Length ++;
            }
        }
        if(Nukleotide>0) {
            UserNukleoValScale = ( UserNukleoMaxVal - UserNukleoMinVal + 1 ) / Nukleotide ;
        } else {
            UserNukleoValScale = 1;
        }
    }
}

void Chromosomen::Protokoll()
{
    double SpliceCodePerChrom ;

    SpliceCodePerChrom = (SplicedChromosoms > 0) ?
            ((double)IntroCodeLenSum/(double)SplicedChromosoms)
            : 0.0
              ;

    if (fileptrPtk != nullptr) {
        if (EvolutionEnd == 0) {
            fprintf (fileptrPtk, "=======================================================\n\n\n");
            fprintf (fileptrPtk, "\nGeneration / Generierungsdauer        : %3zu  /  %zu s\n",
                    (size_t)Generation, (size_t)(GenerationEnd-GenerationStart) );
            fprintf (fileptrPtk, "Populationsgroesse                    : %3zu\n",
                    (size_t)size());
            fprintf (fileptrPtk, "Chromosomen Laenge Minimum            : %3zu\n",
                    (size_t)ChromosomenLenMin);
            fprintf (fileptrPtk, "Chromosomen Laenge Maximum            : %3zu\n",
                    (size_t)ChromosomenLenMax);
            fprintf (fileptrPtk, "Chromosomen Laenge Durchschnitt       : %10.6lf\n",
                    ChromosomenLenAvrg);
            fprintf (fileptrPtk, "Gespleisste Chromosomen               : %3zu\n",
                    (size_t)SplicedChromosoms);
            fprintf (fileptrPtk, "Gespleisstes Code pro Chromosom       : %10.6lf\n",
                    SpliceCodePerChrom);

            fprintf (fileptrPtk, "\nBestFitness                           : %10.6lf\n",
                    BestFitness);
            fprintf (fileptrPtk, "Average Fitness                       : %10.6lf\n",
                    AverageFitness);

            fprintf (fileptrPtk, "\nMutationen dieser Generation          : %3zu\n",
                    (size_t)MutationsThisGeneration);
            fprintf (fileptrPtk, "Inversionen dieser Generation         : %3zu\n",
                    (size_t)InversionsThisGeneration);
            fprintf (fileptrPtk, "Translokationen dieser Generation     : %3zu\n",
                    (size_t)TranslocationsThisGeneration);
            fprintf (fileptrPtk, "\nTheBestEverFitness                    : %10.6lf\n",
                    TheBestEver.GetFitness());
            fprintf (fileptrPtk, "TheBestEversAverageFitness            : %10.6lf\n",
                    TheBestEversAverageFitness);
            fprintf (fileptrPtk, "TheBestEversGeneration                : %3zu\n\n",
                    (size_t)TheBestEversGeneration);
        } else {
            if(Generation>1) {
                fprintf (fileptrPtk, "Avrg. Generationsdauer                  : %f s / Generation\n",
                        ((double) (EvolutionEnd-EvolutionStart))/((double)(GetGeneration()-1)) );
            }
            fprintf (fileptrPtk, "\nGenerationen / Evolutionsdauer        : %3zu /  %zu s\n",
                    (size_t)Generation, (size_t)(EvolutionEnd-EvolutionStart));
        }
    }
}

bool Chromosomen::Echo() const
{
    if (Generation == 1) {
        printf(" Generation: BestGeneration: AverageFitness: BestFitness:"
                " TheBestLength:\n");
    }
    printf("\r%11zu%16zu%16.9lf%13.9lf%15zu",
            (size_t)GetGeneration(), (size_t)TheBestEversGeneration,
            GetAverageFitness(), GetBestFitness(),
            (size_t)TheBestEver.size() );

    if (EvolutionEnd > 0) {
        if (Generation > 1) {
            printf ("\n\nAvrg. Generationsdauer                : %f s / Generation\n",
                    ((double)(EvolutionEnd-EvolutionStart))/((double)(GetGeneration()-1)) );

        }
        printf ("\n\nGenerationen / Evolutionsdauer        : %3zu  /  %3zu s\n",
                (size_t)Generation, (size_t)(EvolutionEnd-EvolutionStart));
    }
    return true;
}

void Chromosom::Ausgabe (std::ostream& OS) const noexcept
{
    const int Zeilenlaenge = 10;

    OS << "( l " << size() << ", f "
       << ( ( GetFitness() < std::numeric_limits<double>::epsilon() ) ? (double)(0) : GetFitness() )
       << " ) < ";
    for (size_type i=0; i<size(); ++i) {
        if (i % Zeilenlaenge == 0 &&  i) {
            OS << "\n\t";
        }
        double wandler = (*this)[i];
        OS << wandler;
        if (i % Zeilenlaenge != Zeilenlaenge-1 && i != size()-1) {
            OS << ", ";
        }
    }
    OS << " >";
}

void Chromosomen::Ausgabe (std::ostream& OS) const noexcept
{
    const int Zeilenlaenge = 7;

    OS << "( " << size() << " ) < ";
    for (size_type i=0; i<size(); ++i) {
        if (i % Zeilenlaenge == 0 &&  i) {
            OS << "\n\t";
        }
        OS << ( (*this)[i].GetFitness() < std::numeric_limits<double>::epsilon() ) ? (double)(0) : (*this)[i].GetFitness();
        if (i % Zeilenlaenge != Zeilenlaenge-1 && i != size()-1) {
            OS << ", ";
        }
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

