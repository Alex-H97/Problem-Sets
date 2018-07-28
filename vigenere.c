//Encrypt string with Vigenere's encryption by the specified key.
#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENALPHA 26

int main(int argc, string argv[])
{
    //Check only one argument has been entered
    if (argc != 2)
    {
        printf("You have entered %i arguments, please enter only one.\n",  argc - 1);
        return 1;
    }
    //Check argument is valid word
    for (int i = 0, n = strlen(argv[1]) ; i < n ; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("You have entered %s, please enter an alphabetical word, with no digits or symbols.\n",  argv[1]);
            return 1;
        }

    }


    //Turn Key into array of sequence or integer keys.
    int key_len = strlen(argv[1]);
    int keys[key_len];
    char char_case;
    for (int i = 0; i < key_len ; i++)
    {
        char_case = isupper(argv[1][i]) ? 'A' : 'a';
        keys[i] = argv[1][i] - char_case;
    }

    //Get users text to encrypt.
    string plaintext = get_string("plaintext: ");
    string ciphertext = plaintext;

    //Apply Vigenere's encryption to each character that is a letter
    char c;
    for (int i = 0, j = 0, n = strlen(plaintext); i < n ; i++)
    {
        c = plaintext[i];

        //Check letter and case
        if isalpha(c)
        {
            j == key_len ? j %= key_len : j;
            char initial_letter = isupper(c) ? 'A' : 'a';

            //Apply Encryption
            c -= initial_letter;
            c += keys[j];
            c %= LENALPHA;
            c += initial_letter;

            j++;
        }

        ciphertext[i] = c;

    }

    //Return ciphertext to screen
    printf("ciphertext: %s\n", ciphertext);
    return 0;
}