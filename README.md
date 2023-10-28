# merkle-tree

An implementation of the functionalities of a Merkle tree (used in blockchain computations) for simple payment verification.

This program has two components: `MerkleGen` and `VerifyTx`. <br/>

Given a number of transactions (2^n, where 1<= n <= 12) and a specific transaction (tx <= 2^n), `MerkleGen` outputs (1) the Merkle root of the Merkle tree constructed with these transactions and (2) the corresponding Merkle branch used to prove that the specific transaction is included in the Merkle tree (i.e., included in the the blockchain block header that contains the Merkle root).

Given a Merkle root, Merkle branch, and a specific transaction, `VerifyTx` evaluates whether or not the transaction exists in the Merkle tree using the Merkle root and branch.

From the /source folder, the two executables MerkleGen and VerifyTx can be generated with:<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`make all`<br/>

They can be removed with:<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`make clean`<br/>


# Testing
You can test the program as follows:
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`./MerkleGen <n> <Txi>` <br/>

Where n is a number 1 <= n <= 12 and Txi is some number Tx1 <= Txi <= Tx2^n<br/>
(That is, Txi is numbered from Tx1, Tx2, ..., Tx2^n)<br/>

An example input is:<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`./MerkleGen 12 Tx3021`</br>

The program will output two files: `root.txt` and `branch.txt`<br/>

### VerifyTx
You can then run:<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`./VerifyTx <rootfile> <branchfile> <Txi>`<br/>
To verify whether the provided Tx can be proven to be included in the Merkle tree.<br/>

An example input is:<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`./VerifyTx root.txt branch.txt Tx3021`<br/>

You can technically use any file name for root.txt/branch.txt, but please keep them in the correct order (root first, then branch).<br/>

MerkleGen will output the files as root.txt and branch.txt exactly.<br/>

VerifyTx assumes that the root and branch files exist in the source directory (that is, MerkleGen should be run first or some root/branch files should be placed in the source directory before VerifyTx is run). <br/>


# Full Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`$ make all`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`$ ./MerkleGen 12 Tx3021`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`$ ./VerifyTx root.txt branch.txt Tx3021`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`$ make clean`<br/>
