//Encrypt string with Ceaser's encryption by the specified key.
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
    //Check argument is valid number
    for (int i = 0, n = strlen(argv[1]) ; i < n ; i++)
    {
        //Skip negative sign
        (i == 0) ? ((argv[1][0] == '-') ? i++ : i) : i;

        if (!isdigit(argv[1][i]))
        {
            printf("You have entered %s, please enter a number.\n",  argv[1]);
            return 1;
        }

    }


    //Turn Key into positive integer < length alphabet
    int key = atoi(argv[1]);
    key %= LENALPHA;
    key += LENALPHA;
    key %= LENALPHA;

    //Get users text to encrypt.
    string plaintext = get_string("plaintext: ");
    string ciphertext = plaintext;

    //Apply Ceaser's encryption to each character that is a letter
    char c;
    for (int i = 0, n = strlen(plaintext); i < n ; i++)
    {
        c = plaintext[i];

        //Check letter and case
        if isalpha(c)
        {
            char initial_letter = isupper(c) ? 'A' : 'a';

            //Apply Encryption
            c -= initial_letter;
            c += key;
            c %= LENALPHA;
            c += initial_letter;
        }

        ciphertext[i] = c;

    }

    //Return ciphertext to screen
    printf("ciphertext: %s\n", ciphertext);
    return 0;
}