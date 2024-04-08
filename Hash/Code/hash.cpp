#include "hash.h"


vector<int> hashTable::readAndCheckInput(string filename) {
    ofstream outputFile(hashTable::testName, ios::app);
    cout << fileName << endl;
    vector<int> values;
    int num;
    string tempData;

    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cout << "Unable to open file: " + filename << endl;
        outputFile << "Unable to open file: " + filename << endl;
        return values;
    }

    if (inputFile.peek() == ifstream::traits_type::eof()) {
        cout << "Input file is empty" << endl;
        outputFile << "Input file is empty" << endl;
        return values;
    }

    while (inputFile >> tempData) {
        try {
            num = stoi(tempData);
        }
        catch (const invalid_argument& ia) {
            cout << tempData << " - is not a valid input " << endl;
            outputFile << tempData << " - is not a valid input " << endl;
            continue;
        }

        if (num < 0) {
            continue;
        }

        values.push_back(num);
    }

    outputFile.close();
    inputFile.close();
    return values;
}


void hashTable::insertLinear(hashNode(&linearTableInsert)[100], vector<int> hashValues) {
    int bucketIndex = 0;
    for (int key : hashValues) {
        int hashIndex = key % 100;
        int lastChainIndex = key % 100;
        int chainOrigin = key % 100;
        bucketDistance.push_back(0);

        if (linearTableInsert[hashIndex].keyValue == NULL) {
            linearTableInsert[hashIndex].keyValue = key;
            directInserts++;
            uniqueValues++;
        }
        else if (linearTableInsert[hashIndex].keyValue == key){
            linearTableInsert[hashIndex].keyCount++;
            directInserts++;
            duplicateValues++;
        }
        else {

            while (linearTableInsert[hashIndex].keyValue != NULL && linearTableInsert[hashIndex].keyValue != key) {
                if (linearTableInsert[hashIndex].keyValue % 100 == key % 100) {
                    lastChainIndex = hashIndex;
                }
                bucketDistance[bucketIndex]++; 

                hashIndex = (hashIndex + 1) % 100;
                collisions++;
            }


            if (linearTableInsert[lastChainIndex].keyValue % 100 == key % 100) {
                linearTableInsert[lastChainIndex].chainIndex = hashIndex;
            }

            
            linearTableInsert[hashIndex].keyValue = key;
            indirectInserts++;  
            bucketIndex++;
        }
    }
}

void hashTable::insertLinear(hashNode(&linearTableInsert)[100],int key) {
        int bucketIndex = 0;
        int hashIndex = key % 100;
        if (linearTableInsert[hashIndex].keyValue == NULL) {
            linearTableInsert[hashIndex].keyValue = key;
            directInserts++;
            uniqueValues++;
        }
        else if (linearTableInsert[hashIndex].keyValue == key) {
            linearTableInsert[hashIndex].keyCount++;
            directInserts++;
            duplicateValues++;
        }
        else {
            while (linearTableInsert[hashIndex].keyValue != NULL && linearTableInsert[hashIndex].keyValue != key) {
                hashIndex = (hashIndex + 1) % 100;
                bucketDistance[bucketIndex]++; 
                collisions++;
            }
            linearTableInsert[hashIndex].keyValue = key;
            indirectInserts++;
        }
        bucketDistance.push_back(0);
        bucketIndex++;
    }

void hashTable::insertOverload(hashNode(&chainedMain)[100], hashNode(&chainedOverflow)[100], int key) {
        int bucketIndex = 0;
        int hashIndex = key % 100;
        if (chainedMain[hashIndex].keyValue == NULL) {
            chainedMain[hashIndex].keyValue = key;
            directInserts++;
            uniqueValues++;
        }
        else if (chainedMain[hashIndex].keyValue == key) {
            chainedMain[hashIndex].keyCount++;
            directInserts++;
            duplicateValues++;
        }
        else {
            if (chainedOverflow[hashIndex].keyValue == NULL) {
                indirectInserts++;
                chainedOverflow[hashIndex].keyValue = key;
                chainedMain[hashIndex].chainIndex = hashIndex;
            }
            else if (chainedOverflow[hashIndex].keyValue == key) {
                indirectInserts++;
                chainedOverflow[hashIndex].keyCount++;
                chainedMain[hashIndex].chainIndex = hashIndex;
            }
            else {
                while (chainedOverflow[hashIndex].keyValue != NULL && chainedOverflow[hashIndex].keyValue != key) {
                    bucketDistance[bucketIndex]++;
                    collisions++;
                    hashIndex = (hashIndex + 1) % 100;
                }
                indirectInserts++;
                chainedOverflow[hashIndex].keyValue = key;
                chainedOverflow[hashIndex].chainIndex = (hashIndex - 1) % 100;
            }
            bucketDistance.push_back(0);
            bucketIndex++;
        }
    }

void hashTable::searchLinear(hashNode(&linearTableInsert)[100], hashNode(&searchQueue)[100]){
    searches++;
    int comparisonIndex = 0;

    for (int i = 0; i <= 99; i++) {
        int hashIndex = searchQueue[i].keyValue % 100;
        int key = searchQueue[i].keyValue;
        comparisons.push_back(0);

        if (linearTableInsert[hashIndex].keyValue == key) {
            comparisons[comparisonIndex]++;
            directAccesss++;
        }
        else if (linearTableInsert[hashIndex].chainIndex != -1) {

            while (linearTableInsert[hashIndex].keyValue != key) { 
                comparisons[comparisonIndex]++;
                if (linearTableInsert[hashIndex].chainIndex == -1) {
                    break;
                }
                hashIndex = linearTableInsert[hashIndex].chainIndex;
            }
            
        }
        indirectAccess++;
        comparisonIndex++;
    }
}

void hashTable::printStats(hashNode(&linearTableInsert)[100], hashNode(&chainedMain)[100], hashNode(&chainedOverflow)[100], hashTable linearTable, hashTable chainedTable) {
    ofstream outputFile(testName, ios::app);
    int totalLinearComparisons = 0, totalChainedComparisons = 0, averageLinearComparison = 0, averageChainedComparisons = 0, largestLinearComparison = 0, largestChainedComparison = 0;
    int averageLinearDistance = 0, averageChainedDistance = 0, largestLinearDistance = 0, largestChainedDistance = 0, totalLinearDistance = 0, totalChainedDistance = 0;
    int avgLinearDirDistance = 0, avgChainedDirDistance = 0, avgLinearNonDirDistance = 0, avgChainedNonDirDistance = 0;
    for (int num : linearTable.bucketDistance) {
        if (num > largestLinearDistance) {
            largestLinearDistance = num;
        }
        totalLinearDistance += num;
    }
    for (int num : chainedTable.bucketDistance) {
        if (num > largestChainedDistance) {
            largestChainedDistance = num;
        }
        totalChainedDistance += num;
    }
    for (int num : linearTable.comparisons) {
        if (num > largestLinearComparison) {
            largestLinearComparison = num;
        }
        totalLinearComparisons += num;
    }
    for (int num : chainedTable.comparisons) {
        if (num > largestChainedComparison) {
            largestChainedComparison = num;
        }
        totalChainedComparisons += num;
    }
    avgLinearDirDistance = (totalLinearDistance + linearTable.directInserts) / linearTable.bucketDistance.size();
    avgChainedDirDistance = (totalChainedDistance + chainedTable.directInserts) / chainedTable.bucketDistance.size();
    avgLinearNonDirDistance = (totalLinearDistance) / linearTable.bucketDistance.size();
    avgChainedNonDirDistance = (totalChainedDistance ) / chainedTable.bucketDistance.size();
    averageLinearDistance = totalLinearDistance / linearTable.bucketDistance.size();
    averageChainedDistance = totalChainedDistance / chainedTable.bucketDistance.size();
    averageLinearComparison = totalLinearComparisons / linearTable.comparisons.size();
    averageChainedComparisons = totalChainedComparisons / chainedTable.comparisons.size();
    cout << "                   "<< hashTable::testName << endl;
    cout << "	Linear Open Addressing Hash Table" << endl;
    cout << "Index              Key             count" << endl;
    for (int i = 0; i <= 99; i++) {
        cout << i << "              " << linearTableInsert[i].keyValue << "             " << linearTableInsert[i].keyCount << endl;
    }

    cout << "                  Operation Counts" << endl;
    cout << "                                      Linear            Chained" << endl;
    cout << "Number of key values inserted:                " << linearTable.uniqueValues + linearTable.duplicateValues << "           " << chainedTable.uniqueValues + chainedTable.duplicateValues << endl;
    cout << "       Unique values:                         " << linearTable.uniqueValues << "           " << chainedTable.uniqueValues << endl;
    cout << "       Duplicate Values:                      " << linearTable.duplicateValues << "             " << chainedTable.duplicateValues << endl;
    
    cout << "Collisions" << endl;
    cout << "        Number of collisions:                 " << linearTable.collisions << "           " << chainedTable.collisions << endl;
    cout << "        Distance from home bucket             " << endl;
    cout << "               Number of direct inserts:      " << linearTable.directInserts << "           " << chainedTable.directInserts << endl;
    cout << "        Number of non-direct inserts:         " << linearTable.directInserts << "            " << chainedTable.indirectInserts << endl;
    cout << "        Average distance from home:           " << averageLinearDistance << "            " << averageChainedDistance << endl;
    cout << "               Including direct inserts       " << avgLinearDirDistance << "            " << avgChainedDirDistance << endl;
    cout << "                non-including direct inserts: " << avgLinearNonDirDistance << "            " << avgChainedNonDirDistance << endl;
    cout << "                Largest distance:             " << largestLinearDistance << "            " << largestChainedDistance << endl;
    cout << "Searches" << endl;
    cout << "   Number of searches:                         " << linearTable.searches << "            " << chainedTable.searches << endl;
    cout << "   Number of comparisons                       " << totalLinearComparisons << "           " << totalChainedComparisons << endl;
    cout << "       Total number of comparisons" << endl;
    cout << "          Number of direct accesses            " << linearTable.directAccesss << "           " << chainedTable.directAccesss << endl;
    cout << "           number of indirect accesses         " << linearTable.indirectAccess << "            " << chainedTable.indirectAccess << endl;
    cout << "           Total number of accesses            " << linearTable.directAccesss + linearTable.indirectAccess << "           " << chainedTable.directAccesss + chainedTable.indirectAccess << endl;
    cout << "           Average number of comparisons       " << averageLinearComparison << "            " << averageChainedComparisons << endl;
    cout << "           Largest number of comparisons        " << largestLinearComparison << "            " << largestChainedComparison << endl;

    outputFile << "                   " << hashTable::testName << endl;
    outputFile << "	Linear Open Addressing Hash Table" << endl;
    outputFile << "Index              Key             count" << endl;
    for (int i = 0; i <= 99; i++) {
        outputFile << i << "                 " << linearTableInsert[i].keyValue << "               " << linearTableInsert[i].keyCount << endl;
    }
  outputFile << "                  Operation Counts" << endl;
  outputFile << "                                      Linear            Chained" << endl;
  outputFile << "Number of key values inserted:                " << linearTable.uniqueValues + linearTable.duplicateValues << "           " << chainedTable.uniqueValues + chainedTable.duplicateValues << endl;
  outputFile << "       Unique values:                         " << linearTable.uniqueValues << "           " << chainedTable.uniqueValues << endl;
  outputFile << "       Duplicate Values:                      " << linearTable.duplicateValues << "             " << chainedTable.duplicateValues << endl;
  outputFile << "Collisions" << endl;
  outputFile << "        Number of collisions:                 " << linearTable.collisions << "           " << chainedTable.collisions << endl;
  outputFile << "        Distance from home bucket             " << endl;
  outputFile << "               Number of direct inserts:      " << linearTable.directInserts << "           " << chainedTable.directInserts << endl;
  outputFile << "        Number of non-direct inserts:         " << linearTable.directInserts << "            " << chainedTable.indirectInserts << endl;
  outputFile << "        Average distance from home:           " << averageLinearDistance << "            " << averageChainedDistance << endl;
  outputFile << "               Including direct inserts       " << avgLinearDirDistance << "            " << avgChainedDirDistance << endl;
  outputFile << "                non-including direct inserts: " << avgLinearNonDirDistance << "            " << avgChainedNonDirDistance << endl;
  outputFile << "                Largest distance:             " << largestLinearDistance << "            " << largestChainedDistance << endl;
  outputFile << "Searches" << endl;
  outputFile << "   Number of searches:                         " << linearTable.searches << "            " << chainedTable.searches << endl;
  outputFile << "   Number of comparisons                       " << totalLinearComparisons << "           " << totalChainedComparisons << endl;
  outputFile << "       Total number of comparisons" << endl;
  outputFile << "          Number of direct accesses            " << linearTable.directAccesss << "           " << chainedTable.directAccesss << endl;
  outputFile << "           number of indirect accesses         " << linearTable.indirectAccess << "            " << chainedTable.indirectAccess << endl;
  outputFile << "           Total number of accesses            " << linearTable.directAccesss + linearTable.indirectAccess << "           " << chainedTable.directAccesss + chainedTable.indirectAccess << endl;
  outputFile << "           Average number of comparisons       " << averageLinearComparison << "            " << averageChainedComparisons << endl;
  outputFile << "           Largest number of comparisons        " << largestLinearComparison << "            " << largestChainedComparison << endl;
}


void hashTable::searchChained(hashNode(&chainedMain)[100], hashNode(&chainedOverflow)[100], hashNode(&searchQueue)[100]) {
    int comparisonIndex = 0;
    searches++;
    for (int i = 0; i <= 99; i++) {
        int hashIndex = searchQueue[i].keyValue % 100;
        int key = searchQueue[i].keyValue;
        int foundValue;
        comparisons.push_back(0);

        if (chainedMain[hashIndex].keyValue == key) {
            comparisons[comparisonIndex]++;
            directAccesss++;
        }
        else if (chainedMain[hashIndex].chainIndex != -1) {
            while (chainedOverflow[hashIndex].keyValue != key) {
                comparisons[comparisonIndex]++;
                hashIndex = (hashIndex + 1) % 100;
            }
            indirectAccess++;
        }
        comparisonIndex++;
    }
}


void hashTable::insertOverload(hashNode(&chainedMain)[100], hashNode(&chainedOverflow)[100], vector<int> hashValues) {
    int bucketIndex = 0;
    for (int key : hashValues) {
        int hashIndex = key % 100;
        int lastChainIndex = key % 100;
        int chainOrigin = key % 100;
        bucketDistance.push_back(0);

        if (chainedMain[hashIndex].keyValue == NULL){
            chainedMain[hashIndex].keyValue = key;
            directInserts++;
            uniqueValues++;
        }
        else if (chainedMain[hashIndex].keyValue == key) {
            chainedMain[hashIndex].keyCount++;
            duplicateValues++;
            directInserts++;
        }
        else {
            if (chainedOverflow[hashIndex].keyValue == NULL) {
                indirectInserts++;
                chainedOverflow[hashIndex].keyValue = key;
                chainedMain[hashIndex].chainIndex = hashIndex;
            }
            else if (chainedOverflow[hashIndex].keyValue == key) { 
                indirectInserts++;
                chainedOverflow[hashIndex].keyCount++;
                chainedMain[hashIndex].chainIndex = hashIndex;
            }
            else {
                while (chainedOverflow[hashIndex].keyValue != NULL && chainedOverflow[hashIndex].keyValue != key) {
                    if (chainedOverflow[hashIndex].keyValue % 100 == key % 100) {
                        lastChainIndex = hashIndex;
                    }
                    bucketDistance[bucketIndex]++;
                    collisions++;
                    hashIndex = (hashIndex + 1) % 100; 
                }
                if (chainedOverflow[lastChainIndex].keyValue % 100 == key % 100) {
                    chainedOverflow[lastChainIndex].chainIndex = hashIndex;
                }
                else if (chainedOverflow[chainOrigin].chainIndex == -1) {
                    chainedMain[chainOrigin].chainIndex = hashIndex;
                }
                chainedOverflow[hashIndex].keyValue = key;
                indirectInserts++;
            }
        }
        bucketIndex++;
    }
}

void hashTable::clearVector() {
    bucketDistance.clear();
}