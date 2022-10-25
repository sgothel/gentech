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

# ifndef _MENGE_H
  # define _MENGE_H

  #include <iostream.h>
  #include "liste.h"      // Template-Liste

  template<class T> class Menge {
  public:
    int leer() const { return l.laenge() == 0; }
    int card() const { return l.laenge(); }
    int istElement(const T&);
    int fuegeEin(const T&);
    int loesche(const T&);
    void inhalt() const;
    Menge operator*(Menge&) const;
  private:
    Liste<T> l;
  };

  template<class T> int Menge<T>::fuegeEin(const T& x)
  {
    return (istElement(x) >= 0) ? 0 : l.fuegeEin(x);
  }

  template<class T> int Menge<T>::loesche(const T& x)
  {
    int pos = istElement(x);
    if (pos >= 0)
      l.loesche(pos);
    return pos >= 0;
  }

  template<class T> int Menge<T>::istElement(const T& x)
  {
    for (int i = 0; i < card(); i++)
      if (l[i] == x)
	return i;
    return -1;
  }

  template<class T> void Menge<T>::inhalt() const
  {
    cout << "{ ";
    if (!leer()) {
      cout << l[0];
      for (int i=1; i<card(); i++)
	cout << ", " << l[i];
    }
    cout << " }" << endl;
  }

  template<class T> Menge<T> Menge<T>::operator*(Menge<T>& a) const
  {
    Menge<T> axb;
    for (int i=0; i<card(); i++)
      if (a.istElement(l[i]) >= 0)
	axb.fuegeEin(l[i]);
    return axb;
  }

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
