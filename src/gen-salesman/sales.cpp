// sales.cpp                                                     6.3.1994
//
// Diese Modul wurde fuer eine Hausarbeit im Fach
// Objektorietierter Programierung (OOP) an der FH Bielefeld
// unter der Betreuung von Prof. Dr. Bunse erstellt.
//
// Autoren der Hausarbeit : Sven Goethel und Christian Mueller
//
//
// Jegliches Copyright aller Dateien ist im Besitz der Autoren.
// Sven Goethel * Stapenhorststr.35a * 33615 Bielefeld * 0521/139228
// Bielefeld, den 11.3.1994.

# include <math.h>
# include "sales.hpp"

/* Es folgen die Zeilen der Abstandstabelle.             */
/* Wegen der Symmetrie genuegt die obere Dreiecksmatrix. */

int line00[] = { 633, 257, 412, 378, 150, 659,  80, 134, 259, 505,
		 488, 353, 372, 330, 581, 70, 585, 496, 648, 475,
		 246, 446, 235, 121 };

int line01[] = { 390, 227, 397, 488, 205, 572, 530, 555, 819, 289,
		 282, 686, 378, 370, 569, 179, 147, 584, 437, 745,
		 624, 581, 518 };

int line02[] = { 169, 175, 112, 511, 196, 154, 272, 618, 262, 110,
		 485, 119, 355, 191, 437, 253, 598, 428, 472, 559,
		 348, 142 };

int line03[] = { 167, 267, 304, 351, 309, 338, 602, 196, 61, 469,
		 150, 289, 346, 196,  85, 625, 452, 528, 543, 364,
		 297 };

int line04[] = { 233, 470, 317, 275, 466, 730, 119, 125, 597, 278,
		 278, 312, 362, 251, 753, 580, 656, 671, 469, 263 };

int line05[] = { 607, 63, 34, 264, 509, 343, 208, 397, 174, 436,
		  83, 532, 351, 653, 480, 364, 451, 240,  35 };

int line06[] = { 629, 638, 469, 728, 502, 364, 578, 389, 571, 589,
		 108, 236, 484, 337, 659, 524, 495, 636 };

int line07[] = {  29, 232, 478, 427, 292, 345, 276, 520,  47, 558,
		 435, 621, 448, 332, 419, 208, 29 };

int line08[] = { 249, 494, 385, 250, 361, 207, 478,  68, 567, 393,
		 638, 465, 349, 436, 225, 36 };

int line09[] = { 276, 495, 352, 143, 193, 588, 189, 395, 427, 395,
		 222, 202, 217,  32, 236 };

int line10[] = { 759, 616, 135, 457, 852, 435, 659, 691, 412, 391,
		 279, 207, 283, 482 };

int line11[] = { 154, 626, 307,  93, 422, 391, 280, 782, 609, 685,
		 700, 521, 373 };

int line12[] = { 483, 164, 247, 287, 256, 145, 639, 466, 542, 557,
		 378, 238 };

int line13[] = { 324, 719, 302, 526, 558, 287, 241, 144,  82, 150,
		 349 };

int line14[] = { 400, 249, 318, 239, 482, 309, 383, 398, 219, 222 };

int line15[] = { 515, 484, 373, 875, 702, 778, 793, 614, 466 };

int line16[] = { 515, 430, 578, 405, 289, 376, 165,  55 };

int line17[] = { 128, 438, 291, 591, 478, 427, 568 };

int line18[] = { 523, 376, 618, 563, 454, 381 };

int line19[] = { 167, 431, 220, 429, 625 };

int line20[] = { 395, 187, 254, 452 };

int line21[] = { 226, 165, 336 };

int line22[] = { 224, 423 };

int line23[] = { 212 };


/* Die Namen der 25 Staedte */

static const char *names[] = {
  "Aachen         ", "Berlin         ", "Bielefeld      ", "Braunschweig   ",
  "Bremen         ", "Dortmund       ", "Dresden        ", "Duesseldorf     ",
  "Essen          ", "Frankfurt/Main ", "Freiburg i. Br.", "Hamburg        ",
  "Hannover       ", "Karlsruhe      ", "Kassel         ", "Kiel           ",
  "Koeln           ", "Leipzig        ", "Magdeburg      ", "Muenchen        ",
  "Nuernberg       ", "Saarbruecken    ", "Stuttgart      ", "Wiesbaden      ",
  "Wuppertal      "
};

static const int *lineno[] = { line00, line01, line02, line03, line04,
		                       line05, line06, line07, line08, line09,
		                       line10, line11, line12, line13, line14,
		                       line15, line16, line17, line18, line19,
		                       line20, line21, line22, line23
		                     };


double SalesGame::Play(const Chromosom& Lsg, bool Gfx) const
{
    if (Gfx == false)	{
        // Berechnung ohne Ausgabe
        Chromosom::size_type ChromPos  = 1;
        double result = 0;

        for ( ;ChromPos < Lsg.size(); ChromPos++)	{
            if( Lsg[ChromPos] > Lsg[ChromPos-1] ) {
                result += lineno[ Lsg[ChromPos-1] ][ Lsg[ChromPos] - Lsg[ChromPos-1] - 1 ];
            } else {
                result += lineno[ Lsg[ChromPos] ][ Lsg[ChromPos-1] - Lsg[ChromPos] - 1 ];
            }
        }
        if ( Lsg[ChromPos - 1] > Lsg[0] ) {
            result += lineno[ Lsg[0] ][ Lsg[ChromPos-1] - Lsg[0] - 1 ];
        } else {
            result += lineno[ Lsg[ChromPos-1] ][ Lsg[0] - Lsg[ChromPos-1] - 1 ];
        }
        return result;
    } else {
        // Protokolliert den zurueckgelegten Weg
        std::cout << std::endl << std::endl << "Die Loesung: " << std::endl;
        for (Chromosom::size_type i = 1, ChromPos = 0; ChromPos < Lsg.size(); ChromPos++, i++)
        {
            std::cout << names[Lsg[ChromPos]] << " -> ";
            if (! (i % 4)) std::cout << std::endl;
        }
        std::cout << std::endl ;
        return -1;
    }
}

