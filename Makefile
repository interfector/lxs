# lxs
PREF=/usr/local/bin
OPTS=
VER=0.0.1
PROG=lxs
SRCDIR=src
INCDIR=include

all:
	g++ $(OPTS) -I$(INCDIR) -o $(PROG) $(SRCDIR)/main.cpp $(SRCDIR)/SymClass.cpp
clean:
	rm $(PROG)
install:
	cp $(PROG) $(PREF)
