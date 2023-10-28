// ---------------------------------------------
// NAME: Kirk Thelen           User ID: klthelen
// DUE DATE: 11/28/2022
// PROJ3
// FILE NAME: MerkleGen.cpp
// PROGRAM PURPOSE:

// ---------------------------------------------

#include <iostream>  // std::cout
#include <stdlib.h>  // atoi
#include <string>    // std::string
#include <fstream>   // ofstream
#include "sha256.h"  // sha256
#include <math.h>    // pow

int main(int argc, char *argv[]) {
  // ********** PARAMETER CHECKING ********** //
  if (argc != 3) {
    std::cout << "usage: ./MerkleGen <n> <Txi>" << std::endl;
    std::cout.flush();
    exit(1);
  }
  int n = atoi(argv[1]);
  std::string txi = argv[2];
  int twoToTheN = pow(2, n);

  // ********** CHECK 1 <= n <= 12 ********** //
  if ((n < 1) || (n > 12)) {
    std::cout << "error: 1<=n<=12" << std::endl;
    std::cout.flush();
    exit(2);
  }
  // ********** CHECK VALID TXi ********** //
  std::string substr = txi.substr(2);
  long z = stol(substr); // z is the transaction that we will make the branch for
  if((z < 1) || (z > twoToTheN)) {
    std::cout << "error: Txi out of bounds" << std::endl;
    std::cout.flush();
    exit(3);
  }
  
  // ********** GENERATE LEAF HASHES ********** //
  std::string *hashArray = new std::string[twoToTheN];
  for(int i = 1; i <= twoToTheN; i++) {
    std::string iToString = std::to_string(i);
    std::string generatedTx = "Tx" + iToString;

    // std::cout << generatedTx << std::endl;
    std::string txiHash = sha256(generatedTx);
    hashArray[i-1] = txiHash;
  }

  // ********** GENERATE BRANCH  ********** // 
  std::string *branchArray = new std::string[n];

  // Note that the hashArray is 0-indexed, i.e.,
  // --> Tx1 is stored at hashArray[0]
  // --> Tx2 is stored at hashArray[1]
  // --> ...
  // --> Txi is stored at hashArray[i-1]

  // METHOD:
  // Figure out which half of the tree our Txi is on
  // --> We can simply take the closest Hash to root on the other side
  // --> Closest Hash to root on the opposite side is branchArray[n-1]
  // Our Txi is some number 1..2^n
  // If we divide its number in half, then it is in one of two halves:
  // --> 1..2^n-1 or 2^n-1...2^n
  // We already saved our transaction number in the variable z
  // --> We will use (z-1) because our array is 0-indexed, so z is stored at z-1
  // Recall that n must be at least 1 which means n-1 is at least 0 (non-negative)
  int currentN = n;
  int currentZ = z;
  bool lastWasLeft = false;
  while (currentN > 1) {
    int twoToTheCurrentN = pow(2, currentN);
    int twoToTheCurrentNMinus1 = pow(2, currentN-1);
    int l;
    int r;

    if (lastWasLeft) {
      // If we are refocusing on the right half now, we will renumber z as though
      // only the new subtree exists
      for(int i = 0; i < twoToTheCurrentN; i++) {
	hashArray[i] = hashArray[twoToTheCurrentN + i];
      }
      currentZ -= twoToTheCurrentN;
    }
    if ((currentZ-1) < twoToTheCurrentNMinus1) {
      // Txi is in the left half of the current subtree
      // --> branchArray[n-1] = Hash(2^n-1..2^n)
      // --> e.g., let Txi = Tx4 and n=4, then
      // ----> 4 < 2^(n-1) == 4 < 2^3 == 4 < 8 == Txi is in the left half
      // ----> branchArray[n-1] = Hash(89101112131415)
      l = twoToTheCurrentNMinus1;
      r = twoToTheCurrentN;
      lastWasLeft = false;
    }
    else {
      l = 0;
      r = twoToTheCurrentNMinus1;
      lastWasLeft = true;
    }
    int skip = 1;
    for(int i = 0; i < (currentN-1); i++) {
      // Calculate the Hash closest to root on one half of the tree
      for(int j = l; j < r; j = j + (2*skip)) {
	std::string txr1 = hashArray[j]; // Recall that hashArray[j] is actually Txj+1
	std::string txr2 = hashArray[j+skip];
	std::string combinedTxr = txr1 + txr2;
	std::string combinedTxrHash = sha256(combinedTxr);
	
	// We can save the combined hash back into hashArray using j's location
	hashArray[j] = combinedTxrHash;
      }
      // As we merge hashes, we will have to "skip" around the hashArray
      // We start by combining hashArray[j]   and hashArray[j+1]
      //                       hashArray[j+2] and hashArray[j+3]
      //                       etc.
      // Then we save the result back to hashArray[j], hashArray[j+2], etc.
      // and combine those     hashArray[j]   and hashArray[j+2]
      //                       etc.
      // Thus, we multiply skip by 2 each time we move up a row in the tree
      skip *= 2; 
    }
    branchArray[currentN-1] = hashArray[l];
    currentN--;
  }

  // When currentN == 1, we are on the leaf level
  // We just need whichever leaf is paired with our original z, and this will be stored
  // in branchArray[0]
  if ((z % 2) == 0) {
    // Even transaction, odd array index
    branchArray[0] = hashArray[z-2];
  }
  else {
    // Odd transaction, even array index
    branchArray[0] = hashArray[z];
  }

  // ********** CALCULATE WALKING PATH ********** //
  // We know some Txi, we need to trace up to the root
  // We will discover which branches we need to calculate the root
  // Luckily, we've saved all the hash values that we need in branchArray
  // Each level in branchArray is the next branch necessary to work up one level
  // This follows the same logic as calculating the branches
  currentN = n;
  currentZ = z;
  lastWasLeft = false;
  int *walkingPath = new int[n];
  
  while (currentN > 1) {
    int twoToTheCurrentN = pow(2, currentN);
    int twoToTheCurrentNMinus1 = pow(2, currentN-1);
  
    if (lastWasLeft) {
      currentZ -= twoToTheCurrentN;
    }
    // Let walkingPath[i] be 0 or 1, where 0 means we concat arrayBranch[i] to the right
    // of Txi and 1 means we concat it to the left
    
    if ((currentZ-1) < twoToTheCurrentNMinus1) {
      walkingPath[currentN-1] = 0;
      lastWasLeft = false;
    }
    else {
      walkingPath[currentN-1] = 1;
      lastWasLeft = true;
    }  
    currentN--;
  }

  // When currentN == 1, we are on the leaf level
  // We just need whichever leaf is paired with our original z, and this will be stored
  // in branchArray[0]
  if ((z % 2) == 0) {
    // Even transaction, odd array index
    walkingPath[0] = 1;
  }
  else {
    // Odd transaction, even array index
    walkingPath[0] = 0;
  }

  // ********** GENERATE ROOT USING WALKING PATH ********** //
   std::string root = sha256(txi);
  std::string concat;
  for (int i = 0; i < n; i++) {
     if (walkingPath[i] == 1) {
      concat = branchArray[i] + root;
     }
    else {
      concat = root + branchArray[i];
     }
    root = sha256(concat);
   }
  
   // ********** OUTPUT BRANCH AND ROOT FILES ********** //
  std::fstream branchFile;
  std::fstream rootFile;
  remove("branch.txt");
  remove("root.txt");
  branchFile.open("branch.txt", std::fstream::out);
  rootFile.open("root.txt", std::fstream::out);
  rootFile << root << std::endl;
  for(int i = 0; i < n; i++) {
    branchFile << branchArray[i] << std::endl;
  }

  rootFile.close();
  branchFile.close();
  delete [] hashArray;
  delete [] branchArray;
  delete [] walkingPath;
}
