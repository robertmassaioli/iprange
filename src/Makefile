CC=g++
CF=-Wall -Werror -g -O0 -fmudflap 
LDFLAGS=-lmudflap
name=iprange.bin

all: main.o
	$(CC) $(LDFLAGS) -o $(name) main.o

main.o: main.cpp iprange.h iprange.tcc
	$(CC) -c $(CF) main.cpp

clean: tidy
	rm -f $(name)

tidy:
	rm -f *~ *.o
