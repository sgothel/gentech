// author : Sven Gothel - Stapenhorststrasse 35a - 33615 Bielefeld
//
// sortlist.h    
//
// Jegliches Copyright aller Dateien ist im Besitz der Autoren.
// Sven Gothel * Stapenhorststr.35a * 33615 Bielefeld * 0521/139228
// Bielefeld, den 11.3.1994.
//
// Diese Datei enthaelt eine Erweiterung der Template-Klasse Liste.
// In der hier deklarierten Template-Klasse 'SortListe' werden die
// einzelnen Listen Elemente sofort beim Einfuegen der Groesse nach
// sortiert ( In aufsteigender(UP)/absteigender(DOWN) Reihenfolge ).
// D.h. die Methode 'fuegeEin' fuegt die Listenelemente mittels
// Bisektionierung gleich sortiert ein.
// fuegeEin liefert nun den Index zurueck, den das eingefuegte Element
// besitzt, ansonsten npos.
// die Listenelemente gleich sortiert eingefuegt werden, mittel Bisektionierung.
//
// Neu ist die Methode findeElement, welche ebenfalls nach der MEthode der 
// Bisektionierung arbeitet.
/*
 *
 *  $Log: sortlist.h $
 *  Revision 1.4  1995/04/07 16:54:56  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.3  1995/04/04 10:37:35  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.2  1994/12/15 15:28:23  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.1  1994/12/15 15:18:43  Sven_Gothel
 *  Initial revision
 *
*/

# ifndef _SORTLISTE_H
  # define _SORTLISTE_H

  # include "liste.h"

  template <class Value_type, typename Size_type = uint_fast32_t> class SortListe : protected Liste<Value_type, Size_type> {
      public:
          typedef Value_type value_type;
          typedef Size_type  size_type;

          // Spezielle Zugriffsrechte fuer SortListe
          using Liste<value_type, size_type>::erase;
          using Liste<value_type, size_type>::operator !;
          using Liste<value_type, size_type>::operator[];
          using Liste<value_type, size_type>::size;
          using Liste<value_type, size_type>::contains;
          using Liste<value_type, size_type>::npos;

          enum order_t { UP, DOWN };
          SortListe(order_t order=UP) : Liste<value_type>(), m_order(order) { }
          SortListe(const SortListe& m) : Liste<value_type>(m), m_order(m.m_order) {}
          ~SortListe() override { }

          SortListe& operator=(const SortListe &m)
          {
              m_order=m.m_order;
              Liste<value_type>::operator=(m);
              return *this;
          }
          // fuegeEin RETURN : neuer Index, oder npos fuer Fehler
          size_type insert(const value_type& a) {
              size_type u=0, o=size()>0 ? size()-1 : 0;
              size_type i=findeIndex(a, u, o);
              if ( npos == i ) {
                  Liste<value_type>::insert(o, a);
                  return o;
              } else {
                  Liste<value_type>::insert(i, a);
                  return i;
              }
          }
          size_type findeElement(const value_type& x) const {
              size_type u=0, o=size()>0 ? size()-1 : 0;
              return findeIndex(x, u, o);
          }

      private:
          /**
              - Beschreibung
              --------------
              Rueckgabe des Index oder npos.
              Rueckgabe der unteren, u, und oberen, o, Grenze
              der letzten Bisektionierung.
              Die Grenze o wird bei nichtauffinden des index derart gesetzt, dass
              das Element x dort eingesetzt werden  koennte !

              - Parameter
              -----------
              E:        x   : Die zu suchende StrClass
              E/A:    u : Start der unteren Grenze, RETURN siehe oben
              E/A:    o : Start der oberen Grenze, RETURN siehe oben


              - allgemeine Ein- und Ausgabeeinheiten
              --------------------------------------
              keine

              - Returncode
              ------------
              Der Index des gefundenen Elementes oder npos.

              - Historie
              ----------
              12.8.1994 Sven Gothel

           * @param x
           * @param u
           * @param o
           * @return >= 0 gefunden index,  npos nicht gefunden
           */
          size_type findeIndex(const value_type &x, size_type &u, size_type &o) const {
              size_type i=0;

              // da gibt es nix ....
              if ( size() == 0 ) { return npos; }

              //Anfangsgrenzen austesten
              if(m_order==UP) {
                  if( u == o && (*this)[o] < x ) { o++; }
                  else if( (*this)[u] > x   )  { o=u;           }
                  else if( (*this)[o] < x   )  { u=o; o++;      }
                  else if( (*this)[u] == x  )  { return u;   }
                  else if( (*this)[o] == x  )  { return o;   }
              } else {
                  if( u == o && (*this)[o] > x ) { o++; }
                  else if( (*this)[u] < x   )  { o=u;           }
                  else if( (*this)[o] > x   )  { u=o; o++;      }
                  else if( (*this)[u] == x  )  { return u; }
                  else if( (*this)[o] == x  )  { return o; }
              }

              // bisektionieren bis kein Element zwischen den grenzen
              while( o-u > 1 ) {
                  i=(u+o)/2;
                  if ( (*this)[i] < x ) {
                      if ( m_order==UP ) u=i;
                      else o=i;
                  } else if ( (*this)[i] > x ) {
                      if ( m_order==UP ) o=i;
                      else u=i;
                  } else {
                      return i;
                  }
              }
              return npos;
          }
          order_t m_order;
  };

# endif


