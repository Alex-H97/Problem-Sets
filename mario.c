//Mario less, print half pyramid of specified height
#include <cs50.h>
#include <stdio.h>

//Function Declarations
void printf_rpchar(char chrct, int num_times);


int main(void)
{
    //Check user input is an integer [0,23]
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 0 || height > 23);

    //Print pyramid of specified height
    for (int i = 0 ; i < height; i ++)
    {
        printf_rpchar(' ', height - (i + 1));
        printf_rpchar('#', i + 2);
        printf("\n");
    }

}



//Function Definitions
void printf_rpchar(char chrct, int num_times)
{
    //Print a character multiple times.
    for (int j = 0; j < num_times; j++)
    {
        printf("%c", chrct);
    }
}