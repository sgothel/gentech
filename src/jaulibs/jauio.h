/**
 *  Datei           : jauio.h
 *  Inhalt          : eigene io funktionen
 *  Programmierer   : Sven Gothel
 *					  Stapenhorststr.35a
 *					  33615 Bielefeld
 *					  0521/139228
 *  Erstellungsdatum: 05.05.1994
 *  Žnderungsdatum  : 28.07.1994
 *
 *
 *  2019-12-17 Sven Gothel: Introduce reusable buffer passing etc
 *
 *  $Log: jauio.h $
 *  Revision 1.4  1995/04/07 16:54:50  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.3  1995/04/04 10:37:25  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.2  1994/12/15 15:28:07  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.1  1994/12/15 15:18:39  Sven_Gothel
 *  Initial revision
 */

#ifndef _JAUIO_H
  # define _JAUIO_H

  # include <stdlib.h>
  # include <stdio.h>
  # include "interror.h"
  # include "assert.h"

  char *GetFileInBuffer(const char *fName, const char *buffer, const size_t buffer_len, size_t *nbytes);

#endif

