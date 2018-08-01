//Apply red filter to image
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

//Change RGB values by
#define RGBTREDDIFF   0
#define RGBTGREENDIFF -250
#define RGBTBLUEDIFF  -250


int main(int argc, char *argv[])
{
    //Check two arguements given by user
    if (argc != 3)
    {
        fprintf(stderr, "You have entered %i arguement(s). Please enter two as: ./whodunit input-file output-file\n", argc - 1);
        return 1;
    }

    //Check input-file can be opened
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Cannpt open input-file: %s\n", argv[1]);
        return 2;
    }

    //Check output-file can be opened
    FILE *outptr = fopen(argv[2], "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Cannot create output-file: %s\n", argv[2]);
        return 3;
    }

    //Obtain file's metadata
    BITMAPFILEHEADER bfh;
    fread(&bfh, sizeof(BITMAPFILEHEADER), 1, inptr);
    BITMAPINFOHEADER bih;
    fread(&bih, sizeof(BITMAPINFOHEADER), 1, inptr);

    //Check file is (likely) a 24-bit uncompressed BMP 4.0
    if (bfh.bfType != 0x4d42 || bfh.bfOffBits != 54 || bih.biSize != 40 ||
        bih.biBitCount != 24 || bih.biCompression != 0)
    {
        fclose(inptr);
        fclose(outptr);
        fprintf(stderr, "Unsupported file format. Please give a 24-bit BMP 4.0 as input-file.\n");
        return 4;
    }

    //Write output-file's metadata
    fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, outptr);

    //Calculate file's padding
    int padding = (4 - (bih.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;


    // iterate over input-file's scanlines
    for (int i = 0, biHeight = abs(bih.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bih.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            //Red value
            if ((int)triple.rgbtRed + RGBTREDDIFF > 255)
            {
                triple.rgbtRed = 255;
            }
            else if ((int)triple.rgbtRed + RGBTREDDIFF < 0)
            {
                triple.rgbtRed = 0;
            }
            else
            {
                triple.rgbtRed += RGBTREDDIFF;
            }
            //Green value
            if ((int)triple.rgbtGreen + RGBTGREENDIFF > 255)
            {
                triple.rgbtGreen = 255;
            }
            else if ((int)triple.rgbtGreen + RGBTGREENDIFF < 0)
            {
                triple.rgbtGreen = 0;
            }
            else
            {
                triple.rgbtGreen += RGBTGREENDIFF;
            }

            //Blue value
            if ((int)triple.rgbtBlue + RGBTBLUEDIFF > 255)
            {
                triple.rgbtBlue = 255;
            }
            else if ((int)triple.rgbtBlue + RGBTBLUEDIFF < 0)
            {
                triple.rgbtBlue = 0;
            }
            else
            {
                triple.rgbtBlue += RGBTBLUEDIFF;
            }

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    //Close files and end program
    fclose(inptr);
    fclose(outptr);
    return 0;
}