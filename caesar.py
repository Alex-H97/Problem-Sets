"""Caesar Homework Problem"""
from cs50 import get_string


# Define global varaibles
LENALPHA = 26


def main(argv):
    """Encrypt plaintext message with key given using Ceasar's cipher."""

    # Ensure correct number of arguements
    if len(argv) != 2:

        # Raise program usage error
        raise Exception("Usage: python3 caesar.py key")

    # Ensure valid key
    try:
        # Retrieve key from input
        key = int(argv[1])

        # Raise Error if negative
        if key < 0:
            raise ValueError

    # If not an integer or negative
    except ValueError:

        # Raise key usage error
        raise Exception("Usage - key: must be a positive integer") from None

    # Retrieve key from input
    key = int(argv[1])

    # Retrieve plain text
    plain = get_string("plaintext: ")

    # Initiate ciphertext varaible
    cipher = ""

    # Iterate over characters in plain
    for char in plain:

        # Check if char is a capital
        if char.isupper():

            # Convert character to alphabetical index
            aIndex = ord(char) - ord('A')

            # Apply Caesar's Cipher
            aIndex = (aIndex + key) % LENALPHA

            # Convert alphabetical index back to character
            char = chr(aIndex + ord('A'))

        # Check if char is a capital
        elif char.islower():

            # Convert character to alphabetical index
            aIndex = ord(char) - ord('a')

            # Apply Caesar's Cipher
            aIndex = (aIndex + key) % LENALPHA

            # Convert alphabetical index back to character
            char = chr(aIndex + ord('a'))

        # Add char to ciphertext
        cipher += char

    # Print ciphertext to user
    print("ciphertext: " + cipher)


if __name__ == "__main__":
    # If program run get programs arguements
    from sys import argv

    # Call main with argv
    main(argv)
