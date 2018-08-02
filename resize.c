// Resizes a BMP file (less comfortable)
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"


//Function Declarations
int sisdigit(char *string);


int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // ensure factor is valid
    if (! sisdigit(argv[1]))
    {
        fprintf(stderr, "The value of 'n' must be a positive integer less than or equal to 100.\n");
        return 1;
    }
    // ensure in range
    int factor = atoi(argv[1]);
    if (factor <= 0 || factor > 100)
    {
        fprintf(stderr, "The value of 'n' must be a positive integer less than or equal to 100.\n");
        return 1;
    }


    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 1;
    }


    // read infile's metadata
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);


    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 1;
    }


    // store infile's required metadata and padding
    LONG ibiHeight = bi.biHeight;
    LONG ibiWidth = bi.biWidth;
    int ipadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // edit outfile's metadata and padding
    bi.biWidth *= factor;
    bi.biHeight *= factor;
    int opadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi.biSizeImage = ((bi.biWidth * sizeof(RGBTRIPLE)) + opadding) * abs(bi.biHeight);
    bf.bfSize = bf.bfOffBits + bi.biSizeImage;


    // write outfile's metadata
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    // iterate over infile's scanlines
    for (int i = 0; i < abs(ibiHeight); i++)
    {
        // repeat line factor times (vertical resize)
        for (int y = 0; y < factor; y++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < ibiWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                for (int x = 0; x < factor; x++)
                {
                    // repeat pixel factor times (horizontal resize)
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // account for padding
            fseek(inptr, ipadding, SEEK_CUR);
            for (int k = 0; k < opadding; k++)
            {
                fputc(0x00, outptr);
            }

            // move infile pointer back to start of scanline
            if (y < factor - 1)
            {
                fseek(inptr, -(ibiWidth * sizeof(RGBTRIPLE)) - ipadding, SEEK_CUR);
            }
        }
    }

    // close files and end
    fclose(inptr);
    fclose(outptr);
    return 0;
}


// Function Definitions
int sisdigit(char *string)
{
    //Check string is a number

    // if initial character is a 'minus', start at second character
    int i = (string[0] == '-') ? 1 : 0;

    for (int n = strlen(string) ; i < n ; i++)
    {
        // iterate over characters in string

        //check character is a digit
        if (! isdigit(string[i]))
        {
            return 0;
        }
    }
    return 1;
}
