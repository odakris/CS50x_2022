#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>


// définition of how many byte to read per block
#define BLOCK_SIZE 512
// définition of how many byte for filename output
#define NAME_SIZE 10
// BYTE type definition
typedef uint8_t BYTE;
// buffer definition in which will be stored read data
BYTE buffer[BLOCK_SIZE];
// counting images for file name
int image_count = 0;
// output_file is a char chain where the name of images is stored
char output_file[NAME_SIZE];
// creation of a pointer to an img file
FILE *img = NULL;

int main(int argc, char *argv[])
{
    if (argc == 2) // check if the user file name has only two argument
    {
        // opening file entered by user
        FILE *input_file = fopen(argv[1], "r");

        // check if input_file is readable
        if (input_file == NULL)
        {
            printf("Could not open file.\n");
            return 1;
        }
        else
        {
            // loop reading input_file until there is no JPEG left
            while (fread(buffer, 1, BLOCK_SIZE, input_file) == BLOCK_SIZE)
            {
                // checking for JPEG signature
                if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
                {
                    // in case of first image detected
                    if (image_count == 0)
                    {
                        // setting up image name for current image
                        sprintf(output_file, "%03i.jpg", image_count);
                        image_count = image_count + 1;
                        // opening image to write
                        img = fopen(output_file, "w");
                    }
                    else
                    {
                        // if it's not the first image detected then closing last image to start recovering the next one
                        fclose(img);
                        // setting up image name for current image
                        sprintf(output_file, "%03i.jpg", image_count);
                        image_count = image_count + 1;
                        // opening image to write
                        img = fopen(output_file, "w");
                    }
                }
                if (image_count != 0)
                {
                    // if a JPEG has been found then write the corresponding image data at the buffer address into img file
                    fwrite(&buffer, 1, BLOCK_SIZE, img);
                }
            }

            // closing last image and freeing allocated memory
            fclose(img);
            // closing input_file and freeing allocated memory
            fclose(input_file);

            return 0;
        }
    }
    else
    {
        printf("Usage: ./recover IMAGE\n"); // abord programm if the user did not enter valid format
        return 1;
    }
}
