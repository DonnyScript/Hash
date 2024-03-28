#ifndef HASH_H
#define HASH_H
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

struct hashNode {
	int keyValue = NULL;
	int keyCount = 0;
	int chainIndex = -1;
};

class hashTable {
protected:
    int uniqueValues = 0;
    int duplicateValues = 0;
    int collisions = 0;
    int directInserts = 0;
    int indirectInserts = 0;
    vector<int> bucketDistance;
    int totalComparisons = 0;
    int totalAccesses = 0;
    int largestComparisons = 0;
public:
    vector<int> readAndCheckInput(string filename);
    string testName;
    string fileName;
    void insertLinear(hashNode (&linearTableInsert)[100], vector<int> hashValues);
    void insertLinear(hashNode(&linearTableInsert)[100], int key);
    void insertOverload(hashNode(&chainedMain)[100], hashNode(&chainedOverflow)[100], vector<int> hashValues);
    void insertOverload(hashNode(&chainedMain)[100], hashNode(&chainedOverflow)[100], int key);

    hashTable() = default;
};

#endif