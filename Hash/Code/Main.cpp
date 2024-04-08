#include "hash.h"
using namespace std;

bool isEmpty(hashNode(&hashTable)[100]) {
    for (int i = 0; i <= 99; i++) {
        if (hashTable[i].keyValue == NULL) {
            return false;
        }
    }
    return true; 
}

int main() {
    hashNode linearTable[100];
    hashNode searchQueue[100];
    hashNode chainedMain[100];
    hashNode chainedOverflow[100];
    hashTable linearHashArray;
    hashTable chainedHash;
    char inputOption;
    ifstream inputFile;
    vector<int> hashValues;

    srand(time(NULL));
    int index = 0;
    cout << "Enter test name: ";
    getline(cin, linearHashArray.testName);
    linearHashArray.testName += ".txt";

    while (true) {
        cout << "Choose input method - (F)ile or (R)andom: ";
        cin >> inputOption;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

        inputOption = toupper(inputOption);

        if (inputOption == 'R' || inputOption == 'F') {
            break;
        }
        else {
            cout << "Invalid input. Please enter 'F', 'f', 'R', or 'r'." << endl;
        }
    }

    switch (inputOption) {
    case 'F': 
    {
        cout << "Enter filename: ";
        cin >> linearHashArray.fileName;
        linearHashArray.fileName += ".txt";

        hashValues = linearHashArray.readAndCheckInput(linearHashArray.fileName);

        if (hashValues.size() > 100) {
            auto eraseStart = hashValues.begin() + 100;
            hashValues.erase(eraseStart, hashValues.end());
        }
        for (int i = 0; i < hashValues.size(); i++) {
            linearHashArray.insertLinear(searchQueue, hashValues.at(i));
        }
        linearHashArray.clearVector();

        linearHashArray.insertLinear(linearTable, hashValues);
        chainedHash.insertOverload(chainedMain, chainedOverflow, hashValues);
        linearHashArray.searchLinear(linearTable, searchQueue);
        chainedHash.searchChained(chainedMain, chainedOverflow, searchQueue);
        inputFile.close();
    }
    break;

    case 'R':
    {
        while (!isEmpty(linearTable)) {
            int keyValue = rand() % 1000;
            linearHashArray.insertLinear(linearTable, keyValue);
            linearHashArray.insertLinear(searchQueue, keyValue);
            chainedHash.insertOverload(chainedMain, chainedOverflow, keyValue);
        }
        linearHashArray.searchLinear(linearTable, searchQueue);
        chainedHash.searchChained(chainedMain, chainedOverflow, searchQueue);
        inputFile.close();
    }
    break;

    default:
        cout << "Invalid input. Please enter 'F' or 'R'." << endl;
        return 1;
    }
    linearHashArray.printStats(linearTable, chainedMain, chainedOverflow, linearHashArray, chainedHash);
    return 0;
}

