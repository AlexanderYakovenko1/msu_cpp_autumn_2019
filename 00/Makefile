CC=g++

run: sum
	./sum 0 1000

sum: main.o 
	$(CC) -o sum main.o

main.o: main.cpp timer.h
	$(CC) -c main.cpp

clean:
	rm -rf *.o my_prog