// author : Sven Gothel - Stapenhorststrasse 35a - 33615 Bielefeld
//
/*
 *
 *  $Log: dblmenge.cc $
 *  Revision 1.4  1995/04/07 16:54:45  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.3  1995/04/04 10:37:20  Sven_Gothel
 *  *** empty log message ***
 *  Revision 1.2  1994/12/15 15:27:46  Sven_Gothel
 *  the very first check in !!!
 *  all files contains the log message now !!!
 *  Sven Gothel, Bielefeld 14.12.1994 !!!
 *  
 *  Revision 1.1  1994/12/15 15:18:34  Sven_Gothel
 *  Initial revision
 *
*/

#include "menge.h"

#include <cmath>
#include <limits>

template<> Menge<double>::size_type Menge<double>::istElement(const double& x)
{
    for (size_type i = 0; i < card(); i++) {
        if( ::fabs( (*this)[i] - x ) < std::numeric_limits<double>::epsilon() * ::fabs(x) ) {
            return i;
        }
    }
    return npos;
}


