CC=c++
CF=-Wall -Werror
LD=
name=iprange

all: main.o iprange.o
	$(CC) $(CF) $(LD) -o $(name) main.o iprange.o

main.o: main.cpp iprange.h
	$(CC) -c $(CF) main.cpp

iprange.o: iprange.h iprange.cpp
	$(CC) -c $(CF) iprange.cpp

clean: tidy
	rm -f $(name)

tidy:
	rm -f *~ *.o
