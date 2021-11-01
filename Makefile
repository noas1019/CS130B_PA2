# Makefile
prog2: prog2.cpp
	g++ -std=c++17 prog2.cpp -o prog2

clean:
	-rm prog2