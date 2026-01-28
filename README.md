# substitution-cipher
## CSCI4530 Project 1
### Team: Addison Thurston and William Kraus

For Project 1, we created an interactive program in which the user is given a selection of options that enables them to decrypt a given ciphertext. The program in decryption.cpp reads the ciphertext from ciphertext.txt and a dictionary from dictionary.txt that it uses to help the user decode the ciphertext. The program starts by assigning the most common English letters "ETAOINSHRDLCUMWFGYPBVKJXQZ" to the most common letters in the ciphertext, which in this case is "SGKCUEQHBZANILXOPRMFVWDJTY" to create the key "LROJNBTHUXAMPCFGSYEQIVKWZD" which works by telling the program what each letter the ciphertext's alphabet corresponds with. For example, the initial key created determines that A->L, B->R, ..., Z->D. Then after the initial step, the user is free to choose from a menu of options to further decrypt the text.

In the menu selection, the user has options numbered 1 through 6.  
These options are as follows:

### 1: Swap Two Letters in the Key  
By entering "1" the user may swap two letters in the key. It only reads the first two characters, so the rest of the line is ignored. In addition, it is only allowed to switch characters that are in the key, so the user can't add and remove letters as they please. If the switch is valid, the change will be applied and the decrypted text will be displayed.

### 2: AutoSolver  
By entering "2" the program runs up to 5 iterations of the AutoSolver. The AutoSolver attempts to improve the key by looping through the alphabet in the order of most frequent letters in English and attempting to swap it with the 4 next most common letters. In this process, it keeps track of the best key found according to the checkSet function, which applies our deduced heuristic for what makes the best English text. After all of these runs, the best key found is applied to the ciphertext.

### 3: Input Key  
By entering "3" the user may input any valid permutation of the English alphabet as a key they wish to use. As such, the key "ABCDEFGHIJKLMNOPQRSTUVWXYZ" will result in the original ciphertext being displayed, as each character will be remapped to itself.

### 4: Check Ciphertext Letter Frequency  
By entering "4" the program will display all letters of the ciphertext in descending frequency along with how many times that letter occurs in the ciphertext.

### 5: Display Highest Score & Best Key  
By entering "5" the program will display the best key found in the decryption process so far, as well as its corresponding score.

### 6: Quit  
Once the user is satisfied with the decryption of the ciphertext, they can end the program by entering "6"