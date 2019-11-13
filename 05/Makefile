CC=g++

run: check
	./check

test: check
	./check

check: check.o 
	$(CC) -std=gnu++14 -o check check.o

check.o: check.cpp
	$(CC) -std=gnu++14 -c check.cpp

clean:
	rm -rf *.o check