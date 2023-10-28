// ---------------------------------------------
// NAME: Kirk Thelen           User ID: klthelen
// DUE DATE: 11/28/2022
// PROJ3
// FILE NAME: VerifyTx.cpp
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
  if (argc != 4) {
    std::cout << "usage: ./VerifyTx <root.txt> <branch.txt> <Txi>" << std::endl;
    std::cout.flush();
    exit(1);
  }
  std::string rootFileName = argv[1];
  std::string branchFileName = argv[2];
  std::string txi = argv[3];
  std::string substr = txi.substr(2);
  long z = stol(substr); // z is the transaction number

  std::ifstream rootFile;
  std::ifstream branchFile;
  rootFile.open(rootFileName);
  branchFile.open(branchFileName);

  std::string root;
  std::getline(rootFile, root);

  std::string *branchArray = new std::string[12]; // 1<=n<=12
  std::string branch;
  int n = 0;
  while (std::getline(branchFile, branch)) {
    branchArray[n] = branch;
    n++;
  }

  // ********** CALCULATE WALKING PATH ********** //
  // We know some Txi, we need to trace up to the root
  // We will discover which branches we need to calculate the root
  // Luckily, we've saved all the hash values that we need in branchArray
  // Each level in branchArray is the next branch necessary to work up one level
  // This follows the same logic as calculating the branches

  int currentN = n;
  int currentZ = z;
  bool lastWasLeft = false;
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
  std::string calculatedRoot = sha256(txi);
  std::string concat;
  for (int i = 0; i < n; i++) {
    if (walkingPath[i] == 1) {
      concat = branchArray[i] + calculatedRoot;
    }
    else {
      concat = calculatedRoot + branchArray[i];
    }
    calculatedRoot = sha256(concat);
  }
  
  // ********** COMPARE CALCULATED ROOT TO PROVIDED ROOT ********** //
  if (root == calculatedRoot) {
    std::cout << "yes" << std::endl;
  }
  else {
    std::cout << "no" << std::endl;
  }
    
  rootFile.close();
  branchFile.close();
  delete [] branchArray;
  delete [] walkingPath;
}
