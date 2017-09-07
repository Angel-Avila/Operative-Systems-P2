
all: init getty sh

INIT: init.o getty.o sh.o
			gcc -o init init.o
			gcc -o getty getty.o
			gcc -o sh sh.o

init.o: init.c getty.c sh.c
				gcc -c init.c
				gcc -c getty.c
				gcc -c sh.c

clean:
			rm init *.o
			rm getty *.o
			rm sh *.o
