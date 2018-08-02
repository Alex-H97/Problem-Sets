//Recover JPEG files from a FAT memory image
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define FATBLOCKBYTESIZE 512
#define LENJPEGIDENTIFIER 4


int main(int argc, char *argv[])
{
    //ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover memory-image\n");
        return 1;
    }

    //open input file
    FILE *memptr = fopen(argv[1], "r");
    if (!memptr)
    {
        fprintf(stderr, "Cannot open file: %s\n", argv[1]);
        return 2;
    }


    //define FAT block
    typedef uint8_t BYTE;
    BYTE *block = malloc(FATBLOCKBYTESIZE * sizeof(BYTE));
    //check valid memory adress
    if (block == NULL)
    {
        fclose(memptr);
        fprintf(stderr, "MEMORY ALLOCATION ERROR\n");
        return 4;
    }


    //generate file name
    int fnum = 0;
    char fname[8];

    //continouly read file chunks of FAT blocks (512-bytes)
    //until end of file or error
    int bool_jpeg;
    FILE *jpgptr;
    while (fread(block, FATBLOCKBYTESIZE, 1, memptr) == 1)
    {
        //check initial bytes for JPEG identifier
        bool_jpeg = 1;
        for (int i = 0 ; i < LENJPEGIDENTIFIER && bool_jpeg ; i++)
        {
            //check each byte against its case
            switch (i)
            {
                case 0:
                    bool_jpeg = (block[i] == 0xFF) ? bool_jpeg : 0;
                    break;
                case 1:
                    bool_jpeg = (block[i] == 0xD8) ? bool_jpeg : 0;
                    break;
                case 2:
                    bool_jpeg = (block[i] == 0xFF) ? bool_jpeg : 0;
                    break;
                case 3:
                    bool_jpeg = (0xDF < block[i] && block[i] < 0xF0) ? bool_jpeg : 0;
                    break;
            }
        }

        //if block is start of new JPEG
        if (bool_jpeg)
        {
            //close previous file
            if (fnum != 0)
            {
                fclose(jpgptr);
            }

            //open new file
            sprintf(fname, "%03i.jpg", fnum);
            jpgptr = fopen(fname, "w");
            fnum++;

            //check valid file
            if (jpgptr == NULL)
            {
                free(block);
                fclose(memptr);
                fprintf(stderr, "Could not create %s.\n", fname);
                return 3;
            }

            //write block to new file
            fwrite(block, FATBLOCKBYTESIZE, 1, jpgptr);
        }
        else if (fnum != 0)
        {
            //write out to file
            fwrite(block, FATBLOCKBYTESIZE, 1, jpgptr);
        }
    }

    //free all heap memory and close last JPEG file
    free(block);
    fclose(jpgptr);


    //check for EOF or error when finished reading
    if (feof(memptr))
    {
        //free all heap memory, close all files and exit
        fclose(memptr);
        return 0;
    }
    else
    {
        fprintf(stderr, "Error reading file: %s\n", argv[1]);
        //free all heap memory, close all files and exit with error
        fclose(memptr);
        return 5;
    }
}