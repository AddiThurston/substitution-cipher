# permutation-cypher
## CSCI4530 Project 1
### Team: Addison Thurston and William Kraus

For Project 1, we created an interactive program in which the user is given a selection of options
that enables them to decrypt a given ciphertext. The program in decryption.cpp reads the ciphertext 
from ciphertext.txt and a dictionary from dictionary.txt that it uses to help the user decode the
ciphertext. The program starts by assigning the most common english letters 
"ETAOINSHRDLCUMWFGYPBVKJXQZ" to the most common letters in the ciphertext, which in this case is
"SGKCUEQHBZANILXOPRMFVWDJTY" to create the key "LROJNBTHUXAMPCFGSYEQIVKWZD" which works by 
telling the program what each letter the ciphertext's alphabet corresponds with. For example,
the initial key created determines that A->L, B->R, ..., Z->D. Then after the initial step, the
user is free to choose from a menu of options to further decrypt the text.

In the menu selection, the user has options numbered 1 through 6. These options are as follows:

1: Swap Two Letters in the Key
    This enables the user
2: AutoSolver
3: Input Key
4: Check Ciphertext Letter Frequency
5: Display Highest Score & Best Key
6: Quit