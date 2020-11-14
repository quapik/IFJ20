# Implementace překladače imperativního jazyka IFJ20
# Tým 041, varianta I
# Autor: xfabia13, Michal Fabián

FILE=projIFJ20
CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -pedantic
SOURCE=main.c scanner.c symtable.c
ZIPNAME=xsimav01

all: $(FILE)

$(FILE): $(SOURCE)
	$(CC) $(CFLAGS) -o $(FILE) $(SOURCE)

zip:
	zip $(ZIPNAME).zip *.c *.h Makefile rozdeleni dokumentace.pdf

clean:
	rm -f *.o $(FILE) $(ZIPNAME).zip
