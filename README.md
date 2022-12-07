# Gentech, A Genetic Algorithm Solver

[Original document location](https://jausoft.com/cgit/gentech.git/about/).

## Git Repository
This project's canonical repositories is hosted on [Gothel Software](https://jausoft.com/cgit/gentech.git/).

## Overview
This genetic algorithm follows the natural process of meiosis,
see our [meiosis compilation (pdf)](https://jausoft.com/cgit/gentech.git/plain/doc/meiosis.pdf).

Also available is the ancient [German documentation from 1994](https://jausoft.com/cgit/gentech.git/plain/doc/gentech-readme-de.txt).

This project is currently under review, see bugzilla Genetic Algorithm Solver:
- [Review & Enhance Genetic Algorithm Implementation](https://jausoft.com/bugzilla/show_bug.cgi?id=1)
- [All entries](https://jausoft.com/bugzilla/buglist.cgi?bug_status=__all__&list_id=6&order=Importance&product=GenAlgSolver&query_format=specific)

## Status
This document requires a rewrite due to poor English 
as well as a review based on our findings regarding [meiosis](https://jausoft.com/cgit/gentech.git/plain/doc/meiosis.pdf).

This version is working using C++17 on a GNU/Linux system.

## Supported Platforms
C++17 and better.

## Building Gentech

### Build Dependencies
- CMake 3.13+ but >= 3.18 is recommended
- gcc >= 8.3.0
  - or clang >= 10.0
- Optional
  - libunwind8 >= 1.2.1

Installing build dependencies on Debian (11 or better):
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
apt install git
apt install build-essential g++ gcc libc-dev libpthread-stubs0-dev 
apt install libunwind8 libunwind-dev
apt install cmake cmake-extras extra-cmake-modules pkg-config
apt install doxygen graphviz
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Build Procedure
The following is covered with [a convenient build script](https://jausoft.com/cgit/gentech.git/tree/scripts/build.sh).

For a generic build use:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
CPU_COUNT=`getconf _NPROCESSORS_ONLN`
git clone --recurse-submodule git://jausoft.com/srv/scm/gentech.git
cd gentech
mkdir build
cd build
cmake ..
make -j $CPU_COUNT install doc
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Our cmake configure has a number of options, *cmake-gui* or *ccmake* can show
you all the options. The interesting ones are detailed below:

Changing install path from /usr/local to /usr
~~~~~~~~~~~~~
-DCMAKE_INSTALL_PREFIX=/usr
~~~~~~~~~~~~~

Building debug build:
~~~~~~~~~~~~~
-DDEBUG=ON
~~~~~~~~~~~~~

To build documentation run: 
~~~~~~~~~~~~~
make doc
~~~~~~~~~~~~~

## The Genetic Algorithm
This genetic algorithm follows the natural process,
see our [compilation about meiosis (pdf)](https://jausoft.com/cgit/gentech.git/plain/doc/meiosis.pdf).

### The Basic Principle
Nature defines and constructs lifeforms with its genome.
The genome is coded by it's chromosomes.

A chromosome is a chain of nucleotides, the elements of the chromosomes.
In nature these nucleotides a represented with the following four bases:
Adenine (A), Guanine (G), Cytosine (C) and Thymine (T).
The different sequences of these nucleotides, bases, on the chromosome,
does code the genome[2].

The nature optimizes it's lifeforms with different methods.
One of these methods is the sexual reproduction, so called mating.
While mating, the genome of the mating partners are mixed,
this is called "crossing over" [1,2,3,6].
The result of the mix is the genome of the partners child(s).
E.g.:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      Father  : A G C T A G | A G G G T C | A C T A G
      Mother  : A A C T C G | A G C G T G | T C T A C
  ------------------------------------------------------
     1st Child: A G C T A G | A G C G T G | A C T A G
     2nd Child: A A C T C G | A G G G T C | T C T A C
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Besides the "normal" crossing over (homologous or symmetric crossing over[2]),
asymmetric crossing over exists[2].
Within the asymmetric crossing over, at least in the version 
of this genetic algorithm, one child contains the whole
genome of both parents.<br/>
So a growing (length) of the chromosomes is possible.

During the reproduction process so called mutations are possible.
This mutations modifies the sequence of chromosomes with the 
following possible ways:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Mutation      :  Nucleotides are changed randomized.
                 In the nature, e.g. by radioactive radiation[1,2,3,4,6].

Inversion     :  A segment of a chromosome is transferred in reversed 
                 order (last in - first out) [2,6].
                 E.g.:   A G T A | A G C A T | C A G T  ==>
                         A G T A | T A C G A | C A G T  .

Translocation :  A segment of a chromosome is transferred
                 to a different position to the destination (progeny). [6]
                 E.g.:   A G T A | A G C A T | C A G T  ==>
                         A G | A G C A T | T A C A G T  .
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

On chromosomes the genome is stored.
The logical whit (minimal part) of this genome is called gene.
The gene represents a segment of a chromosome, but the definition
of this noun is not that sharp since this documentation was written
in March 1994.
A chromosome, e.g. with a length of 10 000 bases, contains
not only usable (useful) information. 
So useless information is also within the chromosomes.
The chromosome's genes are often fractional.

The segments of genes/chromosomes with useful information
are called exons[2].
The segments of genes/chromosomes with useless/junk information
are called introns[2].

The fractional genes/chromosomes are spliced (splicing[2]), 
before they are transferred and used for the working process, 
e.g. the production of protein.
The results of splicing is the useful meaning information,
non fractional extrons.
So the working process receives just the 
(you can call this compressed) useful information.

Using this generation process, the nature is able to create
new life forces (creatures). 
With or without purpose, well, this is a philosophical or
theological matter.

The natural environment defines the viability of it's
creatures.

Darwin's thesis says, that only the best of a population
can prevail, so called "Survival of the Fittest".

This natural selection prefers consequently the best of a
generation. These "best" are able to reproduce itself
more often (than the "worse"), 
so they can take care of a better condition of it's progeny.
The next generation is thus more optimized as its predecessor
(may be). So it is possible, that a population creates it's
next optimized version.

This natural evolution principle, describes with 
crossing over(symmetrical and asymmetrical),
mutations, splicing and selection,
is the role model for this genetic algorithm.
Well, I think that I can think, so it may works ;-)

### Implemented Genetic Algorithm
Under the terms of the natural principle of evolution
a problem should be solved.
The following steps are necessary:

1. Creation of a random starting population

2. Setting the normal fitness [0..1]
  to the spliced genome using a given fitness function.
  This fitness function is problem-oriented,
  and must be define for each problem

3. Select the parents according the best fitness
  (Survival of the Fittest).

4. Reproducing using the selected parents with 
  crossing over(s).

5. Drop the not well conditioned members of the population,
  to make the population's size finite.

6. Mutate if necessary (statistically)

7. continue at point 2.

This look will be terminated, if the chosen target fitness is reached,
or is a chosen number of non optimized generations in a row.

The selection (2) of the parents occurs with the roulette system.
The better individuals receives higher probability as the worse
individuals of the generation.
The probability, that parents are composed outta individuals with a better
fitness is most probably.
But it is not impossible, that parents are composed outta 
individuals, which fitness may be worse at the current time.
Herewith it is avoided, that currently questionable worse solutions
are rejected.[1,3,4,6]

The decease within the population can be regulated in two ways.
One way is to create a complete new generation[4], which replaces
the parents generation.
in this case, the birth rate equals one.
The other way is to use a lower birth rate, e.g. 0.6.
In this case 60 percent new individuals are created,
whereby their parents must die.
Because bigamous relationships are allowed for both methods,
after the decease of the parents as much individuals with a worse
fitness must die, as the population's maximum is not exceeded.

### Implementation Details
This natural analogue principle is declared in the file
[gentech.h](include/gentech/gentech.h), 
and defined in the file [gentech.cpp](src/gentech/gentech.cpp).

The class 'Chromosome' contains the nucleotides of one chromosome,
the methods for splicing and mutations besides others.
For details, please have a look at the file [gentech.h](include/gentech/gentech.h).

The class 'Chromosomes' (many chromosomes -> population) 
contains the methods for selection, crossing over and the
whole evolution process.

The fitness method within the class 'Chromosomes'
is a virtual one.
The user, who want's to define the problem,
must define this one regarding his problem.
It must return a normal floating value within [0..1].

### The Encoding of the Problem
'How do I encode the problem within the chromosomes' ?
Is the basic question, and it's answer the problem's solution.

For example: In the river-problem (see 1.3.1 below), 
the combination of the boat crew must be encoded. 

The encoding is implemented to expose only valid crew combinations.

In this case, one nucleotide has the following range of values within [0..4]:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
0 : 1 Monk
1 : 2 Monks
2 : 1 Cannibal
3 : 2 Cannibals
4 : 1 Monk and 1 Cannibal
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

An encoded (solution) chromosome of this kind, supplies the alternating
boat crew from shore-a to shore b and vice versa.

The implementations of the game itself is contained within the files
'RIVER.[H|CPP]' using the class 'RiverGame'.

The chromosomes encoding of this game is contained within the files
`src/gen-river/river_gen.(h|cpp)`.
Here you find the class 'RiverProblem' as a derivation of
the class 'Chromosomes'.

### Example Applications

#### The River Problem
The river problem is that monks and cannibals must cross the river in
an way, so that the cannibal number is never greater as the monk number
on both riversides - except only cannibals exist on one riverside.
The boat is able to transport up to two persons, but one person as the minimum.

#### The Traveler-Salesman problem
The salesman-problem is that the salesman must travel to many towns in
an way, so that he takes the shortest way.
The difference to the river-problem (see river.gen) is that the
solution is not clear ... meaning that the shortest way is unknown.

## Conclusion
We wish you fun in solving problems following the methods of natural reproduction ;-)

## Historical Notes

This work was initially created 
by Sven Gothel and Christian Mueller in February 1994 
for a home assignment in `Object-Oriented Programming (OOP)` 
at the FH Bielefeld (University for Applied Science) 
under the supervision of Prof. Dr. Bunse.

## Changes

See [Changes](CHANGES.md).

### Bibliographie
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

