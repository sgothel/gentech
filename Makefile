
CPP		:= g++ -x c++ -std=c++17 -c
LN	    := g++

GENTECHLIB	= src/gentech.lib
JAULIBS		= src/jaulibs

# set the compiler flags
#CPPFLAGS := -ggdb3 -O0 -Wall -Wextra -Werror -I. -I$(JAULIBS) -I$(GENTECHLIB)
#LNFLAGS := -ggdb3 -O0 -Wall -lm

CPPFLAGS := -O3 -DNDEBUG -Wall -Wextra -Werror -I. -I$(JAULIBS) -I$(GENTECHLIB)
LNFLAGS := -O3 -Wall -lm

TEMPDIRS = obj bin

.PHONY: all clean tempdirs obj

all: tempdirs bin/rivergen bin/salesman

obj/river_main.o: src/gen-river/river_main.cpp
	$(CPP) -o $@ $(CPPFLAGS) $<

obj/river_genparin.o: src/gen-river/river_genparin.cpp
	$(CPP) -o $@ $(CPPFLAGS) $<

obj/river.o: src/gen-river/river.cpp
	$(CPP) -o $@ $(CPPFLAGS) $<

obj/river_gen.o: src/gen-river/river_gen.cpp
	$(CPP) -o $@ $(CPPFLAGS) $<

obj/sales_main.o: src/gen-salesman/sales_main.cpp
	$(CPP) -o $@ $(CPPFLAGS) $<

obj/sales_genparin.o: src/gen-salesman/sales_genparin.cpp
	$(CPP) -o $@ $(CPPFLAGS) $<

obj/sales.o: src/gen-salesman/sales.cpp
	$(CPP) -o $@ $(CPPFLAGS) $<

obj/sales_gen.o: src/gen-salesman/sales_gen.cpp
	$(CPP) -o $@ $(CPPFLAGS) $<

obj/gentech.o: $(GENTECHLIB)/gentech.cpp
	$(CPP) -o $@ $(CPPFLAGS) $<

obj/maschine.o: $(JAULIBS)/maschine.cpp
	$(CPP) -o $@ $(CPPFLAGS) $<

obj/interror.o: $(JAULIBS)/interror.cpp
	$(CPP) -o $@ $(CPPFLAGS) $<

OBJS_RIVER =  \
 obj/river_genparin.o \
 obj/river_main.o \
 obj/river.o \
 obj/river_gen.o \
 obj/gentech.o \
 obj/maschine.o \
 obj/interror.o

OBJS_SMAN =  \
 obj/sales_genparin.o \
 obj/sales_main.o \
 obj/sales.o \
 obj/sales_gen.o \
 obj/gentech.o \
 obj/maschine.o \
 obj/interror.o

bin/rivergen: $(OBJS_RIVER)
	$(LN) -o $@ $^ $(LNFLAGS)

bin/salesman: $(OBJS_SMAN)
	$(LN) -o $@ $^ $(LNFLAGS)

tempdirs:
	for dir in $(TEMPDIRS); do \
	    mkdir -p $$dir; \
	done

clean:
	rm -rf obj bin Debug build

