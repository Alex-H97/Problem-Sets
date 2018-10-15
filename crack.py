"""Crack Homework Problem"""
from crypt import crypt


# Define classes
class Tuple:
    def __init__(self, char, infront):
        self.char = char  # Next character
        self.infront = infront  # Move pointer infront


# Define global variables
LENPASS = 5     # Password max length
CAPSTART = 26   # Start of capitals in CharFreq

# List of most frequently used password characters to quicken search, split into lower and upper case,
# found [28/07/2018] at: https://csgillespie.wordpress.com/2011/06/16/character-occurrence-in-passwords/
CharFreq = ['e', 'a', 'o', 'r', 'i', 'n', 's', 'l', 't',
            'm', 'd', 'c', 'y', 'b', 'h', 'g', 'u', 'p',
            'k', 'f', 'j', 'w', 'v', 'z', 'x', 'q',
            'S', 'E', 'A', 'R', 'M', 'T', 'B', 'N', 'O',
            'L', 'I', 'C', 'D', 'P', 'H', 'J', 'K', 'G',
            'Y', 'U', 'F', 'W', 'V', 'X', 'Z', 'Q']


def main(argv):
    """Decrypts a password encrypted with C's DES-based "crypt" funtion, using brute force."""

    # Ensure program used correctly
    if len(argv) != 2:

        # Raise program usage error
        raise Exception("Usage: python3 crack.py hash")

    # Obtain the salt
    salt = argv[1][:2]

    # Initialise varaibles for loop
    password = [CharFreq[CAPSTART], ]    # Password guesses
    pointer = 0     # Password character pointer

    # Check initial password guess
    cracked = True if (argv[1] == crypt(''.join(password), salt)) else False

    # Loop over all possible combinations until cracked or too long
    while (not cracked) and (len(password) < LENPASS + 1):

        # Cycle next character at pointer
        nextv = next_char(password[pointer], pointer)

        # Change all characters infront of pointer which need changing
        while nextv.infront:

            # Check if pointer is at the first letter of password
            if pointer == 0:

                # Add new capital character to start of password
                password = list(CharFreq[CAPSTART] + ''.join(password))

                # Set pointer to last character in password
                pointer = len(password) - 1

                # Change next character to first lower character and
                # stop changing characters infront of pointer
                nextv.char = CharFreq[0]
                nextv.infront = False

            # If pointer not at first letter of password
            else:

                # Assign new character to password at pointer
                password[pointer] = nextv.char

                # Move pointer infront
                pointer -= 1

                # Cycle next character at pointer
                nextv = next_char(password[pointer], pointer)

        # Assign new character to password at pointer
        password[pointer] = nextv.char

        # Move the character pointer back to last character in password
        pointer = len(password) - 1

        # Check if hashes match break while loop
        if crypt(''.join(password), salt) == argv[1]:
            cracked = True

    # Print password to screen
    print(''.join(password))


# Function Definitions
def next_char(char, pos):
    """Given a character from the guess password and this character's \\
    position within the password. This function determines the next \\
    character to be used and whether the character infront should also \\
    be changed.

    This is returned in the form of a Tuple:
        - Tuple.char: is the next character.
        - Tuple.infront: a boolean to change the character infront.f"""

    # Iterate over indices of CharFreq
    for index in range(len(CharFreq)):

        # If character at that indice break loop
        if char == CharFreq[index]:
            break

    # Initiate varaible to store if character infront needs changing
    infront = False

    # Check if index is at end of CharFreq array
    if (index == len(CharFreq) - 1):

        # Check if chararcter is not the initial character in password
        if pos != 0:

            # Change character infront
            infront = True

        # Cycle next character to start of list
        index = 0

    # If index is not at end of CharFreq array
    else:

        # Check if character is the initial character in password and
        # if the next index is where the capitals start in CharFreq
        if (pos == 0) and (index == CAPSTART - 1):

            # Change character infront
            infront = True

        # Increment to following index
        index += 1

    # Return Tuple holding next character and whether character infront
    # should be changed too
    return Tuple(CharFreq[index], infront)


if __name__ == "__main__":

    # Retrieve users input
    from sys import argv

    # If program is ran call main
    main(argv)