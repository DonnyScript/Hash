#include "hash.h"


vector<int> hashTable::readAndCheckInput(string filename){
    ofstream outputFile(hashTable::testName, ios::app);
    cout << fileName << endl;
    vector<int> values;
    int num;
    string tempData;

    ifstream inputFile(filename);
    

    if (!inputFile.is_open()) {
        cout << "Unable to open file: " + filename <<  endl;
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
            bucketDistance.push_back(0);
            while (linearTableInsert[hashIndex].keyValue != NULL && linearTableInsert[hashIndex].keyValue != key) {
                hashIndex = (hashIndex + 1) % 100;
                hashTable::bucketDistance[bucketIndex]++;
                collisions++;
            }
            linearTableInsert[hashIndex].keyValue = key;
            indirectInserts++;
        }
        bucketIndex++;
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
            bucketDistance.push_back(0);
            while (linearTableInsert[hashIndex].keyValue != NULL && linearTableInsert[hashIndex].keyValue != key) {
                hashIndex = (hashIndex + 1) % 100;
                bucketDistance[bucketIndex]++; //fix this
                collisions++;
            }
            linearTableInsert[hashIndex].keyValue = key;
            indirectInserts++;
        }
        bucketIndex++;
    }
void hashTable::insertOverload(hashNode(&chainedMain)[100], hashNode(&chainedOverflow)[100], int key) {
    
        int hashIndex = key % 100;
        if (chainedMain[hashIndex].keyValue == NULL) {
            chainedMain[hashIndex].keyValue = key;
            directInserts++;
        }
        else if (chainedMain[hashIndex].keyValue == key) {
            chainedMain[hashIndex].keyCount++;
            duplicateValues++;
        }
        else {
            if (chainedOverflow[hashIndex].keyValue == NULL) {
                chainedOverflow[hashIndex].keyValue = key;
                chainedMain[hashIndex].chainIndex = hashIndex;
            }
            else if (chainedOverflow[hashIndex].keyValue == key) {
                chainedOverflow[hashIndex].keyCount++;
                chainedMain[hashIndex].chainIndex = hashIndex;
            }
            else {
                while (chainedOverflow[hashIndex].keyValue != NULL && chainedOverflow[hashIndex].keyValue != key) {
                    hashIndex = (hashIndex + 1) % 100;
                }
                chainedOverflow[hashIndex].keyValue = key;
                chainedOverflow[hashIndex].chainIndex = (hashIndex - 1) % 100;
            }
        }
    }


void hashTable::insertOverload(hashNode(&chainedMain)[100], hashNode(&chainedOverflow)[100], vector<int> hashValues) {
    for (int key : hashValues) {
        int latestLink = key % 100;
        int nextLink = key % 100;
        int hashIndex = key % 100;
        int lastestPosition = 0;
        if (chainedMain[hashIndex].keyValue == NULL){
            chainedMain[hashIndex].keyValue = key;
            directInserts++;
        }
        else if (chainedMain[hashIndex].keyValue == key) {
            chainedMain[hashIndex].keyCount++;
            duplicateValues++;
        }
        else {
            if (chainedOverflow[hashIndex].keyValue == NULL) {
                chainedOverflow[hashIndex].keyValue = key;
                chainedMain[hashIndex].chainIndex = hashIndex;
            }
            else if (chainedOverflow[hashIndex].keyValue == key) { 
                chainedOverflow[hashIndex].keyCount++;
                chainedMain[hashIndex].chainIndex = hashIndex;
            }
            else {
                while (chainedOverflow[hashIndex].keyValue != NULL && chainedOverflow[hashIndex].keyValue != key) {
                    if ((chainedOverflow[hashIndex].keyValue) % 100 == hashIndex) {
                        lastestPosition = hashIndex;
                    }
                    hashIndex = (hashIndex + 1) % 100;
                }
                chainedOverflow[hashIndex].keyValue = key;
                chainedOverflow[latestLink].chainIndex = hashIndex;
            }
        }
    }
}