// Resizes a BMP file (more comfortable)
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"


//Function Declarations
int sisdigit(char *string);
int iround(double x);
int itrunc(double x);
void iterate_pixels(FILE *inptr, FILE *outptr, int ipadding, int opadding,
                    int wfactor, double wspacing, LONG ibiWidth);


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
        fprintf(stderr, "The value of 'n' must be a positive decimal (2 d.p.) less than or equal to 100.\n");
        return 1;
    }
    // deal with floating point error
    int factor = iround(atof(argv[1]) * 100);
    if (factor <= 0 || factor > 10000)
    {
        fprintf(stderr, "The value of 'n' must be a positive decimal (2 d.p.) less than or equal to 100.\n");
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


    // store infile's required metadata
    LONG ibiHeight = bi.biHeight;
    LONG ibiWidth = bi.biWidth;
    int ipadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // edit outfile's metadata
    bi.biWidth = iround((double)(bi.biWidth * factor) / 100);
    bi.biHeight = iround((double)(bi.biHeight * factor) / 100);
    int opadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi.biSizeImage = ((bi.biWidth * sizeof(RGBTRIPLE)) + opadding) * abs(bi.biHeight);
    bf.bfSize = bf.bfOffBits + bi.biSizeImage;


    // catch zero pixel images
    if (bi.biWidth == 0 || bi.biHeight == 0)
    {
        fprintf(stderr,
                "The resize factor %f is too small, outfile will have no pixels. Please pick a larger decimal.\n",
                atof(argv[1]));
        return 2;
    }


    // write outfile's metadata
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    // determine width and horizontal stretch factors
    int wfactor = abs(itrunc((double)bi.biWidth / ibiWidth));
    int hfactor = abs(itrunc((double)bi.biHeight / ibiHeight));

    // determine remaining pixels after integer stretch factors
    int wremain = bi.biWidth % ibiWidth;
    int hremain = abs(bi.biHeight % ibiHeight);

    // determine which columns/rows to stretch from remaining pixels,
    // and catch div-0 by setting out of range of columns/rows iterations
    double wspacing = (wremain != 0) ? (double)ibiWidth / wremain : (double)ibiWidth + 1;
    double hspacing = (hremain != 0) ? (double)abs(ibiHeight) / hremain : (double)abs(ibiHeight) + 1;


    // iterate over infile's scanlines
    int hcounter = 1;
    for (int i = 0; i < abs(ibiHeight); i++)
    {
        // repeat line factor times (vertical resize)
        for (int y = 0; y < hfactor; y++)
        {
            // iterate over pixels in scanline
            iterate_pixels(inptr, outptr, ipadding, opadding, wfactor, wspacing, ibiWidth);
        }

        // account for fractional factors
        if (i == iround(hspacing * hcounter) - 1)
        {
            // rewrite previous line every spacing
            iterate_pixels(inptr, outptr, ipadding, opadding, wfactor, wspacing, ibiWidth);
            hcounter++;
        }

        // move to next scan line if written out
        fseek(inptr, +(ibiWidth * sizeof(RGBTRIPLE)) + ipadding, SEEK_CUR);
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

    int dotCounter = 0;
    for (int n = strlen(string) ; i < n ; i++)
    {
        // iterate over characters in string

        // check character is a digit or a dot
        if (! isdigit(string[i]) && string[i] != '.')
        {
            return 0;
        }

        if (string[i] == '.')
        {
            dotCounter++;
        }
    }

    // check only one decimal place
    if (dotCounter > 1)
    {
        return 0;
    }

    return 1;
}


int iround(double x)
{
    // return rounded integer
    return (int)round(x);
}


int itrunc(double x)
{
    // return truncated integer
    return (int)trunc(x);
}


void iterate_pixels(FILE *inptr, FILE *outptr, int ipadding, int opadding,
                    int wfactor, double wspacing, LONG ibiWidth)
{
    // iterate over pixels in scanline
    for (int j = 0, wcounter = 1; j < ibiWidth; j++)
    {
        // temporary RGB triple storage
        RGBTRIPLE triple;

        // read RGB triple from infile
        fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

        // write RGB triple to outfile
        for (int x = 0; x < wfactor; x++)
        {
            // repeat pixel factor times (horizontal resize)
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // account for fractional factors
        if (j == iround(wspacing * wcounter) - 1)
        {
            // every spacing write previous RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            wcounter++;
        }

    }

    // account for padding
    fseek(inptr, ipadding, SEEK_CUR);
    for (int k = 0; k < opadding; k++)
    {
        fputc(0x00, outptr);
    }

    // move infile pointer back to start of scanline
    fseek(inptr, -(ibiWidth * sizeof(RGBTRIPLE)) - ipadding, SEEK_CUR);
}