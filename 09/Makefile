CC=g++

run: sort
	python gen_file.py 3000003 -p input
	./sort input output
	python check_sort.py output

test: sort
	python gen_file.py 3000003 -p input
	./sort input output
	python check_sort.py output

sort: sort.o 
	$(CC) -std=gnu++14 -o sort sort.o

sort.o: sort.cpp
	$(CC) -std=gnu++14 -c sort.cpp

clean:
	rm -rf *.o sort input output