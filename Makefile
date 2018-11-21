CC = g++
CFLAGS = -g -Wall

all: ashell


ashell: noncanmode.o
	$(CC) $(CFLAGS) -o ashell noncanmode.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c noncanmode.c

clean:
	rm -f ashell *.o
	echo Clean done
