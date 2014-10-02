CC=gcc

RM=rm -f
CFLAGS= -std=gnu99 -pedantic -Wall

#name of compiled binary
BIN=projekt_ifj

#adds source .c files from our project dir that have to be compiled
SRCS=$(wildcard *.c)
#replacement of .c suffix of source files to .o suffix used for object files
OBJS=$(subst .c,.o,$(SRCS))

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS)

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .dependtool

include .depend