// FULL EXAMPLE
   $ make all
   $ ./MerkleGen 12 Tx3021
   $ ./VerifyTx root.txt branch.txt Tx3021
   $ make clean

// COMPILE
You can compile the program from /source by typing:
    make all

You can remove all created files (binaries, .o, root.txt, and branch.txt) with
    make clean


// TEST MERKLEGEN
You can test the program as according to the specification as follows:
    ./MerkleGen n Txi

Where n is a number 1 <= n <= 12
and Txi is some number Tx1 <= Txi <= Tx2^n
That is, Txi is numbered from Tx1, Tx2, ..., Tx2^n

     An example input is:
     	./MerkleGen 12 Tx3021

The program will output two files, root.txt and branch.txt

// TEST VERIFYTX
You can then run:
    ./VerifyTx rootfile branchfile Txi
To verify whether the provided Tx can be proven to be included in the Merkle tree. 

   An example input is:
        ./VerifyTx root.txt branch.txt Tx3021

You can technically use any file name for root.txt/branch.txt, but please keep them in the correct order (root first, then branch).
MerkleGen will output the files as root.txt and branch.txt exactly.
VerifyTx assumes that the root and branch files exist in the source directory
(that is, MerkleGen should be run first or some root/branch files should be placed in the source directory before VerifyTx is run). 
