// Error.h                    NOV 1993
// anonymos
/*
 *
 *  $Log: interror.h $
 *  Revision 1.4  1995/04/07 16:54:48  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.3  1995/04/04 10:37:24  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.2  1994/12/15 15:27:59  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.1  1994/12/15 15:18:37  Sven_Gothel
 *  Initial revision
 *
*/

# ifndef _ERROR_H
  # define _ERROR_H

  #include <cstdio>
/*  #include <iostream> */
  #include <cstdlib>


  # define INT_ERR(a)     int_err(a,__FILE__)
  # define INT_ERR_ZERO(a,b)    if((a)==0)int_err(b,__FILE__)
  # define INT_ERR_NOT_ZERO(a,b)  if((a)!=0)int_err(b,__FILE__)
  # define ERROR_OUT(a) ErrorOut((a))

  void int_err ( int line , const char * file ) ;
  void ErrorOut(char *fmt, ...);


# endif
