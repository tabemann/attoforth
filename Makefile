IDIR =src/include
CC=gcc
CFLAGS=-O2

ODIR=obj

LIBS=-lm -lpthread

_DEPS = af/common.h af/types.h af/inner.h af/prim.h af/io.h af/builtin.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o inner.o prim.o cond.o io.o builtin.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: src/runtime/%.c $(DEPS)
	mkdir -p obj
	$(CC) -c -o $@ $< -I$(IDIR) $(CFLAGS)

attoforth: $(OBJ)
	$(CC) -o $@ $^ -I$(IDIR) $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
