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

#ifndef _LISTE_H
  #define _LISTE_H

  #include <iostream>
  #include <limits>
  #include <type_traits>

  #include <cstdlib>
  #include <cstdio>
  #include <assert.h>
  #include "interror.h"

  // Klasse Liste
  // Der Typ der Listenelemente ist beliebig
  //
  // 1. Element hat den Index 0 !!!!!
  //
  // Elementfunktionen:
  //
  // Falsche Indexoperatoren rufen exit(1), mit Angabe der Zeilennummer auf.
  //
  // int fuegeEin(const value_type& x, int i)
  //     Erzeugt ein neues Listenelement, welches <value_type> beinhaltet
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
  //  istElement(const value_type&, int =0) somit ist ein Test ob das Element
  //  schon vorhanden ist moeglich.
  //  Es wird von i bis laenge-1 gesucht.
  //  RETURN : ListenIndize, wenn gefunden;
  //           bzw npos, wenn nicht gefunden !!!
  //
  // int Referenz2Index(const value_type& a)
  // Es wird ein Element mit der Referenz a in der Liste gesucht !
  // RETURN : npos  : nicht gefunden.
  //          !=npos : gefunden, index des Elementes.
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

    // #define __LISTE_PARANOIA__ 1

    template<typename Value_type, typename Size_type = uint_fast32_t,
             std::enable_if_t<std::is_integral_v<Size_type> &&
                              std::is_unsigned_v<Size_type>, bool> = true>
    class Liste
    {
        public:
            // typedefs' for C++ named requirements: Container

            typedef Value_type                                  value_type;
            typedef value_type*                                 pointer;
            typedef const value_type*                           const_pointer;
            typedef value_type&                                 reference;
            typedef const value_type&                           const_reference;
            typedef value_type*                                 iterator;
            typedef const value_type*                           const_iterator;
            typedef Size_type                                   size_type;
            typedef typename std::make_signed<size_type>::type  difference_type;
            // typedef std::reverse_iterator<iterator>          reverse_iterator;
            // typedef std::reverse_iterator<const_iterator>    const_reverse_iterator;

            /**
             * Special value representing maximal value of size_type, used to denote an invalid index position return value, i.e. `no position`.
             */
            static constexpr const size_type npos = std::numeric_limits<size_type>::max();

        private:
            // Interne Klasse: So ist keine Freunddeklaration noetig und
            // ListElem ist nicht im ganzen Programm sichtbar.

            class ListElem {
            public:
                ListElem(const value_type& e, ListElem* v = 0, ListElem* n = 0)
                : m_elem(e), m_pred(v), m_next(n)
                {
                    #ifdef __LISTE_PARANOIA__
                    if(m_pred) {
                        if(m_pred->m_next!=this) {
                            INT_ERR(__LINE__);
                        }
                    }
                    if(m_next) {
                        if(m_next->m_pred!=this) {
                            INT_ERR(__LINE__);
                        }
                    }
                    #endif
                }

                value_type m_elem;
                ListElem* m_pred;
                ListElem* m_next; // dbl lst.
            };

      public:
        Liste() noexcept : m_start(0), m_count(0), m_current_elem(0), m_current_idx(0)   { set_to_start(); }

        virtual ~Liste() noexcept { dest(); }
        Liste(const Liste& m) noexcept : m_start(0), m_count(m.m_count), m_current_elem(0), m_current_idx(0) { copy(m); }

        virtual void insert(const size_type pos, const value_type& x) noexcept  {
            fuegeEin(x, pos);
        }
        virtual bool fuegeEin(const value_type& x, size_type idx = 0) noexcept {
            const size_type old_idx = m_current_idx;
            if( idx > m_count ) {
                fprintf(stderr,"\n Falscher Index.");
                fprintf(stderr,"\n Es wurde versucht ein Element an %zu.Stelle einer Liste mit %zu Elementen einzufuegen.\n", (size_t)idx, (size_t)size());
                INT_ERR(__LINE__);
            }
            ListElem *elem_x = new ListElem(x);
            if( nullptr == elem_x ) {
                fprintf(stderr,"\n Speicherfehler.");
                fprintf(stderr,"\n Es wurde versucht ein Listen-Element fuer eine Liste mit %zu Elementen zu erzeugen.\n", (size_t)size());
                INT_ERR(__LINE__);
            }

            if( 0 == idx ) {
                elem_x->m_next = m_start;
                elem_x->m_pred = 0;
                if (m_start) m_start->m_pred = elem_x;
                m_start = elem_x;
                set_to_start();
            } else {
                --idx; // Das Element 'ein' vor dem Ziel anfahren !
                if( idx < old_idx ) {
                    for (size_type k = old_idx; k > idx; k--) {
                        iter_bw();
                    }
                } else if( idx > old_idx ) {
                    for (size_type k = old_idx; k < idx; k++) {
                        iter_fw();
                    }
                }
                elem_x->m_next = m_current_elem->m_next;
                elem_x->m_pred = m_current_elem;
                if (m_current_elem->m_next) {
                    m_current_elem->m_next->m_pred = elem_x;
                }
                m_current_elem->m_next = elem_x;
            }
            ++m_count;
            #ifdef __LISTE_PARANOIA__
                ParanoiaCheck();
            #endif
            return true;
        }

        bool fuegeEin(const Liste& AL, size_type idx = 0) noexcept {
            for (size_type j = 0; j < AL.size(); ++j) {
                if(!fuegeEin (AL[j], idx+j)) {
                    return false;
                }
            }
            #ifdef __LISTE_PARANOIA__
                ParanoiaCheck();
            #endif
            return true;
        }

        virtual bool loesche(size_type i) noexcept {
            const size_type oldInd = m_current_idx;
            if (i >= m_count )
            {
                fprintf(stderr,"\n Falscher Index.");
                fprintf(stderr,"\n Es wurde auf ein Element an %zu.Stelle einer Liste mit %zu Elementen zu loeschen.\n",
                        (size_t)i, (size_t)size());
                INT_ERR(__LINE__);
            }

            if (i < oldInd) {
                for (size_type k = oldInd; k > i; k--) {
                    iter_bw();
                }
            } else if (i > oldInd) {
                for(size_type k = oldInd; k < i; k++) {
                    iter_fw();
                }
            }
            ListElem *NachfZgr=m_current_elem->m_next;
            ListElem *VorgZgr =m_current_elem->m_pred;
            if (m_current_elem == m_start) {
                m_start = NachfZgr;
            } else {
                VorgZgr->m_next = NachfZgr;
            }
            if (NachfZgr) { NachfZgr->m_pred = VorgZgr; }
            delete m_current_elem;
            m_count--;
            if (NachfZgr) {
                m_current_elem=NachfZgr;
            } else if (VorgZgr) {
                m_current_elem=VorgZgr;
                m_current_idx--;
            } else {
                set_to_start();
            }
            assert ( m_current_idx >= 0 );
            assert ( ( size()==0 && m_current_idx==0 && m_current_elem==nullptr && m_start==nullptr ) ||
                    ( size()>0 && m_current_elem!=nullptr && m_start!=nullptr )
            ) ;
            #ifdef __LISTE_PARANOIA__
                ParanoiaCheck();
            #endif
            return true;
        }

        Liste& operator= (const Liste& m) {
            if (this == &m) {
               return *this;
            }
            dest();
            copy(m);
            return *this;
        }

        void operator+=  (const Liste& AL) noexcept
        {
            if (!fuegeEin (AL, size ()))    INT_ERR (__LINE__);
        }
        bool operator !() const noexcept
        {
            return (m_count == 0);
        }
        operator bool() const noexcept
        {
            return !(m_count == 0);
        }

        bool operator == (const Liste& L) const noexcept {
            if (this == &L) return true;
            if (this->size() != L.size()) return false;

            size_type i = 0;
            while (i < size() && ((*this)[i] == L[i])) { ++i; }
            return i == size();
        }
        bool operator != (const Liste& L) const noexcept {
            return !(Liste::operator == (L));
        }

        virtual const value_type& operator[](size_type i) const noexcept
        { return get_element(i); }

        virtual value_type& operator[](size_type i) noexcept
        { return get_element(i); }

        size_type laenge() const noexcept { return m_count; }
        size_type size() const noexcept { return m_count; }

        virtual void Ausgabe (std::ostream& OS) const noexcept {
            const size_type Zeilenlaenge = 10;

            OS << "( " << size() << " ) < ";
            for (size_type i=0; i<size(); i++) {
                if (i % Zeilenlaenge == 0 &&  i)   OS << "\n\t";

                OS << (*this)[i];
                if (i % Zeilenlaenge != Zeilenlaenge-1 && i != size()-1) {
                    OS << ", ";
                }
            }
            OS << " >";
        }

        bool istElement(const value_type& x, size_type i=0) const noexcept {
            for ( ; i < m_count; i++) {
                if ((*this)[i] == x) {
                    return true;
                }
            }
            return false;
        }

        /**
         * Returns index of given element if found or size()
         * @param a
         * @return
         */
        size_type Referenz2Index(const value_type& a) const noexcept {
            size_type i = 0;
            while(i < size() && &a!=&(*this)[i]) { i--; }
            return i;
        }

        #ifdef __LISTE_PARANOIA__
          void ParanoiaCheck() const noexcept {
              ListElem *pi=m_start;
              size_type i=0;
              while ( pi ) {
                  i++;
                  if(pi->m_pred!=nullptr)
                  {
                      if(pi->m_pred->m_next!=pi) {
                          INT_ERR(__LINE__);
                      }
                  }
                  if(pi->m_next!=nullptr)
                  {
                      if(pi->m_next->m_pred!=pi) {
                          INT_ERR(__LINE__);
                      }
                  }
                  pi=pi->m_next;
              }
              if(i!=m_count) {
                  INT_ERR(__LINE__);
              }
          }
        #endif

      protected:

        void set_to_start() const noexcept {
            m_current_idx = 0;
            m_current_elem = m_start;
        }

        void iter_fw() const noexcept {
            if(m_current_elem != 0 && m_current_idx < npos ) {
                m_current_idx++;
                m_current_elem = m_current_elem->m_next;
            } else {
                INT_ERR(__LINE__);
            }
        }

        void iter_bw() const noexcept {
            if(m_current_elem != 0 && m_current_idx > 0) {
                --m_current_idx;
                m_current_elem = m_current_elem->m_pred;
            } else {
                INT_ERR(__LINE__);
            }
        }

        value_type& get_element(const size_type idx) const noexcept {
            const size_type oldInd = m_current_idx;
            if ( idx >= m_count ) {
                fprintf(stderr,"\n Falscher Index.");
                fprintf(stderr,"\n Es wurde auf ein Element mit index %zu einer Liste mit %zd Elementen zugegriffen.\n",
                        (size_t)idx, (size_t)size());
                INT_ERR(__LINE__);
            }

            if (idx < oldInd) {
                for (size_type k = oldInd; k > idx; k--) {
                    iter_bw();
                }
            } else if (idx > oldInd) {
                for(size_type k = oldInd; k < idx; k++) {
                    iter_fw();
                }
            }
            #ifdef __LISTE_PARANOIA__
                ParanoiaCheck();
            #endif
            return m_current_elem->m_elem;
        }

      private:
        void dest() noexcept {
            while( m_start != 0 ) {
                ListElem *tmp = m_start->m_next;
                delete m_start;
                m_start = tmp;
            }
        }

        void copy(const Liste& o) {
            if( (m_count = o.m_count) == 0 ) {
                set_to_start();
                return;
            }
            m_current_idx = o.m_current_idx;
            ListElem *o_iter   = o.m_start;
            ListElem *new_le = m_current_elem = m_start = new ListElem(o_iter->m_elem);

            while((o_iter = o_iter->m_next) != 0)
            {
                new_le->m_next = new ListElem(o_iter->m_elem);
                new_le->m_next->m_pred = new_le;
                new_le = new_le->m_next;
                if (o_iter == o.m_current_elem) {
                    m_current_elem = new_le;
                }
            }
            #ifdef __LISTE_PARANOIA__
                ParanoiaCheck();
            #endif
        }

        ListElem* m_start;
        size_type m_count;
        mutable ListElem* m_current_elem;
        mutable size_type m_current_idx;
    };

    template<class T>
    std::ostream& operator<< (std::ostream& OS, Liste<T>& List)
    {
      List.Ausgabe (OS);
      return OS;
    }

#endif
