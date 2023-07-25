from cs50 import get_int
from cs50 import get_string


def main():
    card_nb = card_number()
    #print(f"CB : {card_nb}")

    count_digit = number_of_digit(card_nb)
    #print(f"count : {count_digit}")

    sum_total = checksum(count_digit, card_nb)
    #print(f"sum : {sum_total}")

    validation(sum_total, count_digit, card_nb)


def card_number():
    """ Prompt user for card number """
    card_nb = get_int("Credit Card Number : ")
    return card_nb


def number_of_digit(card_nb):
    """ count how many digit there is in user card number """
    # Initialize count_digit
    count_digit = 0

    while True:
        # running loop until card_nb = 0
        if card_nb == 0:
            break
        else:
            # // ==> to get an int (getting rid of numbers after comma) and not a float
            card_nb = card_nb // 10
            count_digit = count_digit + 1
    # Return how many digit there is in card number provide by user
    return count_digit


def checksum(count_digit, card_nb):

    sum10 = 0
    digit10 = 0
    digitX2 = 0
    digit_sum = 0
    digit = 0

    """ sum digit from card number """
    for i in range(1, count_digit+1):
        # EVEN
        # multiply each digits by 2 (starting with the number’s second-to-last digit)
        if i % 2 == 0:
            # %10 return last digit of card_nb
            digit = card_nb % 10

            # example : if digit * 2 = 12 we have to sum 1 + 2
            if digit * 2 >= 10:
                while True:
                    # running loop until digit = 0
                    if digit == 0:
                        break
                    else:
                        digit10 = digit * 2 % 10
                        digit = digit // 10
                        # + 1 because digit * 9 can't be greater than 18 so 18 %10 = 8
                        sum10 = digit10 + 1

                digitX2 = digitX2 + sum10

            else:
                digitX2 = digitX2 + 2 * digit
        # ODD
        # sum of the digits that weren’t multiplied by 2 (starting from the end)
        else:
            digit = card_nb % 10
            digit_sum = digit_sum + digit

        card_nb = card_nb // 10

    sum_total = digitX2 + digit_sum
    return sum_total


def validation(sum_total, count_digit, card_nb):
    """ check for valid card number (Mastercard/VISA/American Express/INVALID) """
    # Checking if card number is valid
    if sum_total % 10 == 0:

        digit = 0
        digit1 = 0

        if count_digit == 13 or count_digit == 15 or count_digit == 16:
            # CHECKING FOR AMERICAN EXPRESS
            if count_digit == 15:
                while True:
                    if card_nb == 0:
                        break
                    else:
                        digit = card_nb % 10

                        if card_nb <= 99 and card_nb >= 10:
                            digit1 = digit1 + digit
                        if card_nb <= 9:
                            digit1 = digit1 + digit * 10

                        card_nb = card_nb // 10

                if digit1 == 34 or digit1 == 37:
                    print("AMEX")
                else:
                    print("INVALID")

            # CHECKING FOR MASTERCARD
            if count_digit == 16:

                while True:
                    if card_nb == 0:
                        break
                    else:
                        digit = card_nb % 10

                        if card_nb <= 99 and card_nb >= 10:
                            digit1 = digit1 + digit
                        if card_nb <= 9:
                            digit1 = digit1 + digit * 10
                        card_nb = card_nb // 10

                #print(f"digit1: {digit1}")
                if digit1 == 51 or digit1 == 52 or digit1 == 53 or digit1 == 54 or digit1 == 55 or digit == 4:

                    if digit1 == 51 or digit1 == 52 or digit1 == 53 or digit1 == 54 or digit1 == 55:
                        print("MASTERCARD")

                    if digit == 4:
                        print("VISA")
                else:
                    print("INVALID")

            # CHECKING FOR VISA
            if count_digit == 13:

                while True:
                    if card_nb == 0:
                        break
                    else:
                        digit = card_nb % 10
                        card_nb = card_nb // 10

                if digit == 4:
                    print("VISA")
                else:
                    printf("INVALID")
        else:
            print("INVALID")

    else:
        print("INVALID")


if __name__ == "__main__":
    main()
