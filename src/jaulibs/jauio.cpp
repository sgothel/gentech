// author : Sven Gothel - Stapenhorststrasse 35a - 33615 Bielefeld
//
/**
 *  Datei           : jauio.cc
 *  Inhalt          : eigene io funktionen
 *  Programmierer   : Sven Gothel
 *					  Stapenhorststr.35a
 *					  33615 Bielefeld
 *					  0521/139228
 *  Erstellungsdatum: 05.05.1994
 *  nderungsdatum  : 28.07.1994
 *
 *
 *  $Log: jauio.cc $
 *  Revision 1.4  1995/04/07 16:54:49  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.3  1995/04/04 10:37:25  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.2  1994/12/15 15:28:05  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.1  1994/12/15 15:18:39  Sven_Gothel
 *  Initial revision
 *
 */

# include "jauio.h"

/*DOCST------------------------------------------------------------------------------

	Eigen IO Funktion
	----------------------------------------
	char *GetFileInBuffer(char *fName)

	- Beschreibung
	--------------
	Es wird der Versuch unternommen eine Text-Datei komplett in den Speicher
	einzuladen.

	- Parameter
	-----------
	E:Dateiname

	- allgemeine Ein- und Ausgabeeinheiten
	--------------------------------------
	keine

	- Returncode
	------------
	OK : SpeicherAdresse.
		 Der Speicher muss sp„ter mittels free wieder freigegeben werden.

	FAlSE : NULL

	- Historie
	----------
	28.7.1994 Sven Gothel
    2019-12-17 Sven Gothel: Introduce reusable buffer passing etc

------------------------------------------------------------------------------DOCEND*/
char *GetFileInBuffer(const char *fName, const char *buffer, const size_t buffer_len, size_t *nbytes)
{
  FILE *fTmp=NULL;
  size_t i, ReadBytes;
  char *res;

  if(buffer_len==0 || NULL == buffer) {
	fprintf(stderr, "Illegal Arguments\n");
	INT_ERR(__LINE__);
  }

  if( (fTmp=fopen(fName,"rt")) == NULL )
  {
	INT_ERR(__LINE__);
  }

  // kopieren !!!!
  i=0;
  while ( ( ReadBytes=fread(buffer+i, 1, buffer_len-i, fTmp) ) > 0 )
  {
	i+=ReadBytes;
	assert(i<=buffer_len);
  }

  if(feof(fTmp)==0)
  {
    *nbytes=0;
	res=NULL;
  } else {
    *nbytes=i+1;
	Buffer[i]=0;
    res=malloc(i+1);
    memcpy(res, buffer, i+1);
  }


  fclose(fTmp);

  return res;
}

