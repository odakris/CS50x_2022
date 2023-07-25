#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Max number of candidates
#define MAX 9

// election[i] are links between candidates
int election[MAX];

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0) // check if name entered is in the list
        {
            ranks[rank] = i; // put candidate index in ranks[] array
            // printf("candidate name %s\n", candidates[i]);
            // printf("candidate number %i\n", i);
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // j = i+1 to move candidates from candidate until the last one
            // j is increasing as we move from candidate, each candidate is prefered over less candidate as they are less and less prefered
            // put points preference of ballot prefered candidate (ranks[i]) over candidate ranks[j]
            preferences[ranks[i]][ranks[j]] = preferences[ranks[i]][ranks[j]] + 1;
        }
    }
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     for (int j = 0; j < candidate_count; j++)
    //     {
    //         printf("%i", preferences[i][j]);
    //     }
    //     printf("\n");
    // }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    // Pair is a 2D array, one array pair.winner & one array pair.loser
    // identification of which candidate is prefered over an other candidate and store them in pair array
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // store preferences[i][j] as winner and preferences[j][i] as loser
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count = pair_count + 1; // increase when a pair of candidate (W/L) is detected
            }
            // store preferences[j][i] as winner and preferences[i][j] as loser
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count = pair_count + 1; // increase when a pair of candidate (W/L) is detected
            }
        }
    }
    // printf("pair_count: %i\n", pair_count);
    // for (int i = 0; i < pair_count; i++)
    // {
    //     printf("winner: %i / loser : %i\n", pairs[i].winner, pairs[i].loser);
    // }
    return;
}

// Sort pairs in decreasing order by strength of victory == NUMBER OF VOTE OF WINNER
void sort_pairs(void)
{
    printf("\n");
    // TODO
    // Here we want to sort by decreasing order from winner with most votes to winner with less votes
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            // checking for strongest victory between two winner to sort them decreasingly
            if ((preferences[pairs[i].winner][pairs[i].loser]) < (preferences[pairs[j].winner][pairs[j].loser]))
            {
                pair temp_pairs = pairs[i]; // temp_pairs is temporary storage to avoid erasing data
                pairs[i] = pairs[j];
                pairs[j] = temp_pairs;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;  // lock W/L pairs
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // for (int i = 0; i < pair_count; i++)
    // {
    //     for (int j = 0; j < pair_count; j++)
    //     {
    //         if (locked[i][j] == true)
    //         {
    //             election[j] = election[j] + 1;
    //         }
    //     }
    // }
    // for (int i = 0; i < pair_count; i++)
    // {
    //     if (election[i] > election[i + 1])
    //     {
    //         int temp_elec = election[i]; // temp_pairs is temporary storage to avoid erasing data
    //         election[i] = election[i + 1];
    //         election[i + 1] = temp_elec;
    //     }
    // }
    // printf("%s", candidates[election[0]]);
    return;
}
