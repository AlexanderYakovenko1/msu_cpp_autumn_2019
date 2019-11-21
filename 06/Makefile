CC=g++

run: check
	./check 2

test: check
	./test.py ./check tests.txt

check: check.o 
	$(CC) -std=gnu++14 -o check check.o

check.o: check.cpp
	$(CC) -std=gnu++14 -c check.cpp

clean:
	rm -rf *.o check