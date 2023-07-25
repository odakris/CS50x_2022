import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # TODO: Read database file into a variable
    # creation of database variable
    # list() or [] are used for lists and take integers as indices
    # database = []
    database = list()
    # opening csv file
    with open(sys.argv[1]) as csv_file:
        # reading csv file
        reader = csv.DictReader(csv_file)
        # Loop that append data from csv file into database variable
        for row in reader:
            # Database is a list of dictionaries
            database.append(row)

    # # Print database of suspected people
    # print("Database")
    # for i in range(len(database)):
    #     print(database[i])

    # # Returns every keys of database
    # print("KEYS")
    # for key in database[0]:
    #     print(f"key :'{key}'", end = ' ')
    # print("")

    # TODO: Read DNA sequence file into a variable
    # DNA = list()
    DNA = []
    # opening txt file
    f = open(sys.argv[2], 'r')
    # read txt file into DNA variable
    DNA = f.read()
    # closing file
    f.close()
    # # Print DNA to search for
    # print(f"DNA : {DNA}")

    # TODO: Find longest match of each STR in DNA sequence
    # dict() or {} are used for dictionaries and take strings as indices. This allow us to use strings as indices in loops
    # STR_match = {}
    STR_match = dict()

    # Create a dict of longest_match for each STR
    for STR in database[0]:
        # str() turns integers into strings
        # This would be useful for comparison. We will compare strings and not integers
        STR_match[STR] = str(longest_match(DNA, STR))
    # # Print all longest STR in DNA to analyse
    # print(f"STR match: {STR_match}")

    # TODO: Check database for matching profiles
    # First loop goes throught every rows or every suspects
    for row in range(len(database)):
        # As in STR_match 'name' key is paired with '0' (because there is no 'N A M E' STR in DNA)
        # And in database 'name' key is paired with a suspect name (Alice, Bob etc...)
        # So to simulate 'name' is matching, count start from 1 by default
        count = 1
        # Second loop goes through every columns or every STRs (from 'name' to last STR)
        for STR in STR_match:
            # Checking if STR_match match with current suspect's STRs (ALL STRs)
            if STR_match[STR] == database[row][STR]:
                count = count + 1
        # Checking if count is equal to total STR searching for (small database : 3 STR // large : 8, excluding 'name')
        if count == len(STR_match):
            print(database[row]['name'])
            break
    # If all suspects don't match all STR then 'no match'
    if count != len(STR_match):
        print('No match')


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
