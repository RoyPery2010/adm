CFLAGS = -Wall -Wextra -Wswitch-enum -std=c11 -pedantic
LIBS = 
.PHONY: all
all: pasm admi
pasm: pasm.c adm.c
	$(CC) $(CFLAGS) -o pasm pasm.c $(LIBS)

admi: admi.c adm.c
	$(CC) $(CFLAGS) -o admi admi.c $(LIBS)
.PHONY: examples
examples: ./examples/fib.adm ./examples/123.adm

./examples/fib.adm: ./examples/fib.pasm
	./pasm ./examples/fib.pasm ./examples/fib.adm
./examples/123.adm: ./examples/123.pasm
	./pasm ./examples/123.pasm ./examples/123.adm