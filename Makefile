CFLAGS = -I. -Wall

pa1: main.o
		g++ $(CFLAGS) main.o -o pa1

main.o: main.cpp
		g++ $(CFLAGS) -c main.cpp

clean:
