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


//Creates a certainty score based on whether or not letters appear more fequently
//in dictionary matched substrings or "nonsense" strings". Gives higher weight to
//letters found in longer words (max score around 4 letters). Works mostly as a 
//helper for autochecker to keep it from prioritizing garbage two letter words.
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
                int scores[] = {0,0,1,5,6,4,4,4,5};
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
	return count;
}

//Attempt to automatically decrypt the ciphertext by permuting the key
//until the key with the highest certainty score is obtained.
void autoDecrypt(string& key, string& ciphertext, const unordered_set<string>& dict) {
    int highScore = checkSet(decrypt(ciphertext, key), dict);
    string commonTextFreq = "ETAOINSHRDLCUMWFGYPBVKJXQZ";

    for (size_t i = 0; i < commonTextFreq.length(); i++) {  // loop through the characters in order of frequency in english
        char a = commonTextFreq[i];
        string bestKey = key;
        for (size_t j = i+1; j <= i+4 && j < commonTextFreq.length(); j++) {    
            // try swapping with the next 4 most frequent letters to see if it increases our score
            char b = commonTextFreq[j];
            permuteKey(key, a, b);
            int score = checkSet(decrypt(ciphertext, key), dict);
            if (score > highScore) {
                highScore = score;
                bestKey = key;
            }
        }
        key = bestKey;
    }
}

// helper function to turn the user inputted keys to uppercase
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
    cout << "Corresponding alphabet:     " << "ABCDEFGHIJKLMNOPQRSTUVWXYZ" << endl;
    cout << "Letters remapped using key: " << key << endl;
    cout << solution << endl;
    int highscore = checkSet(solution, dict);
    string bestKey = key;
    cout << "Initial Score: " << highscore << endl;

    // user interaction
    string input;
    string options = "1: Swap Two Letters in the Key\n2: AutoSolver\n3: Input Key\n4: Check Ciphertext Letter Frequency\n5: Display Highest Score & Best Key\n6: Quit\n";
    cout << options;
    getline(cin, input);
    while (input != "6") {
        if (input == "1") { // the user can swap 2 chars
            cout << "Which characters do you want to swap?\n";
            char a,b;
            cin >> a >> b;
            getline(cin, input);
            permuteKey(key, toupper(a), toupper(b));
        } else if (input == "2") {  // the user can let the autosolve function try to work on the text
            int bestScore = checkSet(decrypt(ciphertext, key), dict);
            for (int i = 0; i < 5; i++) {
                autoDecrypt(key, ciphertext, dict);
                int newScore = checkSet(decrypt(ciphertext, key), dict);
                if (newScore > bestScore) {
                    cout << "AutoSolver improved score by " << newScore-bestScore << " points with key: " << key << endl;
                    bestScore = newScore;
                } else if (newScore == bestScore) {
                    cout << "AutoSolver can no longer improve the current key\n";
                    break;
                }
            }
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
        } else if (input == "4") {  // display the ciphertext letter frequency
            for (const auto& pair : cipherFreq) {
                cout << pair.first << " - " << pair.second << std::endl;
            }
        } else if (input == "5") {
            cout << "High Score: " << highscore << endl;
            cout << "Best Key: " << bestKey << endl;
        } else {
            cout << "Invalid input\n";
        }
        
        solution = decrypt(ciphertext, key);
        int score = checkSet(solution, dict);
        if (score > highscore) {
            highscore = score;
            bestKey = key;
        }

        // print the current key, decrypted text, and score
        cout << "\nCorresponding alphabet:     " << "ABCDEFGHIJKLMNOPQRSTUVWXYZ" << endl;
        cout << "Letters remapped using key: " << key << endl;
        cout << solution << endl;
        cout << "Score: " << score << endl;
        cout << options;
        getline(cin, input);
    }

    return 0;
}
