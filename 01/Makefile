CC=g++

run: calc
	./calc "2+2"

test: calc
	./test.py ./calc tests.txt

calc: calc.o 
	$(CC) -o calc calc.o

calc.o: calc.cpp
	$(CC) -c calc.cpp

clean:
	rm -rf *.o calc