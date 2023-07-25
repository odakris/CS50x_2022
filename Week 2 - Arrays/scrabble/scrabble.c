#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // initialization of retruned word score
    int score = 0;

    // loop to calculate score of a word / strlen give how many letter there is in a string
    for (int i = 0, word_lenght = strlen(word); i < word_lenght; i++)
    {
        if (islower(word[i]))
        {
            score += POINTS[word[i] - 'a']; // subtract ASCII value of 'a' (here 97) to the ASCII value of the corresponding letter
        }                                   // word[i] - 'a' (or 97) subtraction gives location of corresponding points // for example word[i] = 'c' & 'c' = 99 and 'a' = 97 so 99 - 97 = 2 and POINTS[2] is the number of points for letter 'c'
        else if (isupper(word[i]))
        {
            score += POINTS[word[i] - 'A']; // subtract ASCII value of 'A' (here 65) to the ASCII value of the corresponding letter
        }
    }
    return score;
}
