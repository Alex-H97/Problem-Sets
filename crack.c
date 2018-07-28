//Decrypt a password encrypted with C's DES-based "crypt" funtion.
#define _XOPEN_SOURCE
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LENPASS 5
#define LENCHARS 52
#define CAPSTART 26

//Structure Declarations
struct tuple
{
    char c;
    int infront;
};

//Function Declarations
struct tuple next_char(char c, int pos);


//List of most frequently used password characters to quicken search, split into lower and upper case,
//found [28/07/2018] at: https://csgillespie.wordpress.com/2011/06/16/character-occurrence-in-passwords/
char Char_freq[LENCHARS] = {'e', 'a', 'o', 'r', 'i', 'n', 's', 'l', 't',
                            'm', 'd', 'c', 'y', 'b', 'h', 'g', 'u', 'p',
                            'k', 'f', 'j', 'w', 'v', 'z', 'x', 'q',
                            'S', 'E', 'A', 'R', 'M', 'T', 'B', 'N', 'O',
                            'L', 'I', 'C', 'D', 'P', 'H', 'J', 'K', 'G',
                            'Y', 'U', 'F', 'W', 'V', 'X', 'Z', 'Q'
                           };


int main(int argc, string argv[])
{
    //Check only one command line argument entered
    if (argc != 2)
    {
        printf("You have entered %i arguments, please enter only hash.\n",  argc - 1);
        return 0;
    }


    //Get salt
    char salt[2];
    for (int n = 0; n < 2 ; n++)
    {
        salt[n] = argv[1][n];
    }


    //Password string declaration
    string password = (string)malloc(LENPASS * sizeof(char));
    password[0] = Char_freq[CAPSTART];


    //Initialise varaibles for loop
    bool cracked = false;
    int i = 0;
    int num_chars = 1;
    struct tuple nextv;
    do
    {
        //Cycle character and check if character infront needs changing repeatedly
        nextv = next_char(password[i], i);
        while (nextv.infront)
        {
            //Check if character infront is the first letter of password
            if (i == 0)
            {
                //Increase password characters and start initial characters capitalised
                eprintf("Change first letter!\n");
                i = num_chars;
                num_chars++;
                password[0] = Char_freq[CAPSTART];
                nextv.c = Char_freq[0];
                nextv.infront = 0;
            }

            else
            {
                //Move character pointer infront then cycle and check infront again
                password[i] = nextv.c;
                i--;
                nextv = next_char(password[i], i);
            }
        }
        password[i] = nextv.c;

        //Move character pointer back to last character in password
        i = num_chars - 1;


        //Check if hashes match
        strcmp(crypt(password, salt), argv[1]) == 0 ? (eprintf("Found it!\n"), cracked = true) : i;
    }
    while (!cracked && strlen(password) < LENPASS + 1);

    //Free password from memory (avoids memory problems)
    printf("%s\n", password);
    free(password);
    return 0;
}


//Function Defintions
struct tuple next_char(char c, int pos)
{
    //Determine indice in array
    int i;
    for (i = 0 ; i < LENCHARS ; i++)
    {
        if (c == Char_freq[i]) {break;}
    }

    //Cycle through characters in array, by checking if last element in array
    struct tuple rtn;
    rtn.infront = 0;
    switch (i)
    {
        case LENCHARS - 1:
            //Check if initial character in password, which starts capitalised
            pos != 0 ? (i = 0, rtn.infront = 1) : (i = 0);
            break;
        default:
            //Check if initial character in password, which starts capitalised
            (pos == 0 && i == 25) ? (i++, rtn.infront = 1) : i++;
            break;
    }

    rtn.c = Char_freq[i];
    return rtn;
}