// author : Sven Gothel - Stapenhorststrasse 35a - 33615 Bielefeld
//
// menge.h				
//
// Original von
// Martin Schader & Stefan Kuhlins : Programmieren in C++, Springer Verlag,
// ISBN-3-540-56524-8
//
// Kapitel 18, Aufgabe 9
//
/*
 *
 *  $Log: menge.h $
 *  Revision 1.4  1995/04/07 16:54:53  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.3  1995/04/04 10:37:29  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.2  1994/12/15 15:28:16  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.1  1994/12/15 15:18:42  Sven_Gothel
 *  Initial revision
 *
*/

#ifndef _MENGE_H
    #define _MENGE_H

    #include <iostream>
    #include <limits>
    #include "liste.h"      // Template-Liste

    template<class T> class Menge {
        public:
            typedef typename Liste<T>::size_type size_type;

            /**
             * Special value representing maximal value of size_type, used to denote an invalid index position return value, i.e. `no position`.
             */
            static constexpr const size_type npos = std::numeric_limits<size_type>::max();

            bool leer() const { return l.laenge() == 0; }
            size_type card() const { return l.laenge(); }
            size_type istElement(const T& x) {
                for (size_type i = 0; i < card(); ++i) {
                    if (l[i] == x) {
                        return i;
                    }
                }
                return npos;
            }
            bool fuegeEin(const T& x) {
                return ( npos != istElement(x) ) ? false : l.fuegeEin(x);
            }
            bool loesche(const T& x) {
                const int pos = istElement(x);
                if( npos != pos ) {
                    return l.loesche(pos);
                } else {
                    return false;
                }
            }
            void inhalt() const {
                std::cout << "{ ";
                if (!leer()) {
                    std::cout << l[0];
                    for (int i=1; i<card(); i++) {
                        std::cout << ", " << l[i];
                    }
                }
                std::cout << " }" << std::endl;
            }

            Menge operator*(Menge& a) const {
                Menge<T> axb;
                for (int i=0; i<card(); i++) {
                    if (a.istElement(l[i]) >= 0) {
                        axb.fuegeEin(l[i]);
                    }
                }
                return axb;
            }
            const T& operator[](int i) const { return l[i]; }
            T& operator[](int i) { return l[i]; }

        private:
            Liste<T> l;
    };

  // TESTTEIL DER MENGE ::
  /*
  int main()
  {
    Menge<int> m1;
    m1.fuegeEin(127);
    m1.fuegeEin(214);
    m1.fuegeEin(63);
    cout << "m1 = "; m1.inhalt();

    Menge<int> m2;
    m2.fuegeEin(127);
    m2.fuegeEin(210);
    m2.fuegeEin(63);
    cout << "m2 = "; m2.inhalt();

    const Menge<int> x = m1*m2;
    cout << "m1*m2 = "; x.inhalt();
    return 0;
  }

  */

# endif
