// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// counting how many word is read from dictionary
int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // determine hash index for current word
    int hash_index = hash(word);

    // loop searching for current word in the hash table
    // cursor will move through linked list at table[hash_index] until the word is found or until NULL
    for (node *cursor = table[hash_index]; cursor != NULL; cursor = cursor->next)
    {
        // insensitive case word comparison
        if (strcasecmp(cursor->word, word) == 0)
        {
            // if word is in the dictionary return true
            return true;
        }
    }
    // if not return false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // I tried to implement the hash function but couldn't figure it out after all
    // and as I don't want to copy and paste a working code from someone else I just skipped it and leave it as is it
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // opening dictionary
    FILE *file = fopen(dictionary, "r");

    // check if dictionary is readable
    if (file == NULL)
    {
        return false;
    }

    // word variable in which each word from dictionary will be temporary stored
    char word[LENGTH];

    // loop reading file until there is no word left to read
    while (fscanf(file, "%s", word) != EOF)
    {
        // counting how many word are imported from dictionary for size function
        word_count = word_count + 1;

        // node creation for current read word
        node *n = malloc(sizeof(node));

        // checking if there is enough memory space
        if (n == NULL)
        {
            return false;
        }
        // copy current word into node->word
        strcpy(n->word, word);
        // initialize node link to next word
        n->next = NULL;

        // using hash function to determine at which index should be store the current word
        int hash_index = hash(word);

        // setting new word pointer
        n->next = table[hash_index];

        // make the hash table at the current hash index starting with current added node
        // last added word is first of the linked list
        table[hash_index] = n;
    }

    // closing opened file (dictionary) to free allocated memory
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // returning word_count calculated in LOAD function while importing dictionary
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Loop going through every buckets of the Hash table
    for (int i = 0; i < N; i++)
    {
        // creation of cursor pointer
        node *cursor = table[i];
        // creation of a temporary pointer equal to cursor
        node *tmp = cursor;

        // Loop running until there is no memory left to free
        while (cursor != NULL)
        {
            // setting cursor pointer to next word in the linked list
            cursor = cursor->next;
            // freeing current memory
            free(tmp);
            // setting tmp to cursor in order to still have access to next memory word to free in the linked list
            tmp = cursor;
        }
    }
    return true;
}
