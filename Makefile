CC = clang
CFLAGS = -Wall -Wextra -std=c99 -O2

bptree.o: bptree.c

testbpt: testbpt.cc bptree.c
	g++ bptree.c testbpt.cc -g -o testbpt -fpermissive || echo "*** BUILD FAILURE ***"
