CC	= c++
FLAGS 	= 
CFLAGS	= -g -O2

OBJ_FILE = sha256.o

all: ${OBJ_FILE} MerkleGen VerifyTx

MerkleGen: MerkleGen.o
	${CC} ${FLAGS} ${CFLAGS} -w -o MerkleGen MerkleGen.o ${OBJ_FILE}

MerkleGen.o: MerkleGen.cpp
	${CC} ${FLAGS} ${CFLAGS} -w -c MerkleGen.cpp

VerifyTx: VerifyTx.o
	${CC} ${FLAGS} ${CFLAGS} -w -o VerifyTx VerifyTx.o ${OBJ_FILE}

VerifyTx.o: VerifyTx.cpp
	${CC} ${FLAGS} ${CFLAGS} -w -c VerifyTx.cpp

sha256.o: sha256.cpp
	${CC} ${FLAGS} ${CFLAGS} -c sha256.cpp

clean:
	rm -f MerkleGen MerkleGen.o VerifyTx VerifyTx.o sha256.o root.txt branch.txt
