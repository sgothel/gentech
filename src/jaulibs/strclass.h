/*************************************************************************/
// author : Sven Gothel - Stapenhorststrasse 35a - 33615 Bielefeld
//
/*
 *  Datei           : strclass.h
 *  Inhalt          : Deklaration der Klasse :
 *                        o StrClass
 *  Programmierer   : Sven Gothel
 *  Erstellungsdatum: 05.05.1994
 *
 *  2019-12-17 Sven Gothel: Holding transient string length, 
 *             removing file dependencies and some cleanup
 *
 *  $Log: strclass.h $
 *  Revision 1.4  1995/04/07 16:54:58  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.3  1995/04/04 10:37:37  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.2  1994/12/15 15:28:30  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.1  1994/12/15 15:18:46  Sven_Gothel
 *  Initial revision
 *
 */
/*************************************************************************/

# ifndef _STRCLASS_H
  # define _STRCLASS_H

  # include <iostream>

  class StrClass {

  friend std::ostream& operator<< (std::ostream& OS, StrClass& str);

  public:
     StrClass() : StrPtr(nullptr), m_length(0) {}
     StrClass(const StrClass &m) { copy(m); }
     StrClass(const char *m) { copy(m); }
     ~StrClass() { dest(); }
     StrClass& operator=(const StrClass&);
     // not using the implicit conversion, because the costs of run-time ...
     StrClass& operator=(const char *);
     void strcat(const StrClass &m);
     // the standarised operator at Bjarne Stroustrup's suggestion.
     StrClass& operator+=(const StrClass &m) { strcat(m); return *this; }
     int operator==(const StrClass &m) const;
     int operator!=(const StrClass &m) const { return !(*this==m); }
     int operator>=(const StrClass &m) const;
     int operator<=(const StrClass &m) const;
     int operator>(const StrClass &m) const;
     int operator<(const StrClass &m) const;
     operator const char *(void) const { return StrPtr; }
     // operator char *(void) { return StrPtr; }
     char & operator[](int i) { return StrPtr[i]; }
     char operator[](int i) const { return StrPtr[i]; }
     int length(void) const { return m_length; }
     operator unsigned int() { return (unsigned int)m_length; }

     // rather old, but still a member-function because of compatibility ... :
     char *getStrPtr(void) { return StrPtr; }
     const char *getStrPtr(void) const { return StrPtr; }
     
     //
     // arbitrary ..
     //
     void assignDouble(double m);

     //
     // string manipulation
     //
     int chk4SubStr(const StrClass &SubStr, int StartIndex=0);  // return : index of found SubStr or -1
     
     int delSubStr(const StrClass &SubStr, int StartIndex=0);  // return : index of deleted substr or -1
     int delAllSubStr(const StrClass &SubStr); // return number of deleted sub strings ..
   
     bool insertSubStr(const StrClass &SubStr, int Index);
   
     int substSubStr(const StrClass &SubStr, const StrClass &Substitute, int StartIndex=0); // return: index of subst or -1
     int substAllSubStr(const StrClass &SubStr, const StrClass &Substitute); // return number of substitutions
   
     void delAllXtraWhiteSpaces();
     
  private:
     void dest();
     void copy(const char *);
     void copy(const StrClass &);
     char *StrPtr; 
     int m_length;
  };

  // the global operator at Bjarne Stroustrup's suggestion.
  StrClass operator+(const StrClass &m, const StrClass &n) ; 

  std::ostream& operator<< (std::ostream& OS, StrClass& str);

# endif
