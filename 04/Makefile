CC=g++

run: check
	./check 1

test: check
	./test.py ./check tests.txt

check: check.o 
	$(CC) -o check check.o

check.o: check.cpp
	$(CC) -c check.cpp

clean:
	rm -rf *.o check