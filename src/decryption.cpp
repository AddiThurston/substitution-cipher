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

// reads the frequency of letters in the ciphertext to a vector
void setCipherFreq(string& s, vector<pair<char, int> >& freq) {
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

// determines the character with the higher frequency to be greater
bool freqSort(pair<char, int>& a, pair<char, int>& b) {
    return a.second > b.second;
}

string decrypt(string& ciphertext, string& key) {
    string plaintext = ""; 
    for (char& c : ciphertext) {
        plaintext += key[c-'A'];
    }
    return plaintext;
}

void permuteKey(string& key, char a, char b) {
    for (char& c : key) {
        if (c == a) {
            c = b;
        } else if (c == b) {
            c = a;
        }
    }
}


size_t checkSet(const string& text, const unordered_set<string>& dict){
	string word;
	size_t count = 0;
	bool goodBits[text.size()];
	map<char, int> matchFreq;
	map<char, int> unmatchedFreq;
	map<char, int> certaintyMap;

    for (size_t i = 0; i < text.length(); i++) {
        goodBits[i] = false;
    }

	for(char c = 'A'; c<='Z'; c++){
		matchFreq.insert({c,0});
		unmatchedFreq.insert({c,0});
		certaintyMap.insert({c,0});
	}

	for (int i = 2; i <= 8; i++){
		for (size_t j = 0; j < text.size()-i; j++){
			const auto& sub = text.substr(j,i);
			auto it = dict.find(sub);
			if (it != dict.end()){
				for(size_t k = j; k <= j+i; k++){
					goodBits[k] = true;
				}
				for(char c : sub){
					matchFreq[c] += i;
				}
                int scores[] = {0,0,1,5,6,4,3,2,2};
				count += scores[i];
			}
		}
	}
	for (size_t i = 0; i<sizeof(goodBits); i++){
		if (goodBits[i] == true){
			matchFreq[text[i]]++;
		} else {
			unmatchedFreq[text[i]]++;
		}
	}

    for (size_t i = 0; i < text.length(); i++) {
        if (goodBits[i]) count++;
    }

	// cout << "Match certainty: " << endl;
	// for (const auto& p : matchFreq){
	// 	float matchNum = p.second;
	// 	float unmatchedNum = unmatchedFreq[p.first];
	// 	float sum = matchNum + unmatchedNum;
	// 	float certaintyPercent = (matchNum/sum) * 100;

	// 	cout << p.first << ": " << certaintyPercent << "%" << endl;
	// }

	return count;
}


void autoDecrypt(string& key, string& ciphertext, const unordered_set<string>& dict) {
    int highScore = checkSet(decrypt(ciphertext, key), dict);

    for (char a = 'A'; a <= 'Z'; a++) {
        for (char b = 'A'; b <= 'Z'; b++) {
            if (a == b) continue;
            permuteKey(key, a, b);
            int score = checkSet(decrypt(ciphertext, key), dict);
            if (score > highScore) {
                highScore = score;
            } else {
                permuteKey(key, a, b);
            }
        }
    }
}

void strToUpper(string& s) {
    for (char& c : s) {
        c = toupper(c);
    }
}


int main() {
    unordered_set<string> dict; // the dictionary from dictionary.txt
    string ciphertext;  // the ciphertext from ciphertext.txt
    vector<pair<char, int> > cipherFreq; // the frequency of all letters in the ciphertext
    vector<pair<char, int> > dictFreq;  // the frequency of all letters in the dictionary

    if (readToSet(dict, "dictionary.txt") == -1) {
        return 1;
    }
    cout << "Done reading dictionary file\n";

    if (readToString(ciphertext, "ciphertext.txt") == -1) {
        return 1;
    }
    cout << "Done reading ciphertext file\n";

    setCipherFreq(ciphertext, cipherFreq);
    cout << "Done counting frequency of ciphertext letters\n\n";
    sort(cipherFreq.begin(), cipherFreq.end(), freqSort);

    string commonTextFreq = "ETAOINSHRDLCUMWFGYPBVKJXQZ";
    // solution's frequency chart - "ETAINOSRHDCMLPUGWFYBVJKXQZ"

    // setup the key from the vector of frequencies
    string key(26, ' '); // 26 spaces
    for (int i = 0; i < 26; i++) {
        // set the key's alphabetical position of the ith most frequent cipher letter to the ith most frequent common text letter
        key[cipherFreq[i].first - 'A'] = commonTextFreq[i]; 
    }

    string solution = decrypt(ciphertext, key);
    cout << "Decrypted text using key: " << key << endl;
    cout << solution << endl;
    cout << "Initial Score: " << checkSet(solution, dict) << endl;

    // user interaction
    string input;
    cout << "1: Swap two letters in the key\n2: Autosolver\n3: Input Key\n4: Quit\n";
    getline(cin, input);
    while (input != "4") {
        if (input == "1") { // the user can swap 2 chars
            cout << "Which characters do you want to swap?\n";
            char a,b;
            cin >> a >> b;
            getline(cin, input);
            permuteKey(key, toupper(a), toupper(b));
        } else if (input == "2") {  // the user can let the autosolve function try to work on the text
            autoDecrypt(key, ciphertext, dict);
        } else if (input == "3") {  // the user can input a key
            cout << "Key: ";
            getline(cin, input);
            strToUpper(input);
            // check that the key is valid
            bool goodKey = true;
            for (char c : commonTextFreq) {
                if (input.find(c) == string::npos) {
                    goodKey = false;
                    break;
                }
            }
            if (goodKey && input.length() == 26) {
                key = input;
            } else {
                cout << "Invalid Key\n";
            }
        } else {
            cout << "Invalid input\n";
        }
        // print the current key, decrypted text, and score
        solution = decrypt(ciphertext, key);
        cout << "\nDecrypted text using key: " << key << endl;
        cout << solution << endl;
        cout << "Score: " << checkSet(solution, dict) << endl;
        cout << "1: Swap two letters in the key\n2: Autosolver\n3: Input Key\n4: Quit\n";
        getline(cin, input);
    }

    return 0;
}
