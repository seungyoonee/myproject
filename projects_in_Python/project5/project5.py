'''
In the game Hangman, the computer selects a random word that the player must guess.
Instead of guessing the entire word at once, the player guesses individual letters.
After each guess, the computer reveals where in the word the guessed letters appear.
The computer counts how often the player guesses a letter that does not appear in the word.
If the player gets more than 5 guesses wrong, the player loses.
If the player uncovers all letters in the word, she wins.

To guess the word, your program should read in the file dictionary.txt 
Download dictionary.txt and store the words in a list.
Then it should randomly select one word from the list.
'''
import random

def read_dictionary(filename):
    dictionary_file = open(filename, 'r')
    
    # creating dicctionary list to store words
    dict_li = []
    for line in dictionary_file:
        line = line.strip()
        dict_li.append(line)
        print(line)
    
    return dict_li
    
def main():
    # reading the file dictionary.txt
    filename = "dictionary.txt"
    dictionary = read_dictionary(filename)
    
    # randomly selecing one word from the dictionary
    rand_index = random.randint(0, len(dictionary) - 1)
    rand_word = dictionary[rand_index]
        
    # for printing out the word with letters uncovered
    guessed_li = []
    for i in range(len(rand_word)):
        guessed_li.append("_")
    
    print("Welcome to Hangman!") # welcoming prompt
    
    # main game loop with maximum 5 guesses
    wrong_guess = 0
    while wrong_guess != 5:
        right_guess = False
        letter = str(input("\nGuess a letter: "))
        # cheick if the letter appears in the word
        for i in range(len(rand_word)):
            if rand_word[i] == letter.lower(): # lower case
                guessed_li[i] = letter.lower()
                right_guess = True
            elif rand_word[i] == letter.upper(): # upper case
                guessed_li[i] = letter.upper()
                right_guess = True
                
        if right_guess:
            # print if the user guesses a correct letter
            for j in range(len(guessed_li)):
                print(guessed_li[j], " ", end="")
        else:
            # if the guess is wrong
            print("Wrong guess!")
            wrong_guess += 1
        # check if the user got the word before 5 wrong guesses
        if "_" not in guessed_li:
            break
    
    # combining 
    final_guess = ""
    for i in guessed_li:
        final_guess += i

    # check if the user guessed the word right
    if final_guess == rand_word:
        print("\nYou have won!\nThe word is:", rand_word)
    else:
        print("\nYou have lost..\nThe word is:", rand_word)
    
if __name__ == "__main__":
    main()

