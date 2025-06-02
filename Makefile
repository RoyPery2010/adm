CFLAGS = -Wall -Wextra -Wswitch-enum -Wmissing-prototypes -std=c11 -pedantic
LIBS = 
CC = gcc

.PHONY: all
all: pasm adme depasm nan
pasm: ./src/pasm.c ./src/adm.h
	$(CC) $(CFLAGS) -o pasm ./src/pasm.c $(LIBS)

adme: ./src/adme.c ./src/adm.h
	$(CC) $(CFLAGS) -o adme ./src/adme.c $(LIBS)
depasm: ./src/depasm.c ./src/adm.h
	$(CC) $(CFLAGS) -o depasm ./src/depasm.c $(LIBS)
nan: ./src/nan.c
	$(CC) $(CFLAGS) -o nan ./src/nan.c $(LIBS)
.PHONY: examples
examples: ./examples/fib.adm ./examples/123.adm

./examples/fib.adm: pasm ./examples/fib.pasm
	./pasm ./examples/fib.pasm ./examples/fib.adm
./examples/123.adm: pasm ./examples/123.pasm
	./pasm ./examples/123.pasm ./examples/123.adm