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

    # Retrieve cipher text
    cipher = get_string("ciphertext: ")

    # Initiate plaintext varaible
    plain = ""

    # Iterate over characters in plain
    for char in cipher:

        # Check if char is a capital
        if char.isupper():
            print("Upper")

            # Convert character to alphabetical index
            aIndex = ord(char) - ord('A')

            # Decrypt Caesar's Cipher
            aIndex = remainder(aIndex - key, LENALPHA) + LENALPHA
            aIndex %= LENALPHA

            # Convert alphabetical index back to character
            char = chr(aIndex + ord('A'))

        # Check if char is a capital
        elif char.islower():

            # Convert character to alphabetical index
            aIndex = ord(char) - ord('a')

            # Decrypt Caesar's Cipher
            aIndex = remainder(aIndex - key, LENALPHA) + LENALPHA
            aIndex %= LENALPHA

            # Convert alphabetical index back to character
            char = chr(aIndex + ord('a'))

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
