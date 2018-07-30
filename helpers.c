// Helper functions for music

#include <cs50.h>
#include <math.h>
#include <string.h>

#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    //Convert characters to integers and apply calculation
    int a = fraction[0] - '0', b = fraction[2] - '0';
    return a * (8 / b);
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    int l = strlen(note);
    //Account for accidentals
    int n = (l == 3) ? (note[1] == '#' ? +1 : -1) : 0;

    //Account for octave
    int octave = note[l - 1] - '0';
    n += (octave - 4) * 12;

    //Account for note
    switch (note[0])
    {
        case 'C':
            n -= 9;
            break;
        case 'D':
            n -= 7;
            break;
        case 'E':
            n -= 5;
            break;
        case 'F':
            n -= 4;
            break;
        case 'G':
            n -= 2;
            break;
        case 'B':
            n += 2;
            break;
        default:
            break;
    }

    //Compute frequency and return rounded integer in Hertz
    double freq = 440 * pow(2, (double)n / 12);
    return (int)round(freq);

}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    if (strcmp(s, "") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
