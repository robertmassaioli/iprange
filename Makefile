CC=c++
CF=-Wall -Werror -g -O3
LD=
name=iprange.bin

all: main.o
	$(CC) $(CF) $(LD) -o $(name) main.o

main.o: main.cpp iprange.h iprange.cpp
	$(CC) -c $(CF) main.cpp

clean: tidy
	rm -f $(name)

tidy:
	rm -f *~ *.o
