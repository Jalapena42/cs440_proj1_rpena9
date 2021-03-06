CFLAGS= -Wall -Werror -g -std=c++14

all: test

test.o: Deque.hpp test.cpp
	g++ -c ${CFLAGS} test.cpp

test: test.o
	g++ -Wl,--no-as-needed -ldl -o program1 test.o