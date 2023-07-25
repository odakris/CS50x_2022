#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// argc return number of string input in the prompt by user
// argv retrun what the user put in the prompt

int main(int argc, string argv[])
{
    string KEY = argv[1];

    if (argc == 2) // check if the user enter the key
    {
        if (strlen(KEY) == 26) // check lenght of key
        {
            for (int i = 0; i < strlen(KEY); i++)
            {
                if (isalpha(KEY[i]) == false) // check is key is only alphabetic
                {
                    printf("Key must be only alphabetic\n");
                    return 1;
                }
            }
            for (int i = 0; i < strlen(KEY); i++) // check if each letter are unique in the key
            {
                for (int j = 0; j < strlen(KEY); j++)
                {
                    if (i != j)
                    {
                        if (KEY[i] == KEY[j])
                        {
                            printf("letters must be unique\n");
                            return 1;
                        }
                    }
                }
            }
        }
        else
        {
            printf("Key must contain 26 characters.\n"); // abord programm if the user enter wrong or not valid key
            return 1;
        }
    }
    else
    {
        printf("Key is required to continue\n"); // abord programm if the user did not enter the key or more string that required
        return 1;
    }

    printf("plaintext:  ");
    string plaintext = get_string("");

    printf("ciphertext: ");

    for (int i = 0, lenght_plaintext = strlen(plaintext); i < lenght_plaintext; i++)
    {
        string ciphertext[strlen(plaintext)]; // initialization of ciphertext string

        if (isalpha(plaintext[i]))
        {
            if (isupper(plaintext[i])) // uppercase
            {
                int position = plaintext[i] - 'A';
                printf("%c", toupper(KEY[position]));
            }
            if (islower(plaintext[i])) // lowercase
            {
                int position = plaintext[i] - 'a';
                printf("%c", tolower(KEY[position]));
            }
        }
        else
        {
            printf("%c", plaintext[i]); // print no-alphabetic charactere
        }
    }
    printf("\n");
    return 0;
}

// store la key dans un string comme dans scrabble (ex KEY = YTNSHKVEFXRBAUQZCLWDMIPGJO donc KEY[3] = N)
// store les lettres de l'alphabet dans l'ordre pour connaitre son ordre ( ex: C = 3e lettre)
