gentech.doc                                                    11.3.1994

First of all, gentech.lib is used by gen-river AND gen-salesman !
If you want to download the whole stuff, download:

	jaulibs.zip
	gen-river.zip
	gen-salesman.zip
	gentech.zip

The following information appears on german. 

Diese Documentation wurde fuer eine Hausarbeit im Fach
Objektorietierter Programierung (OOP) an der FH Bielefeld
unter der Betreuung von Prof. Dr. Bunse erstellt.

Autoren der Hausarbeit : Sven Goethel und Christian Mueller

Jegliches Copyright aller Dateien ist im Besitz der Autoren.
Sven Goethel * Stapenhorststr.35a * 33615 Bielefeld * 0521/139228
Bielefeld, den 11.3.1994.


Inhaltsverzeichniss
===================

1. Das Flussproblem
2. Die Analyse
3. Die Black-Box ( Loesungs-Such-Prozess )
4. Natuerliche Optimierung
5. Genetische Algorithmen
6. Die Kodierung des Problems
7. Die Ausfuehrbaren Dateien / Die Compilation


1. Das Flussproblem
===================

  Es sollte ein Suchprozess programmiert werden, der die Loesung
  fuer das Missionare und Kannibalen Spiel findet.
  Programmiert sollte in C++, einer Objektorientierten Programmiersprache (OOP).
  Siehe river.gen !!


2. Die Analyse
==============

  Die Autoren der Loesung haben eine Black-Box Programmiert,
  in der man das Problem einwirft, und spaeter eine Loesung herausbekommt.
  Diese Black-Box ist derart abstrakt, dass fast ein jeglich erdenkliches
  Problem mit ihr geloest werden sollte.
  In diesem Beispiel ist sie in der Lage das Flussproblem zu loesen.


3. Die Black-Box ( Loesungs-Such-Prozess )
==========================================

  Die Black-Box sollte auf natuerlicher Weise eine Loesung finden.
  Eine Loesung ist eine vollendete Optimierung nichtloesender Verfahren.


4. Natuerliche Optimierung
==========================

  Die Natur definiert und konstruiert die Lebewesen mittels ihres Erbgutes.
  Das Erbgut ist in sogenannten Chromosome kodiert.
  Ein Chromosom ist eine Kette von Nukleotiden, den Elementen der
  Chromosomen.
  In der Natur werden diese Nukleotide mit den folgenden vier Basen dargestellt:
  Adenin (A), Guanin (G), Cytosin (C) und Thymin (T).
  Die unterschiedliche Folge dieser Nukleotide, Basen, auf dem Chromosom
  kodiert die Erbinformation[2].

  Die Natur optimiert ihre Lebewesen mit verschiedenen Methoden.
  Eine dieser Methoden ist die sexuelle Fortpflanzung, auch Paarung genannt.
  Hierbei vermischt (Crossing Over [1,2,3,6]) sich waehrend der Paarung
  das Erbgut beider Elternteile zu einem neuen Lebewesen, dem Kind.
  Z.B.    Vater  : A G C T A G | A G G G T C | A C T A G
          Mutter : A A C T C G | A G C G T G | T C T A C
      ------------------------------------------------------
         1.Kind  : A G C T A G | A G C G T G | A C T A G
         2.Kind  : A A C T C G | A G G G T C | T C T A C

  Neben dem normalen Crossing Over, homologenes Crossing Over [2],
  existiert auch das Asymmetrische Crossing Over [2].
  Hierbei erhaelt, zumindest in der Version des genetischen Algorithmus
  der Autoren, ein Kind die gesamte Erbinformation beider Eltern.
  Eine Verlaengerung der Chromosomen steht somit nichts im Weg.

  Waehrend einer solchen Fortpflanzung sind auch sogenannte Mutationen
  moeglich. Diese Mutationen veraendern die Nukleotidenfolge der Chromosomen
  in folgender Weise.

    Mutation      :  Es werden zufaellig Nukleotide auf dem Chromosom veraendert.
                     I.d. Natur z.B. durch radioaktive Strahlung [1,2,3,4,6].

    Inversion     :  Ein Chromosomenabschnitt wird in umgekehrter Reihenfolge
                     in dem Nachkommen uebertragen [2,6].
                     Z.B.    A G T A | A G C A T | C A G T  ==>
                             A G T A | T A C G A | C A G T  .

    Translokation :  Ein Chromosomenabschnitt wird in dem Nachkommen
                     an eine andere Position uebertragen [6].
                     Z.B.    A G T A | A G C A T | C A G T  ==>
                             A G | A G C A T | T A C A G T  .

  Auf den Chromosomen ist, s.o., die Erbinformation gespeichert.
  Ein kleinster logischer Teil dieser Erbinformation nennt man Gen.
  Das Gen stellt allgemein einen Chromosomenabschnitt dar.
  Dazu muss gesagt werden, dass die Definition des Gen's 
  heute noch sehr unscharf ist.
  Ein Chromosom mit der Laenge von 10000 Basen enthaelt aber nicht nur
  nuetzliche Information. Die auf dem Chromosomen enthaltenen Gene
  liegen oft in gestueckelter Form dar. Die Abschnitte des Gens mit
  der nuetzlichen Information nennt man Exons[2].
  Die Abschnitte mit der redundanten Information nennt man Introns[2].
  Diese gestueckelten Chromosomen werden vor Uebergabe an den
  'Arbeitprozess' ( Z.B. Proteinerzeugung) gespleisst (Splicing[2]).
  Hierbei wird die redundante Information herausgefiltert
  und dem Arbeitsprozess nur die relevante Information zur Verfuegung
  gestellt.

  Mittels dieser Generations-Verfahren ist die Natur in der Lage
  neue Lebewesen zu erzeugen.
  Ob diese Lebewesen lebensfaehig sind, bestimmt ihre Umwelt.
  Darwins Theorie besagt, dass sich nur die Besten einer Population
  durchsetzen koennen. Die sogenannte Methode des 'Survival of the Fittest'.
  Diese natuerliche Auslese (Selektion) bevorzugt somit die besten
  einer Generation. Diese 'Besten' koennen sich haeufiger fortpflanzen
  und somit fuer eine besser konditionierten Nachwelt sorgen.
  Die 2.Generation ist somit optimierter als die 1.Generation.

  Diese natuerlichen Evolutionsverfahren, beschrieben durch
  das Crossing Over (homologenes und asymmetrisches),
  der Mutationen, des Splicing und der Selektion,
  sind Vorbild fuer den genetischen Algorithmus, den die Autoren
  implementiert haben.


5. Genetische Algorithmen
=========================

  " Computerprogramme, die aus einem der natuerlichen Selektion
    nachempfundenen Evolutionsprozess hervorgehen, koennen komplexere
    Probleme loesen, die selbst die Autoren der Programme nicht
    vollstaendig verstehen. " ( John H. Holland [1] )

  Gemaess des natuerlichen Evolutionsverfahren soll ein Problem
  geloest werden.
  Folgende schritte sind dafuer notwendig:

    1.) Erzeugung einer zufaelligen Startpopulation
    2.) Vergabe einer normierten Fitness ( [0..1] )
        an der gespleissten Erbinformation
        mittels eine uebergebenden Fitness-Funktion.
    3.) Selektieren der Eltern nach der besten Fitness
        (Survival of the Fittest)
    4.) Fortpflanzen der selektierten Eltern mittels Crossing Overs.
    5.) Sterben nicht so gut konditionierten Lebewesen,
        um die Populationsgroesse nicht zu sprengen.
    6.) Evt. Mutieren
    7.) Bei Punkt 2.) Fortfahren.

  Abgebrochen wird dieser Block, wenn die Zielfitness erreicht ist,
  oder es seit einer waehlbaren Anzahl von Generationen keine
  durchschnittliche Verbesserung erzielt worden.

  Die Selektion ( Punkt 3 )  der Eltern geschieht mittels Roulette-System.
  D.h. die besseren Individuen erhalten proportional mehr Zahlen der
  Roulette-Drehscheibe, als die schlechteren ihrer Generation.
  Die Wahrscheinlichkeit, dass die Eltern sich aus Individuen guter
  Fitness zusammensetzen ist somit recht hoch.
  Das ein Elternteil aber auch ein Individuum mit schlechter Fitness sein
  kann ist nicht ausgeschlossen. Hiermit wird vermieden, dass die
  zur Zeit zweifelhaften Loesungen schlechter Individuen vollstaendig
  verworfen werden[1,3,4,6].

  Das Sterben in der Population kann zweierlei geregelt werden.
  Zum einem ist es moeglich eine vollstaendig neue Generation
  zu erschaffen [4], welche die Elterngeneration ersetzt.
  Die Geburtenrate ist in diesem Fall gleich 1.
  Zum anderen kann mann eine niedrigere Geburtenrate z.B. 0.6 angeben.
  Hier werden 60 Prozent neue Individuen erzeugt.
  Die Eltern muessen dann sterben.
  Da bei beiden Methoden bigamistische Beziehungen erlaubt sind,
  muessen nach dem Tod der Eltern noch soviele Individuen mit der
  schlechtesten Fitness sterben, damit die Populationsgroesse die Maximalen   
  Populationsgroesse nicht uebersteigt.

  Dieses natuerliche Verfahren ist in der Datei 'GENTECH.H' deklariert
  und in der Datei 'GENTECH.CPP' definiert.
  Die dort enthaltene Klasse 'Chromosom' enthaelt die Nukleotide eines
  Chromosomes und u.a. die Methoden des Spleissen und des Mutierens.
  Naeheres zur Implementation siehe in der Datei 'GENTECH.H'.
  Die ebenfalls in 'GENTECH.H' enthaltene Klasse 'Chromosomen'
  enthaelt u.a. die Methoden der Selektion, des CrossingOvers und
  der gesammten Evolution.
  Die Klasse 'Chromosomen' ist eine doppelt verkettete Liste der Klasse
  'Chromosom' vererbt worden. Zudem ist sie eine Abstrakte-Klasse.
  D.h. sie enthaelt eine ganz virtuale Funktion.
  Diese virtuale Funktion ist die Fitness-Funktion.
  Diese muss der Benutzer der Klassen auf sein Problem spezifizieren.
  Sie muss eine Fitness von 0 bis 1, inklusive ([0..1]), zurueckgeben.
  Naehere Erlaeuterungen zu den Details der Implementation entnehmen Sie bitte
  dem gut dokumentierten File 'GENTECH.H'.


6. Die Kodierung des Problems
=============================

  'Wie sage ich es den Chromosomen ?' Lautet die Frage, wenn das Problem
  oder besser die Loesung des selbigen auf einem Chromosom kodiert werden soll.
  In dem Flussproblem muss die Zusammensetzung der Bootsbesatzung kodiert
  werden. Christian Mueller implementierte eine Kodierung, so dass
  es nur regelgerechte Bootsbesatzungen gibt.
  Ein Nukleotid erhaelt dabei folgenden Benutzer-Wertebereich [0..4]:
    0 : 1 Moench
    1 : 2 Moenche
    2 : 1 Kannibale
    3 : 2 Kannibalen
    4 : 1 Moench und 1 Kannibale

  Ein derart kodiertes Loesungschromosom liefert dann alternierend die
  Bootsbestzung von Ufer A nach Ufer B, und umgekehrt.
  Die Implementierung des reinen Spieles ist in der Datei 'RIVER.H[CPP]'
  mittels der Klasse 'RiverGame' deklariert [definiert].
  Die Deklaration [Definition] des Flussproblems steht in der Datei
  'RIVERGEN.H[CPP]'. Dort ist die von der Klasse 'Chromosomen' abgeleitete
  Klasse 'RiverProblem' fuer die Gesamtloesung verantwortlich.
  Die Klasse 'RiverProblem' definiert mithilfe der beinhaltenden Klasse  
  'RiverGame' ( eine Has-A-Beziehung ) die ganz virtuale Fitnessfunktion der 
  abgeleiteten Klasse 'Chromosomen' ( Is-A-Beziehung ).

7. Die Ausfuehrbaren Dateien / Die Compilation
==============================================

  Folgende ausfuehrbare Dateien sollten vorhanden sein :

  RIVER.EXE	      : Die Version ohne jegliche DEBUG Informationen
		        ( schnellere Ausfuehrung als RIVERDBG.EXE )
  RIVERDBG.EXE     : Die Version mit DEBUG Informationen.
		        Diese Version enthaelt Zahlreiche
		        'assert' Sicherheitsabfragen.
		        Selbige sollten in der Testphase die Fehler aufspueren !!!
  RIVER		:      wie RIVER.EXE auf UNIX !

  Compilieren, falls Source Code vorhanden, koennen Sie einfach mit 'make' !!!
  Unter Linux und Win32+Cygnus-Gnu-win32 sollte es keine Probleme geben ...

  Sven Goethel, Bielefeld den 11.3.1994

  Die Autoren wuenschen bei der natuerlichen Fortpflanzung der
  Loesungsansaetze viel Vergnuegen !!!!!!!!!!!!!!!!!!!!!!!!!!!


  Literaturverzeichniss :

  [1] Holland, John H. : Genetische Algorithmen,
      Spektrum der Wissenschaft, September 1992, S.44-51
  [2] Chambon, Pierre  : Gestueckelte Gene - ein Informations-Mosaik,
      Spektrum der Wissenschaft, Juli 1981, S.104-117
  [3] Dewdney, A.K.    : Genetische Algorithmen,
      Computer-Kurzweil, Spektrum-der-Wissenschaft-Verlagsgesellschaft
      1988, ISBN 3-922508-50-2, S.68-72
  [4] Weckwerth, Guido : Zeugende Zahlen,
      MC, Oktober 1993, S.54-58, Januar 1994, S.72-75
  [5] Schader, Kuhlins : Programmieren in C++,
      Springer-Verlag Heidelberg 1993, ISBN 3-540-56524-8
  [6] Schoeneburg, Eberhard : Genetische Algorithmen und Evolutionsstrategien
      Addison-Wesley 1994, ISBN 3-89319-495-2

