
CC		= g++ -c
LN              = g++
GENTECHLIB	= ../gentech.lib
JAULIBS		= ../../jaulibs

CCOPTS		= -g -I. -I$(JAULIBS) -I$(GENTECHLIB) -D__LINUX__

.SUFFIXES	: .cc .o

#		*Implicit Rules*
.cc.o:
	$(CC) -o ${<:.cc=.o} $(CCOPTS) $<

#		*List Macros*


OBJS =  \
 main.o \
 genparin.o \
 sales.o \
 salesgen.o \
 $(GENTECHLIB)/gentech.o \
 $(JAULIBS)/maschine.o \
 $(JAULIBS)/interror.o

#		*Explicit Rules*
#		*Explicit Rules*
salesgen: $(OBJS)
	$(LN) -o $@ $(OBJS) -lm -lstdc++

clean:
	rm -f $(OBJS)
