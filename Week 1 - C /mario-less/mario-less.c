#include <cs50.h> //library
#include <stdio.h>

// declaration of left pyramid function
void left_pyramid(int n_row);

int main(void)
{
    // n is height of pyramid
    int n_row;

    // asking the pyramid height to user until it matchs requirements
    do
    {
        n_row = get_int("How tall the pyramid should be? : ");
    }
    while (n_row < 1 || n_row > 8);

    left_pyramid(n_row);
}

// LEFT PYRAMID FUNCTION
void left_pyramid(n_row)
{
    // ROWS
    for (int i = 1; i <= n_row; i++)
    {
        // COLUMS
        for (int j = 1; j <= n_row - i; j++)
        {
            // print space
            printf(" ");
        }
        for (int k = 1; k <= i; k++)
        {
            // print a brick
            printf("#");
        }
        // move to next line
        printf("\n");
    }
}
