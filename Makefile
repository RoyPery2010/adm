CFLAGS = -Wall -Wextra -Wswitch-enum -Wmissing-prototypes -std=c11 -pedantic
LIBS = 
CC = gcc

.PHONY: all
all: pasm adme depasm
pasm: ./src/pasm.c ./src/adm.h
	$(CC) $(CFLAGS) -o pasm ./src/pasm.c $(LIBS)

adme: ./src/adme.c ./src/adm.h
	$(CC) $(CFLAGS) -o adme ./src/adme.c $(LIBS)
depasm: ./src/depasm.c ./src/adm.h
	$(CC) $(CFLAGS) -o depasm ./src/depasm.c $(LIBS)
.PHONY: examples
examples: ./examples/fib.adm ./examples/123i.adm ./examples/123f.adm ./examples/e.adm ./examples/pi.adm

./examples/fib.adm: pasm ./examples/fib.pasm
	./pasm ./examples/fib.pasm ./examples/fib.adm
./examples/123i.adm: pasm ./examples/123i.pasm
	./pasm ./examples/123i.pasm ./examples/123i.adm
./examples/123f.adm: pasm ./examples/123f.pasm
	./pasm ./examples/123f.pasm ./examples/123f.adm
./examples/e.adm: pasm ./examples/e.pasm
	./pasm ./examples/e.pasm ./examples/e.adm
./examples/pi.adm: pasm ./examples/pi.pasm
	./pasm ./examples/pi.pasm ./examples/pi.adm