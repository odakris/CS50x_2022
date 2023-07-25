#include <cs50.h> //library
#include <stdio.h>

// FULL PYRAMID FUNCTION DECLARATION
void full_pyramid(int n_row);

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

    full_pyramid(n_row);
}

// FULL PYRAMID FUNCTION
void full_pyramid(n_row)
{
    for (int i = 1; i <= n_row; i++) // ROWS
    {
        // LEFT PYRAMID
        for (int j = 1; j <= n_row - i; j++) // COLS
        {
            printf(" "); // print space of left pyramid side
        }
        for (int j = 1; j <= i; j++) // COLS
        {
            printf("#"); // print bricks of left pyramid side
        }
        // SPACE BETWEEN PYRAMIDS
        printf("  ");
        // RIGHT PYRAMID
        for (int j = 1; j <= i; j++) // COLS
        {
            printf("#"); // print bricks of right pyramid side
        }
        // move to next row
        printf("\n");
    }
}
