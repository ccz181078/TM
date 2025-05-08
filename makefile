all: main.exe

main.exe: main.cpp
	g++ main.cpp -O3 -o main.exe -std=c++17

