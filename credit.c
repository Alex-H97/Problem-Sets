//Check credit card number against Luhn's algorithm
#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Ask for card number make sure number is positive
    long long cnum;
    do
    {
        cnum = get_long_long("Number: ");
    }
    while (cnum < 0);


    //Declare and define card type variables
    string ctype = "INVALID";
    char part_check = 'n';

    //Declare and define Luhn's algorithm variables
    int digit;
    int digit_length = 0;
    int evn_sum = 0;
    int odd_sum = 0;

    //Iterate through each digit from last to first, break for too many digits
    bool too_long = false;
    while (cnum > 0 && too_long == false)
    {
        digit = cnum % 10;
        cnum -= digit;
        cnum /= 10;
        digit_length++;

        //Check final digits for card type, or break if too many digits
        if (digit_length > 12)
        {
            if (digit_length == 13 && digit == 4)
            {
                ctype = "VISA";
            }
            else if (digit_length == 14)
            {
                if (digit == 4 || digit == 7)
                {
                    part_check = 'a';
                }
                else
                {
                    ctype = "INVALID";
                }
            }
            else if (digit_length == 15)
            {
                if (digit == 3 && part_check == 'a')
                {
                    ctype = "AMEX";
                }
                else if (digit >= 1 && digit <= 5)
                {
                    part_check = 'm';
                }
            }
            else if (digit_length == 16)
            {
                if (digit == 5 && part_check == 'm')
                {
                    ctype = "MASTERCARD";
                }
                else if (digit == 4)
                {
                    ctype = "VISA";
                }
                else
                {
                    ctype = "INVALID";
                    too_long = true;
                }
            }
        }

        //Apply Luhn's Algorithm
        if (digit_length % 2 == 0)
        {
            //Even digits: double and sum digits
            digit *= 2;
            evn_sum += digit % 10;
            digit -= digit % 10;
            digit /= 10;
            evn_sum += digit % 10;
        }
        else
        {
            //Odd digits: sum
            odd_sum += digit;
        }

    }

    if ((evn_sum + odd_sum) % 10 != 0)
    {
        ctype = "INVALID";
    }
    printf("%s\n", ctype);

}


//Define Functions

