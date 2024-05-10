all: execute

execute: 
	g++ -std=c++17 generate.cc -lsparsebase -fopenmp -lgomp -std=c++17 -o execute

clean:
	rm -f execute
