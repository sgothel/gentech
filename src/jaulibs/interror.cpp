// Error.h                    NOV 1993
// anonymos
/*
 *
 *  $Log: interror.cc $
 *  Revision 1.7  1995/05/08 03:05:59  sg
 *  #elif define __ACAD_APPLICATION__ !!!
 *  Revision 1.6  1995/04/07 16:54:48  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.5  1995/04/04 11:22:10  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.4  1995/04/04 10:39:19  Sven_Gothel
 *  ads_alert fuer __ACAD_APPLICATION__
 *  Revision 1.3  1995/04/04 10:37:23  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.2  1994/12/15 15:27:57  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.1  1994/12/15 15:18:37  Sven_Gothel
 *  Initial revision
 *
*/

#include <cstdarg>     // uses va_list

#ifdef __ACAD_APPLICATION__ 
  #include "adslib.h"
  //void ads_abort(const char *);
#endif
#ifdef _Windows
  #include "windows.h"
#endif
# include "interror.h"


void int_err ( int line , const char * file ) {

  char text [ 256 ] ;

  snprintf ( text , 256, "Intern Error in %s, line %d.\nPlease contact the software-developer !\n" , file , line ) ;
  # ifdef _Windows
    FatalAppExit ( 0 , text ) ;
  # elif defined __ACAD_APPLICATION__ 
    ads_alert ("Intern Error !!!");
    ads_printf(text);
    ads_abort ("\nACAD-ADS applikation aborted, fatal error!\n");
  # else
    fprintf(stderr, "%s\n", text);
    abort();
  # endif
}

void ErrorOut(char *fmt, ...)
{
  va_list va;
  char Buffer[500];

  va_start(va, fmt);
  (void) vsnprintf(Buffer, 500, fmt, va);
  #ifdef _Windows
    HWND hwnd=GetActiveWindow();
    (void )MessageBox(hwnd,Buffer,"ERROR",
                      MB_APPLMODAL|MB_OK|MB_ICONHAND);
  # elif defined __ACAD_APPLICATION__
    ads_alert ("Error !!!");
    ads_printf(Buffer);
  #else
    fprintf(stderr, "%s", Buffer);
  #endif
  va_end(va);
}


