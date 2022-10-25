// author : Sven Gothel - Stapenhorststrasse 35a - 33615 Bielefeld
//
/*************************************************************************/
/*
 *  Datei           : strclass.cc
 *  Inhalt          : Definition fuer Funktionen der Klasse :
 *                        o StrClass
 *  Programmierer   : Sven Gothel
 *                  
 *  Erstellungsdatum: 05.05.1994
 *
 *  2019-12-17 Sven Gothel: Holding transient string length, 
 *             removing file dependencies and some cleanup
 *
 *  $Log: strclass.cc $
 *  Revision 1.4  1995/04/07 16:54:57  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.3  1995/04/04 10:37:36  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.2  1994/12/15 15:28:28  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.1  1994/12/15 15:18:45  Sven_Gothel
 *  Initial revision
 *
 */
/*************************************************************************/

#include "strclass.h"
#include <climits>

std::ostream& operator<< (std::ostream& OS, StrClass& str)
{
    if(str.length()==0) {
        OS << "(nullptr)" ;
    } else {
        OS << str.StrPtr ;
    }
    return OS;
}

void StrClass::copy(const char *m)
{
    if(m!=nullptr)
    {
      // FIXME: consider using strnlen(..) using maxlen!
      {
          const size_t _length = strlen(m);
          if( _length > INT_MAX ) {
              INT_ERR(__LINE__);
          }
          m_length = (int) _length;
      }
      if ( ( StrPtr=new char[length+1] ) == nullptr )
      {
          fprintf(stderr,"Insufficient memory\n");
          INT_ERR(__LINE__);
      }
      strncpy(StrPtr, m, length+1);
      // StrPtr[length] = 0; // explicit EOS in case using strnlen(..) using maxlen
    } else {
      StrPtr=nullptr;
      m_length=0;
    }
}

void StrClass::copy(const StrClass &m)
{
    if( 0 < m.length() )
    {
      length = m.length();
      if ( ( StrPtr=new char[length+1] ) == nullptr )
      {
        fprintf(stderr,"Insufficient memory\n");
        INT_ERR(__LINE__);
      }
      strncpy(StrPtr, m, length+1);
    } else {
      StrPtr=nullptr;
      length=0;
    }
}


void StrClass::dest()
{
    if( nullptr != StrPtr ) {
      delete[] StrPtr;
      StrPtr=nullptr;
    }
    length = 0;
}

void StrClass::strcat(const StrClass &m)
{                  
     char *buffer;
     const int bufferLen = length() + m.length() + 1;
     if( 1 == bufferLen ) {
       return;
     }
     if ( ( buffer=new char[bufferLen] ) == nullptr ) {
       INT_ERR(__LINE__);
     }
     if( 0 < length() ) {
       strncpy(buffer, StrPtr, length()+1);
     } else {
       buffer[0]=0;  
     }

     if( 0 < m.length() ) {
         strncpy(buffer+length(), m.StrPtr, m.length()+1);
     }

     if( nullptr != StrPtr ) {
         delete[] StrPtr;
     }
     StrPtr=buffer;
     length=bufferLen;
}

StrClass& StrClass::operator=(const StrClass &m)
{
    if(this != &m)
    {
       if( nullptr != StrPtr ) {
         delete[] StrPtr;
       }
       copy(m);
    }
    return *this;
}

StrClass& StrClass::operator=(const char *StrPtr)
{
    if(this->StrPtr != StrPtr)
    {
      if( nullptr != StrPtr ) {
        delete[] StrPtr;
      }
      copy(StrPtr);
    }
    return *this;
}

int StrClass::operator==(const StrClass &m) const
{
    if(this == &m || this->StrPtr == m.StrPtr ) {
      return 1;
    }
    if ( 0 == length() )
    {
      return 0 == m.length();
    } 
    if ( 0 == m.length() ) {
      return 0;
    }
    if ( strcmp(StrPtr, m.StrPtr) == 0 ) {
      return 1;
    }
    return 0;
}

int StrClass::operator>=(const StrClass &m) const
{
    if ( 0 == length() )
    {
      return 0 == m.length();
    }

    if ( 0 == m.length() ) {
      return 1;
    }

    int b=strcmp(StrPtr, m.StrPtr);
    if ( b == 0 || b>0 ) {
      return 1;
    }
    return 0;
}

int StrClass::operator<=(const StrClass &m) const
{
    if ( 0 == length() ) return 1;

    if ( 0 == m.length() ) return 0;

    int b=strcmp(StrPtr, m.StrPtr);
    if ( b == 0 || b<0 ) {
      return 1;
    }
    return 0;
}

int StrClass::operator>(const StrClass &m) const
{
    if ( 0 == length() ) return 0;

    if ( 0 == m.length() ) return 1;

    if ( strcmp(StrPtr, m.StrPtr) > 0 ) {
      return 1;
    }
    return 0;
}

int StrClass::operator<(const StrClass &m) const
{
    if ( 0 == length() ) return 1;

    if ( 0 == m.length() ) return 0;

    if ( strcmp(StrPtr, m.StrPtr) < 0 ) {
      return 1;
    }
    return 0;
}
     
void StrClass::assignDouble(double m) 
{ 
      char buf[256]; 
      snprintf(buf, 256, "%.0lf", m); 
      if( nullptr != StrPtr ) {
        delete[] StrPtr;
      }
      copy(buf);
}

int StrClass::chk4SubStr(const StrClass &SubStr, int StartIndex)
{                                                              
    const int l=length(), l2=SubStr.length();
    int i;
    
    for (i=StartIndex; l-i>=l2; i++)
    {                                    
      if( strncmp( StrPtr+i, SubStr.StrPtr, l2 ) == 0 ) {
        return i;
      }
    }
    return -1;
}

int StrClass::delSubStr(const StrClass &SubStr, int StartIndex)
{
    const int i=chk4SubStr(SubStr, StartIndex);
    
    if( 0 <= i ) {
	const int l = length();
	const int l2 = SubStr.length();
      length=l-l2;
      char *newstr = new char[length+1];
      if( !newstr ) {
        INT_ERR(__LINE__);
      }
      if( 0 < i ) {
        strncpy(newstr, StrPtr, i); // w/o EOS
      }
      strncpy(newstr+i, StrPtr+i+l2, length+1-i);
      delete[] StrPtr;
      StrPtr=newstr;
    }
    return i;
}

bool StrClass::insertSubStr(const StrClass &SubStr, int Index)
{
	const int l = length();
	const int l2 = SubStr.length();
	
	if( l < Index ) return false;
      if( 0 == l2 ) return true;
	
      length = l+l2;
	char *newstr=new char[length+1];
	if(!newstr) {
        INT_ERR(__LINE__);
      }
	if( 0 < Index ) {
        strncpy(newstr, StrPtr, Index); // w/o EOS
      }
	
	newstr[Index]=0;
	strcat(newstr, SubStr.StrPtr);
	
	if(Index<l)	strcat(newstr, &(StrPtr[Index]));
	
	delete[] StrPtr;
	StrPtr=newstr;
	return true;
}

int StrClass::delAllSubStr(const StrClass &SubStr)
{
    int i=0;
    
    while(delSubStr(SubStr)>=0) i++;
    
    return i;
}             

int StrClass::substSubStr(const StrClass &SubStr, const StrClass &Substitute, int StartIndex)
{
    int i=StartIndex;
    
    if((i=chk4SubStr(SubStr,i))>=0)
    {
      if(delSubStr(SubStr, i)<0) INT_ERR(__LINE__);
	if(insertSubStr(Substitute, i)==false) INT_ERR(__LINE__);
    }
    
    return i;
}

int StrClass::substAllSubStr(const StrClass &SubStr, const StrClass &Substitute)
{
    int i=0;
    
    while(substSubStr(SubStr,Substitute)>=0) i++;
    
    return i;
}

void StrClass::delAllXtraWhiteSpaces()
{
	substAllSubStr("	"," "); // tab
	substAllSubStr("  "," "); // blank blank
	if(length()==1 && ( StrPtr[0]==' ' || StrPtr[0]=='	' ) ) {
        dest();
    }
}

/***************************************************************************/
/***************************************************************************/

StrClass operator+(const StrClass &m, const StrClass &n)
{
    StrClass sum=m;
    sum+=n;
    return sum;
}

/***************************************************************************/
/***************************************************************************/
