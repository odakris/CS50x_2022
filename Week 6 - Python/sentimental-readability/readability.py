from cs50 import get_string


def main():

    # Prompt user for text to analyse
    text = get_string("Text : ")
    # Count letters
    letters = count_letters(text)
    # Count words
    words = count_words(text)
    # Count sentences
    sentences = count_sentences(text)

    # Average number of letters per 100 words in the text
    L = (letters/words)*100
    # Average number of sentences per 100 words in the text
    S = (sentences/words)*100

    # Coleman-Liau index calculation
    index = 0.0588 * L - 0.296 * S - 15.8

    # Printing results
    if index < 1:
        print("Before Grade 1")
    elif index > 16:
        print("Grade 16+")
    else:
        print(f"Grade {int(round(index))}")


def count_letters(text):
    """ count letters in text """
    # Counting every character in text and substracting every other characters that are not letters to count only letters
    letters = len(text) - text.count(" ") - text.count(".") - text.count("?")
    letters = letters - text.count("!") - text.count("'") - text.count('"') - text.count(",")
    return letters


def count_words(text):
    """ count words in text """
    # Count every space (as words are separated by space) in text and adding +1 for last word of text
    words = text.count(" ") + 1
    return words


def count_sentences(text):
    """ text sentences in text """
    # Count dots / ! & ? in text to count sentences
    sentences = text.count(".") + text.count("?") + text.count("!")
    return sentences


if __name__ == "__main__":
    main()
