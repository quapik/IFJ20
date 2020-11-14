/*
# Implementace překladače imperativního jazyka IFJ20
# Tým číslo 041, varianta I
# Autoři projektu:
#	Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
#	Fabián Michal   xfabia13@stud.fit.vutbr.cz
#	Čábela Radek    xcabel04@stud.fit.vutbr.cz
#	Poposki Vasil   xpopos00@stud.fit.vutbr.cz
#	Prosinec 2020, Fakulta informačních technologií VUT v Brně
# Makefile
*/

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
