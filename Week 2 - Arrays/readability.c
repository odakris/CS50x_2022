#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text : ");

    int letters = count_letters(text);
    printf("%i letters\n", letters);

    int words = count_words(text);
    printf("%i words\n", words);

    int sentences = count_sentences(text);
    printf("%i sentences\n", sentences);

    float L = ((float)letters / (float)words) * 100; // average number of letters per 100 words in the text
    // printf("L = %f\n", L);

    float S = ((float)sentences / (float)words) * 100; // average number of sentences per 100 words in the text
    // printf("S = %f\n", S);

    float index = 0.0588 * L - 0.296 * S - 15.8;
    // printf("index : %f\n", index);

    // Print GRADE
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int)round(index));
    }
}

// function that count letters
int count_letters(string text)
{
    // Initialization of letter count
    int letters = 0;

    for (int i = 0, lenght_text = strlen(text); i < lenght_text; i++)
    {
        if (islower(text[i]) || isupper(text[i])) // checking for lowercase and uppercase letter to only count letters
        {
            letters = letters + 1;
        }
    }
    return letters;
}

// function that count words
int count_words(string text)
{
    // Initialization of word count
    int words = 0;

    for (int i = 0, lenght_text = strlen(text); i < lenght_text; i++)
    {
        if (text[i] == ' ' || text[i] == '\0') // checking for space to only count words
        {
            words = words + 1;
        }
    }
    words = words + 1; // count the last word that haven't any space after it as it's the last word

    return words;
}

// function that count sentences
int count_sentences(string text)
{
    // Initialization of sentence count
    int sentences = 0;

    for (int i = 0, lenght_text = strlen(text); i < lenght_text; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?') // checking for '.' or '!' or '?' to only count sentences
        {
            sentences = sentences + 1;
        }
    }
    return sentences;
}
