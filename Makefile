CFLAGS = -Wall -Wextra -Wswitch-enum -std=c11 -pedantic
LIBS = 
.PHONY: all
all: pasm adme
pasm: ./src/pasm.c ./src/adm.h
	$(CC) $(CFLAGS) -o pasm ./src/pasm.c $(LIBS)

adme: ./src/adme.c ./src/adm.h
	$(CC) $(CFLAGS) -o adme ./src/adme.c $(LIBS)
.PHONY: examples
examples: ./examples/fib.adm ./examples/123.adm

./examples/fib.adm: pasm ./examples/fib.pasm
	./pasm ./examples/fib.pasm ./examples/fib.adm
./examples/123.adm: pasm ./examples/123.pasm
	./pasm ./examples/123.pasm ./examples/123.adm