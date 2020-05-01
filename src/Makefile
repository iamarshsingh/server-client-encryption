IDIR =../include
CC=g++
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = RSAClass.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = client.o server.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: $(OBJ)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

client: $(ODIR)/client.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

server: $(ODIR)/server.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
