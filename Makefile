CFLAGS = -Wall -Wextra -Wswitch-enum -std=c11 -pedantic
LIBS = 
.PHONY: all
all: pasm admi
pasm: ./src/pasm.c ./src/adm.c
	$(CC) $(CFLAGS) -o pasm ./src/pasm.c $(LIBS)

admi: ./src/admi.c ./src/adm.c
	$(CC) $(CFLAGS) -o admi ./src/admi.c $(LIBS)
.PHONY: examples
examples: ./examples/fib.adm ./examples/123.adm

./examples/fib.adm: pasm ./examples/fib.pasm
	./pasm ./examples/fib.pasm ./examples/fib.adm
./examples/123.adm: pasm ./examples/123.pasm
	./pasm ./examples/123.pasm ./examples/123.adm