CC = clang
CFLAGS = -Wall -Wextra -std=c99 -O2

bptree.o: bptree.c

testbpt: testbpt.cc bptree.c
	g++ -std=c++11 bptree.c testbpt.cc -g -o testbpt -fpermissive || echo "*** BUILD FAILURE ***"
