# lxs
PREF=/usr/local/bin
VERSION=0.3.2
OPTS=-Wall -pedantic -O3 -I./include -DVERSION=\"${VERSION}\"
PROG=lxs
SRCDIR=src

all:
	g++ $(OPTS) -o $(PROG) $(SRCDIR)/main.cpp $(SRCDIR)/SymClass.cpp
clean:
	rm $(PROG)
install:
	cp $(PROG) $(PREF)
