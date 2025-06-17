CFLAGS = -Wall -Wextra -Wswitch-enum -Wmissing-prototypes -std=c11 -pedantic
LIBS = 
CC = gcc

EXAMPLES = $(patsubst ./examples/%.pasm,./examples/%.adm, $(wildcard ./examples/*.pasm))

.PHONY: all
all: pasm adme depasm
pasm: ./src/pasm.c ./src/adm.h
	$(CC) $(CFLAGS) -o pasm ./src/pasm.c $(LIBS)

adme: ./src/adme.c ./src/adm.h
	$(CC) $(CFLAGS) -o adme ./src/adme.c $(LIBS)
depasm: ./src/depasm.c ./src/adm.h
	$(CC) $(CFLAGS) -o depasm ./src/depasm.c $(LIBS)
.PHONY: examples
examples: $(EXAMPLES)


examples/%.adm: examples/%.pasm pasm
	./pasm $< $@