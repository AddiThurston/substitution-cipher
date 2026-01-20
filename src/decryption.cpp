#include <iostream>
#include <fstream>
#include <unordered_set>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// reads a file to an unordered_set of strings
int readToSet(unordered_set<string>& d, string fileName) {
    ifstream fin(fileName);
    if (!fin.is_open()) {
        cout << "Error opening " << fileName << endl;
        return -1;
    }

    string word;
    while (fin >> word) {
        d.insert(word);
    }
    fin.close();
    return 0;
}

// reads a file to a single string
int readToString(string& s, string fileName) {
    s = "";
    ifstream fin(fileName);
    if (!fin.is_open()) {   // check that the file is opened correctly
        cout << "Error opening " << fileName << endl;
        return -1;
    }

    string input;
    while (fin >> input) {
        s += input;  // keep appending the string with the new input
    }
    fin.close();
    return 0;
}

// reads the frequency of letters to a map
void getCharFreq(string& s, vector<pair<char, int> >& freq) {
    map<char, int> freqMap;
    // initialize the frequency of the characters to 0
    for (char c = 'A'; c <= 'Z'; c++) {
        freqMap.insert({c, 0});
    }

    for (char& c : s) {
        freqMap[c]++; // increment the frequency of the current character by 1;
    }
    for (const auto& p : freqMap) {
        freq.push_back({p.first, p.second});
    }
    
}

bool freqSort(pair<char, int>& a, pair<char, int>& b) {
    return a.second > b.second;
}

int main() {
    unordered_set<string> dict; // the dictionary from dictionary.txt
    string ciphertext;  // the ciphertext from ciphertext.txt
    vector<pair<char, int> > cipherFreq; // the frequency of all letters in the ciphertext

    if (readToSet(dict, "dictionary.txt") == -1) {
        return 1;
    }
    cout << "Done reading dictionary file\n";

    if (readToString(ciphertext, "ciphertext.txt") == -1) {
        return 1;
    }
    cout << "Done reading ciphertext file\n";

    getCharFreq(ciphertext, cipherFreq);
    cout << "Done counting frequencies of ciphertext letters\n";
    sort(cipherFreq.begin(), cipherFreq.end(), freqSort);
    for (const auto& pair : cipherFreq) {
        std::cout << pair.first << " - " << pair.second << std::endl;
    }
    
    return 0;
}