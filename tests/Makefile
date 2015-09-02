#########################################
#               Makefile                #
#   Autor:  Vojtech Mašek (xmasek15)    #
#########################################

CC=gcc

RM=rm -f
STD= c99
CFLAGS= -std=$(STD) -pedantic -Wall -Wextra $(FLAGS)

#name of compiled binary
BIN=test_ifj

#adds source .c files from our project dir that have to be compiled
ALL_SRCS=$(wildcard *.c ../*.c)
#this filters out main.c that is containing another main
SRCS=$(filter-out ../main.c,$(ALL_SRCS))

#replacement of .c suffix of source files to .o suffix used for object files
OBJS=$(subst .c,.o,$(SRCS))

all: $(BIN) dist-clean

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
