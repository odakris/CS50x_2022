#include <cs50.h>
#include <stdio.h>

long card_number(void);
int number_of_digit(long card_nb);
int checksum(int count_digit, long card_nb);
void validation(int sum_total, int count_digit, long card_nb);

int main(void)
{
    // prompt user for card number
    long card_nb = card_number();

    // Calculate the number of digit
    int count_digit = number_of_digit(card_nb);
    //printf("count_digit: %i\n", count_digit);

    // LOOP to verify validity of card number
    int sum_total = checksum(count_digit, card_nb);
    //printf("sum_total: %i\n", sum_total);

    // Validation        // identification du type de carte (AMEX, VISA ou MASTERCARD)
    validation(sum_total, count_digit, card_nb);
}

long card_number(void)
{
    long card_nb = get_long("Credit Card Number : "); // Ask for CB number
    return card_nb;
}

int number_of_digit(long card_nb)
{
    int count_digit = 0;
    do
    {
        // %10 return last digit from CB number input
        int digit = card_nb % 10;
        card_nb = card_nb / 10;
        count_digit = count_digit + 1;
    }
    while (card_nb > 0);
    return count_digit; // return how many digit there is in number provide by user
}

int checksum(int count_digit, long card_nb)
{
    int sum10 = 0;
    int digit10 = 0;
    int digitX2 = 0;
    int digit_sum = 0;
    int digit = 0;

    for (int i = 1; i <= count_digit; i++)
    {
        if (i % 2 == 0) // EVEN (from one before last digit) // sum of digit * 2
        {
            digit = card_nb % 10;

            // if digit*2 >= 10 // example : digit*2 = 12 we have to to 1 + 2
            if (digit * 2 >= 10)
            {
                do
                {
                    digit10 = digit * 2 % 10;
                    digit = digit / 10;
                    sum10 = digit10 + 1;
                }
                while (digit > 0);
                digitX2 = digitX2 + sum10;
            }
            else
            {
                digitX2 = digitX2 + 2 * digit;
            }
        }
        else // ODD (from last digit) sum of digit
        {
            digit = card_nb % 10;
            digit_sum = digit_sum + digit;
        }
        card_nb = card_nb / 10;
    }
    int sum_total = digitX2 + digit_sum;
    return sum_total;
}

void validation(int sum_total, int count_digit, long card_nb)
{
    if (sum_total % 10 == 0)
    {
        int digit = 0;
        int digit1 = 0;

        if (count_digit == 13 || count_digit == 15 || count_digit == 16)
        {
            // checking for American Express
            if (count_digit == 15)
            {
                do
                {
                    digit = card_nb % 10;

                    if (card_nb <= 99 && card_nb >= 10)
                    {
                        digit1 = digit1 + digit;
                    }
                    if (card_nb <= 9)
                    {
                        digit1 = digit1 + digit * 10;
                    }
                    card_nb = card_nb / 10;
                }
                while (card_nb > 0);

                if (digit1 == 34 || digit1 == 37)
                {
                    printf("AMEX\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }

            // checking for MasterCard
            if (count_digit == 16)
            {
                do
                {
                    digit = card_nb % 10;

                    if (card_nb <= 99 && card_nb >= 10)
                    {
                        digit1 = digit1 + digit;
                    }
                    if (card_nb <= 9)
                    {
                        digit1 = digit1 + digit * 10;
                    }
                    card_nb = card_nb / 10;
                }
                while (card_nb > 0);

                if (digit1 == 51 || digit1 == 52 || digit1 == 53 || digit1 == 54 || digit1 == 55 || digit == 4)
                {
                    if (digit1 == 51 || digit1 == 52 || digit1 == 53 || digit1 == 54 || digit1 == 55)
                    {
                        printf("MASTERCARD\n");
                    }
                    if (digit == 4)
                    {
                        printf("VISA\n");
                    }
                }
                else
                {
                    printf("INVALID\n");
                }
            }

            // checking for VISA
            if (count_digit == 13)
            {
                do
                {
                    digit = card_nb % 10;
                    card_nb = card_nb / 10;
                }
                while (card_nb > 0);

                if (digit == 4)
                {
                    printf("VISA\n");
                }
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
