//Calcuate fewest coins required for change with a greedy algorithm
#include <cs50.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    //Require users change input to be a positive float
    float change;
    do
    {
        change = get_float("Change owed: ");
    }
    while (change < 0);

    //Convert change to integer to avoid imprecisions
    change *= 100;
    change = round(change);
    int cents = change;

    //Determine number of coins by taking largest coin possible (greedy)
    int num_coins = 0;
    while (cents > 0)
    {
        if (cents >= 25)
        {
            cents -= 25;
            num_coins++;
        }
        else if (cents >= 10)
        {
            cents -= 10;
            num_coins++;
        }
        else if (cents >= 5)
        {
            cents -= 5;
            num_coins++;
        }
        else
        {
            cents -= 1;
            num_coins++;
        }
    }

    //Print total number of coins
    printf("%i\n", num_coins);
}