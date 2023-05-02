OBJ = util.o basics.o tuple.o binary.o
PROG = test-util test-basics test-tuple test-binary
BIBLIO = -lm -lgmp
ARG = -Wall -Wextra -g

all: $(PROG)

test-binary : test-binary.o $(OBJ)
	gcc test-binary.o $(OBJ) -o test_bin $(BIBLIO) 

test-tuple : test-tuple.o $(OBJ)
	gcc test-tuple.o $(OBJ) -o test_t $(BIBLIO) 

test-basics : test-basics.o $(OBJ)
	gcc test-basics.o $(OBJ) -o test_b $(BIBLIO) 

test-util : test-util.o $(OBJ)
	gcc test-util.o $(OBJ) -o test_u $(BIBLIO) 

test-binary.o : test-binary.c 
	gcc -c test-binary.c $(ARG)

test-tuple.o : test-tuple.c 
	gcc -c test-tuple.c $(ARG)

test-basics.o : test-basics.c 
	gcc -c test-basics.c $(ARG)

test-util.o : test-util.c 
	gcc -c test-util.c	$(ARG)

binary.o : ../util/binary.c ../util/subsum.h
	gcc -c ../util/binary.c $(ARG)

tuple.o : ../util/tuple.c ../util/subsum.h
	gcc -c ../util/tuple.c $(ARG)

basics.o : ../util/basics.c ../util/subsum.h
	gcc -c ../util/basics.c $(ARG)
	
util.o : ../util/util.c ../util/subsum.h
	gcc -c ../util/util.c $(ARG)

clean :
	rm -f *.o
