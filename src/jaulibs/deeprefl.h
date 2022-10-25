
/*  
  $Log: deeprefl.h $
  Revision 1.6  1995/04/07 16:54:46  Sven_Gothel
  *** empty log message ***
  Revision 1.5  1995/04/05 00:26:17  Sven_Gothel
  oups
  Revision 1.4  1995/04/04 20:51:58  Sven_Gothel
  __NOT_ANSI_Cplusplus__ zur static_cast umgehung eingebaut ...
  Revision 1.3  1995/04/04 10:37:21  Sven_Gothel
  *** empty log message ***
  Revision 1.2  1994/12/15 15:27:48  Sven_Gothel
  the very first check in !!!
  all files contains the log message now !!!
  Sven Gothel, Bielefeld 14.12.1994 !!!
  
  Revision 1.1  1994/12/15 15:18:35  Sven_Gothel
  Initial revision
 // author : Sven Gothel - Stapenhorststrasse 35a - 33615 Bielefeld
*/

#ifndef _DEEPREFL_H
  #define _DEEPREFL_H

  #include "liste.h"
  #include <limits.h>

  template<class Tt>
  class DeepRefListe : protected Liste<Tt *>
  {
    public:
      DeepRefListe () : Liste<Tt *>() {}
      DeepRefListe (const DeepRefListe &a) { DeepCopy(a); }
      virtual ~DeepRefListe () { DeepDest(); }

      virtual DeepRefListe * clone ()
      { return new DeepRefListe(*this); }

      Liste<Tt *>::laenge;

      int fuegeEin(Tt * const & pa, int Pos = 0)
      { return Liste<Tt *>::fuegeEin(pa, Pos); }

      int fuegeEin(Tt &a, int Pos = 0)
      { Tt *p=a.clone(); return fuegeEin(p, Pos); }

      Tt& operator[](long idx) const
      { 
        if(idx>INT_MAX) INT_ERR(__LINE__);
        #ifdef __NOT_ANSI_Cplusplus__
          return *(Liste<Tt *>::operator[]((int)idx)); 
        #else
          return *(Liste<Tt *>::operator[](static_cast<int>(idx))); 
        #endif
      }

      int loesche(int idx)
      { delete Liste<Tt *>::operator[](idx); return Liste<Tt *>::loesche(idx); }

      DeepRefListe& operator= (const DeepRefListe& a)
      {
        if(this!=&a)
        {
          DeepDest();
          DeepCopy(a);
        }
        return *this;
      }

      int operator==( const DeepRefListe &a ) const ;

    private:

      void DeepCopy (const DeepRefListe &a);
      void DeepDest ();
  };

  template<class Tt>
  int DeepRefListe<Tt>::operator==( const DeepRefListe<Tt> &a ) const
  {
    if ( laenge() != a.laenge() ) return 0;
    int i;
    for ( i=0; 
          i<laenge() && 
          (*this)[i]==a[i]; 
          i++
        ) ;
    return i==laenge();
  }

  template<class Tt>
  void DeepRefListe<Tt>::DeepDest ()
  { for(int i=0; i<laenge(); i++) delete Liste<Tt *>::operator[](i); }

  template<class Tt>
  void DeepRefListe<Tt>::DeepCopy (const DeepRefListe<Tt> &a)
  {
    for(int i=0; i<a.laenge(); i++)
      fuegeEin(a[i], i);
  }

  /* TESTTEIL

    #include <iostream.h>

    class A
    {
      public:
        virtual ~A() { cout << "Destruktor von A !" << endl ; }
        virtual A* clone() { return new A(*this); }
        int operator==(const A&) { return 0; }
      protected:
        A() {};
    };

    class B : public A
    {
      public:
        virtual ~B() { cout << "Destruktor von B !" << endl ; }
        virtual A* clone() { return new B(*this); }
        int operator==(const B&) { return 0; }
    };


    int main ()
    {
      A *pa1=new B;
      A *pa2=new B;

      DeepRefListe<A> al;

      al.fuegeEin(pa1);
      al.fuegeEin(pa2);

      DeepRefListe<A> bl=al;

      cout << "Voila, die Destruktoren :\n";

      return 0;
    }

  */

#endif

