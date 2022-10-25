// authors : 
//   Sven Gothel - www.jausoft.com
//
// liste.h  
//
// Original von
// Martin Schader & Stefan Kuhlins : Programmieren in C++, Springer Verlag,
// ISBN-3-540-56524-8
//
// Modifizierungen :
//   fuegeEin, loesche und beide Indexoperatoren verwenden nun die
//   doppelt verkettete Liste mit dem aktuellen Zeiger und Indize !!!
//
// Modifizierungen :
//   operator !(), operator int(), operator new
//   ListElem ist jetzt eine interne Klasse von Liste (Chr. Mueller)
//
// istElement uebertragen von Menge.h von Sven Gothel.
// Referenz2Index eingefuegt von Sven Gothel.
/*
 *
 *  $Log: liste.h $
 *  Revision 1.9  1995/04/07 16:54:51  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.8  1995/04/04 10:38:18  Sven_Gothel
 *  Listen Index-Operator nun Strenger fuer constante Listen (
 *    Rueckgabewert auch hier konstant !! )
 *  Revision 1.7  1995/04/04 10:37:26  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.6  1995/03/07 05:59:28  sg
 *  WATCOM Version < 10.0 bedingte compilierung ergaenzt !!!
 *  Revision 1.5  1995/03/06 18:49:56  ks
 *  Bedingte compilation jetzt auch fuer WATCOM C++ Version < 10.0
 *  Revision 1.4  1994/12/15 15:28:10  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.3  1994/12/15 15:26:57  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.2  1994/12/15 15:24:36  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.1  1994/12/15 15:18:40  Sven_Gothel
 *  Initial revision
 *
*/

# ifndef _LISTE_H
  # define _LISTE_H

  # include <iostream>
  # include <cstdlib>
  # include <cstdio>
  # include <assert.h>
  # include "interror.h"

  // Klasse Liste
  // Der Typ der Listenelemente ist beliebig
  //
  // 1. Element hat den Index 0 !!!!!
  //
  // Elementfunktionen:
  //
  // Falsche Indexoperatoren rufen exit(1), mit Angabe der Zeilennummer auf.
  //
  // int fuegeEin(const Tt& x, int i)
  //     Erzeugt ein neues Listenelement, welches <Tt> beinhaltet
  //     und fuegt es als (i+1)'te Komponente in die Liste ein.
  //     liefert 1, falls erfolgreich, 0 sonst
  // int loesche(int i)
  //     loescht (i+1)'te Komponente aus der Liste
  //     liefert 1, falls erfolgreich, 0 sonst
  // int laenge()
  //     liefert die aktuelle Anzahl der Listenelemente
  // der Indexoperator [] const ist fuer const und nicht-const
  //     Listenobjekte definiert
  //     Ein Zugriff mit falschem Index ruft exit(1) auf
  // der NOT-Operator ! gibt einen int-Wert ungleich Null
  //     zurueck, wenn die Liste leer ist, sonst wird Null
  //     zurueckgeliefert
  // der Umwandlungsoperator 'int()' ist das Gegenstueck zum
  //     NOT-Operator, d.h. er liefert einen int-Wert ungleich
  //     Null bei nicht leerer Liste, und Null bei leerer Liste
  //  !! Vorsicht: wird eine Listeinstanz in einem Zusammen-
  //  !! hang benutzt, indem ein int-Wert erwartet wird, so wird
  //  !! dieser Umwandlungsoperator aufgerufen
  //
  //  istElement(const Tt&, int =0) somit ist ein Test ob das Element
  //  schon vorhanden ist moeglich.
  //  Es wird von i bis laenge-1 gesucht.
  //  RETURN : ListenIndize, wenn gefunden;
  //           bzw -1, wenn nicht gefunden !!!
  //
  // int Referenz2Index(const Tt& a)
  // Es wird ein Element mit der Referenz a in der Liste gesucht !
  // RETURN : -1  : nicht gefunden.
  //          >=0 : gefunden, index des Elementes. Sven Gothel
  //
  //
  // Iteratoren:
  //
  // ListElem *anfang()
  //     liefert einen Zeiger auf den Listenanfang
  // ListElem *iter_fw()
  //     positioniert den Zeiger auf das folgende Element
  //     liefert am Ende der Liste 0
  // ListElem *iter_bw()
  //     positioniert den Zeiger auf das vohergehende Element
  //     liefert am Anfang der Liste 0

    template<class T> class Liste;

    template<class T>
    std::ostream& operator<< (std::ostream& OS, Liste<T>& List)
    {
      List.Ausgabe (OS);
      return OS;
    }


    template<class Tt> class Liste
    {
        friend std::ostream& operator<< <> (std::ostream& OS, Liste<Tt>& List) ;

        private:
            // Interne Klasse: So ist keine Freunddeklaration noetig und
            // ListElem ist nicht im ganzen Programm sichtbar.

            class ListElem {
            public:
                ListElem(const Tt& e, ListElem* v = 0, ListElem* n = 0)
                : element(e), vorg(v), nachf(n)
                {
    #ifdef __LISTE_PARANOIA__
                    if(vorg)
                    {
                        if(vorg->nachf!=this) INT_ERR(__LINE__);
                    }
                    if(nachf)
                    {
                        if(nachf->vorg!=this) INT_ERR(__LINE__);
                    }
    #endif
                }

                Tt element;
                ListElem* vorg;
                ListElem* nachf; // dbl lst.
            };

      public:
        Liste() : start(0), anzahl(0), aktZgr(0), aktInd(0)   { anfang(); }

        virtual ~Liste() { dest(); }
        Liste(const Liste& m) : start(0), anzahl(m.anzahl), aktZgr(0), aktInd(0) { copy(m); }

        virtual int fuegeEin(const Tt&, int = 0);

        int fuegeEin(const Liste<Tt>& AL, int Pos = 0) ;

        virtual int loesche(int);

        Liste& operator= (const Liste&);
        void operator+=  (const Liste<Tt>& AL)
        {
            if (!fuegeEin (AL, laenge ()))    INT_ERR (__LINE__);
        }
        int operator !() const
        {
            return (anzahl == 0);
        }
        operator int() const
        {
            return !(anzahl == 0);
        }

        int operator == (const Liste<Tt>&) const;
        int operator != (const Liste<Tt>& L) const
        {
            return !(Liste<Tt>::operator == (L));
        }

        virtual const Tt& operator[](int i) const
        { return hole_element(i); }

        virtual Tt& operator[](int i)
        { return hole_element(i); }

        int laenge() const { return anzahl; }
        virtual void Ausgabe (std::ostream&);
        int istElement(const Tt&, int =0) const;
        int Referenz2Index(const Tt&) const;

        #ifdef __LISTE_PARANOIA__
          void ParanoiaCheck() const;
        #endif

      protected:

        void anfang() const 
        { 
          *((int *)(&aktInd)) = 0; 
          *((ListElem**)(& aktZgr)) = start; 
        }

        void iter_fw() const 
        { 
          if(aktZgr != 0) 
          {
            *((int *)(&aktInd)) += 1; 
            *((ListElem**)(& aktZgr)) = aktZgr->nachf;
          }
          else INT_ERR(__LINE__);
        }

        void iter_bw() const 
        { 
          if(aktZgr != 0) 
          {
            *((int *)(&aktInd)) -= 1; 
            *((ListElem**)(& aktZgr)) = aktZgr->vorg;
          }
          else INT_ERR(__LINE__);
        }

        Tt& hole_element(int i) const;

      private:
        void dest();
        void copy(const Liste&);
        ListElem* start;
        int anzahl;
        ListElem* aktZgr;
        int aktInd;
    };

    template<class Tt> void Liste<Tt>::dest()
    {
      while (start != 0)
      {
        ListElem *tmp = start->nachf;
        delete start;
        start = tmp;
      }
    }

    template<class Tt> void Liste<Tt>::copy(const Liste<Tt>& m)
    {
      if ((anzahl = m.anzahl) == 0)
      {
        anfang();
        return;
      }

      aktInd = m.aktInd;
      ListElem *mZgr   = m.start;
      ListElem *neuZgr = aktZgr = start = new ListElem(mZgr->element);

      while((mZgr = mZgr->nachf) != 0)
      {
        neuZgr->nachf = new ListElem(mZgr->element);
        neuZgr->nachf->vorg = neuZgr;
        neuZgr = neuZgr->nachf;
        if (mZgr == m.aktZgr) aktZgr = neuZgr;
      }
      #ifdef __LISTE_PARANOIA__
        ParanoiaCheck();
      #endif
    }

    template<class Tt> Liste<Tt>& Liste<Tt>::operator=(const Liste<Tt>& m)
    {
      if (this == &m)
         return *this;
      dest();
      copy(m);
      return *this;
    }

    template<class Tt> int Liste<Tt>::fuegeEin(const Tt& x, int i)
    {
      const int oldInd = aktInd;
      if (i < 0 || i > anzahl)
      {
        fprintf(stderr,"\n Falscher Index.");
        fprintf(stderr,"\n Es wurde versucht ein Element an %d.Stelle einer Liste mit %d Elementen einzufuegen.\n",
         i, laenge());
        INT_ERR(__LINE__);
      }
      ListElem *neu = new ListElem(x);
      if (neu == 0)
      {
        fprintf(stderr,"\n Speicherfehler.");
        fprintf(stderr,"\n Es wurde versucht ein Listen-Element fuer eine Liste mit %d Elementen zu erzeugen.\n",
         laenge());
        INT_ERR(__LINE__);
      }

      if (i == 0)
      {
        neu->nachf = start;
        neu->vorg = 0;
        if (start) start->vorg = neu;
        start = neu;
        anfang();
      }
      else
      {
        i--; // Das Element 'ein' vor dem Ziel anfahren !
        if (i < oldInd)
          for (int k = oldInd; k > i; k--) iter_bw();
        else if (i > oldInd)
          for (int k = oldInd; k < i; k++) iter_fw();
        neu->nachf = aktZgr->nachf;
        neu->vorg = aktZgr;
        if (aktZgr->nachf) aktZgr->nachf->vorg = neu;
        aktZgr->nachf = neu;
      }
      anzahl++;
      #ifdef __LISTE_PARANOIA__
        ParanoiaCheck();
      #endif
      return 1;
    }

    template<class Tt>
    int Liste<Tt>::fuegeEin(const Liste<Tt>& AL, int Pos )
    {
      for (int i = AL.laenge ()-1; i >= 0; i--)
      {
        if(!fuegeEin (AL[i], Pos)) return 0;
      }
      #ifdef __LISTE_PARANOIA__
        ParanoiaCheck();
      #endif
      return 1;
    }

    template<class Tt> int Liste<Tt>::loesche(int i)
    {
      const int oldInd = aktInd;
      if (i < 0 || i >= anzahl )
      {
        fprintf(stderr,"\n Falscher Index.");
        fprintf(stderr,"\n Es wurde auf ein Element an %d.Stelle einer Liste mit %d Elementen zu loeschen.\n",
         i, laenge());
        INT_ERR(__LINE__);
      }

      if (i < oldInd)
      {
        for (int k = oldInd; k > i; k--)
          iter_bw();
      }
      else if (i > oldInd)
      {
         for(int k = oldInd; k < i; k++)
          iter_fw();
      }
      ListElem *NachfZgr=aktZgr->nachf;
      ListElem *VorgZgr =aktZgr->vorg;
      if (aktZgr == start)
        start = NachfZgr;
      else
        VorgZgr->nachf = NachfZgr;
      if (NachfZgr) NachfZgr->vorg = VorgZgr;
      delete aktZgr;
      anzahl--;
      if (NachfZgr)
        aktZgr=NachfZgr;
      else if (VorgZgr)
      {
        aktZgr=VorgZgr;
        aktInd--;
      }
      else anfang();
      assert ( aktInd >= 0 );
      assert ( ( laenge()==0 && aktInd==0 && aktZgr==nullptr && start==nullptr ) ||
               ( laenge()>0 && aktZgr!=nullptr && start!=nullptr )
             ) ;
      #ifdef __LISTE_PARANOIA__
        ParanoiaCheck();
      #endif
      return 1;
    }

      template<class Tt>
      int Liste<Tt>::operator==(const Liste<Tt>& L) const
      {
        if (this == &L) return 1;
        if (this->laenge() != L.laenge()) return 0;

        int i= laenge()-1;
        while (i >= 0 && ((*this)[i] == L[i])) i--;
        return i == -1;
      }

    template<class Tt> Tt& Liste<Tt>::hole_element(int i) const
    {
      const int oldInd = aktInd;
      if (i < 0 || i >= anzahl)
      {
        fprintf(stderr,"\n Falscher Index.");
        fprintf(stderr,"\n Es wurde auf ein Element an %d.Stelle einer Liste mit %d Elementen zugegriffen.\n",
           i, laenge());
        INT_ERR(__LINE__);
      }

      if (i < oldInd)
      {
        for (int k = oldInd; k > i; k--)
          iter_bw();
      }
      else if (i > oldInd)
      {
        for(int k = oldInd; k < i; k++)
          iter_fw();
      }
      #ifdef __LISTE_PARANOIA__
        ParanoiaCheck();
      #endif
      return aktZgr->element;
    }

    template<class Tt> int Liste<Tt>::istElement(const Tt& x, int i) const
    {
      for ( ; i < anzahl; i++)
      {
        if ((*this)[i] == x) return i;
      }
      return -1;
    }

    template<class Tt> int Liste<Tt>::Referenz2Index(const Tt& a) const
    {
      int i=laenge()-1;
      while(i>=0 && &a!=&(*this)[i]) i--;
      return i;
    }

    #ifdef __LISTE_PARANOIA__
      template<class Tt> void Liste<Tt>::ParanoiaCheck() const
      {
        ListElem *pi=start;
        int i=0;
        while ( pi )
        {
          i++;
          if(pi->vorg!=nullptr)
          {
            if(pi->vorg->nachf!=pi)
              INT_ERR(__LINE__);
          }
          if(pi->nachf!=nullptr)
          {
            if(pi->nachf->vorg!=pi)
              INT_ERR(__LINE__);
          }
          pi=pi->nachf;
        }
        if(i!=anzahl)
          INT_ERR(__LINE__);
      }
    #endif

    template<class T> void Liste<T>::Ausgabe (std::ostream& OS)
    {
      const int Zeilenlaenge = 10;
  
      OS << "( " << laenge() << " ) < ";
      for (int i=0; i<laenge(); i++) {
        if (i % Zeilenlaenge == 0 &&  i)   OS << "\n\t";
  
        OS << (*this)[i];
        if (i % Zeilenlaenge != Zeilenlaenge-1 && i != laenge()-1)
  	  OS << ", ";
      }
      OS << " >";
    }
  
# endif
