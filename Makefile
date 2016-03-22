CC = g++
CFLAGS  = -std=c++11 -g -Wall

project2: project2.o 
	$(CC) $(CFLAGS) project2.o -o project2

project2.o: project2.cpp
	$(CC) $(CFLAGS) -c project2.cpp

clean:
	rm -f *.o
	rm -f project2
