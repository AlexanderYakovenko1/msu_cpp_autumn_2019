CC=g++

run: check
	./check 15 "3 5 8 r 14 2 r"

test: check
	./test.py ./check tests.txt

check: check.o 
	$(CC) -o check check.o

check.o: check.cpp
	$(CC) -c check.cpp

clean:
	rm -rf *.o check