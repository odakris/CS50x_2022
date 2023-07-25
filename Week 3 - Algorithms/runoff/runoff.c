#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
} candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {
        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // TODO

    for (int i = 0; i < candidate_count; i++)
    {
        // check for candidate name match and store corresponding index into preference
        if (strcmp(name, candidates[i].name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // loop checking for voter preferences
    for (int i = 0; i < voter_count; i++)
    {
        int j = 0; // initialization of other preferences checking if 1st preference is true (eliminated)

        // check if candidate is still into the race
        if (candidates[preferences[i][j]].eliminated == false)
        {
            candidates[preferences[i][j]].votes = candidates[preferences[i][j]].votes + 1;
        }
        // If candidate is already eliminated
        else if (candidates[preferences[i][j]].eliminated == true)
        {
            // checking for candidate still in the race if 1st choice is out
            do
            {
                j++;
                if (candidates[preferences[i][j]].eliminated == false)
                {
                    candidates[preferences[i][j]].votes = candidates[preferences[i][j]].votes + 1;
                }
            }
            while (candidates[preferences[i][j]].eliminated == true);
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // TODO
    int majority = voter_count / 2;

    if (voter_count % 2 != 0) // check if voter_count is even or odd
    {
        majority = majority + 1; // add 1 to majority if voter_count is odd
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > majority && candidates[i].eliminated == false)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // TODO
    int min;
    int temp_min; // variable to store temporary minimum

    for (int i = 0; i < candidate_count - 1; i++)
    {
        // Check if candidates[i] is still in the race
        if (candidates[i].eliminated == false)
        {
            if (candidates[i].votes < candidates[i + 1].votes)
            {
                temp_min = candidates[i].votes;
            }
            // Check if candidates[i+1] is still in the race
            else if (candidates[i + 1].eliminated == false)
            {
                temp_min = candidates[i + 1].votes;
            }
            // compare temporary min to previous min
            if (temp_min <= min)
            {
                min = temp_min;
            }
        }
    }
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == false)
        {
            // check if any candidates has more votes than min, then there is no tie
            if (candidates[i].votes != min)
            {
                return false;
            }
        }
    }
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        // check if a candidate still into the race has a total vote = min, then this candidate should be eliminated
        if (candidates[i].eliminated == false && candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}
