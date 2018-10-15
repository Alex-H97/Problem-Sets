"""Vigenère Homework Problem"""
from cs50 import get_string


# Define global varaibles
LENALPHA = 26


def main(argv):
    """Encrypt plaintext message with key given using Vigenère's cipher."""

    # Ensure correct number of arguements
    if len(argv) != 2:

        # Raise program usage error
        raise Exception("Usage: python3 vigenere.py key")

    # Ensure valid key
    if not argv[1].isalpha():

        # Raise key usage error
        raise Exception("Usage - key: must be a string of alphabetical characters")

    # Initiate list to hold alphabetical indexes
    key = []

    # Iterate over characters in specified key in lowercase
    for char in argv[1].lower():

        # Convert character to alphabetical index
        aIndex = ord(char) - ord('a')

        # Append to key list
        key.append(aIndex)

    # Retrieve plain text
    plain = get_string("plaintext: ")

    # Initiate ciphertext and key index varaibles
    cipher = ""
    kIndex = 0

    # Iterate over characters in plain
    for char in plain:

        # Ensure key index is within range
        kIndex %= len(key)

        # Check if char is a capital
        if char.isupper():

            # Convert character to alphabetical index
            aIndex = ord(char) - ord('A')

            # Apply Vigenere's Cipher
            aIndex = (aIndex + key[kIndex]) % LENALPHA

            # Convert alphabetical index back to character
            char = chr(aIndex + ord('A'))

            # Increment key index by one
            kIndex += 1

        # Check if char is a capital
        elif char.islower():

            # Convert character to alphabetical index
            aIndex = ord(char) - ord('a')

            # Apply Caesar's Cipher
            aIndex = (aIndex + key[kIndex]) % LENALPHA

            # Convert alphabetical index back to character
            char = chr(aIndex + ord('a'))

            # Increment key index by one
            kIndex += 1

        # Add char to ciphertext
        cipher += char

    # Print ciphertext to user
    print("ciphertext: " + cipher)


if __name__ == "__main__":
    # If program run get programs arguements
    from sys import argv

    # Call main with argv
    main(argv)
