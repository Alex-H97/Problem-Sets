"""Extra to Caesar Homework"""
from cs50 import get_string


# Define global varaibles
LENALPHA = 26


def main(argv):
    """Decrypt ciphertext message with key given using Ceasar's cipher."""

    # Ensure correct number of arguements
    if len(argv) != 2:

        # Raise program usage error
        raise Exception("Usage: python3 decrypt_c.py key")

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

    # Retrieve cipher text
    cipher = get_string("ciphertext: ")

    # Initiate plaintext varaible
    plain = ""
    kIndex = 0

    # Iterate over characters in plain
    for char in cipher:

        # Ensure key index is within range
        kIndex %= len(key)

        # Check if char is a capital
        if char.isupper():

            # Convert character to alphabetical index
            aIndex = ord(char) - ord('A')

            # Decrypt Caesar's Cipher
            aIndex = remainder(aIndex - key[kIndex], LENALPHA) + LENALPHA
            aIndex %= LENALPHA

            # Convert alphabetical index back to character
            char = chr(aIndex + ord('A'))

            # Increment key index by one
            kIndex += 1

        # Check if char is a capital
        elif char.islower():

            # Convert character to alphabetical index
            aIndex = ord(char) - ord('a')

            # Decrypt Caesar's Cipher
            aIndex = remainder(aIndex - key[kIndex], LENALPHA) + LENALPHA
            aIndex %= LENALPHA

            # Convert alphabetical index back to character
            char = chr(aIndex + ord('a'))

            # Increment key index by one
            kIndex += 1

        # Add char to ciphertext
        plain += char

    # Print plaintext to user
    print("plaintext: " + plain)


def remainder(a, b):
    """Gives the remainder for both positive and negative numerators, \\
    a, divide by the denominator, b, i.e. remainder of a / b."""

    # Find number of times b goes into a
    c = int(a / b)

    # Find size of this
    c *= b

    # Subtract from a to find remainder
    r = a - c

    # Return if successful
    return r


if __name__ == "__main__":
    # If program run get programs arguements
    from sys import argv

    # Call main with argv
    main(argv)
