#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
} candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();

    for (int i = 0; i < candidate_count; i++)
    {
        printf("name %s : %i votes\n", candidates[i].name, candidates[i].votes);
    }
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0) // check is name enter is in the list
        {
            candidates[i].votes = candidates[i].votes + 1; // add vote to corresponding candidate
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // sort candidates from the one with most votes to the one with less votes
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = 0; j < candidate_count - 1; j++)
        {
            if (candidates[i].votes < candidates[j + 1].votes)
            {
                string temp_name = candidates[i].name;          // temp is avoiding erasing data while switching data
                int temp_votes = candidates[i].votes;

                candidates[i].name = candidates[j + 1].name;
                candidates[i].votes = candidates[j + 1].votes;

                candidates[j + 1].name = temp_name;
                candidates[j + 1].votes = temp_votes;
            }
        }
    }

    printf("%s\n", candidates[0].name); // Print the winner

    for (int i = 0; i < candidate_count - 1; i++)
    {
        if (candidates[0].votes == candidates[i + 1].votes)     // check if there is ties and print every tie candidates
        {
            printf("%s\n", candidates[i + 1].name);
        }
    }
    return;
}
