testbpt: testbpt.cc bptree.c
	g++ bptree.c testbpt.cc -g -o testbpt -fpermissive || echo "*** BUILD FAILURE ***"
