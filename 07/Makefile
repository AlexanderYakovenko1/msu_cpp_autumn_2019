CC=g++

run: test
	./test

test: tst
	./test

tst: test.o 
	$(CC) -std=gnu++14 -o test test.o

test.o: test.cpp vector.h
	$(CC) -std=gnu++14 -c test.cpp

clean:
	rm -rf *.o test