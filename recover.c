// Recover JPG files from "Memory card".

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t  BYTE;

int main (int argc, char *argv[]){
    if (argc != 2 )
    {
        fprintf(stderr, "Wrong input. Use ./recover FILENAME\n");
        return 1;
    }

    // Name the inputfile
    char *infile = argv[0];

    //Open datafile and check if can be opened.
    FILE* data = fopen(infile, "r");

    if (data == NULL)
    {
        fprintf(stderr, "Could not open file\n");
        return 2;
    }

    // Variables used later on.
    BYTE buffer[512];
    int jpg_count = 0;
    int jpgfound = 0;
    FILE* img = NULL;

    // Read over every block
    while  (fread(buffer, 512, 1, data) == 1)
    {
        //check if JPG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xe0) == 0xe0)
        {
            //if JPG, take note. If start is new JPG, close previous file.
            if (jpgfound == 1)
            {
                fclose(img);
            }
            else
            {
                jpgfound = 1;
            }
            char filename[8];
            sprintf(filename, "%03i.jpg", jpg_count);
            img = fopen(filename, "a");
            jpg_count++;
        }
        //if JPG, but not start, continue writing

        if (jpgfound == 1)
        {
        fwrite(&buffer, 512, 1, img);
        }
    }

    return 0;
}