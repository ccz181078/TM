all: main.exe

main.exe: main.cpp
	g++ main.cpp -O2 -g3 -o main.exe -fopenmp -std=c++17

