"""Credit Homework Problem"""
from cs50 import get_int


def main():
    """Determine card type and validty of card using Luhn's algorithm."""

    # Obtain users card number
    cNum = get_int("Number: ")

    # Ensure users input is valid card number
    while cNum < 0:
        cNum = get_int("Number: ")

    # Initiate varaible to hold sum and convert card number to a string
    total = 0
    cNum = str(cNum)

    # Iterate over every other digit starting from the penultimate
    for dLoc in range(len(cNum) - 1, 0, -2):

        # Retrive digit from card number
        cDigit = int(cNum[dLoc - 1])

        # Multiply card's digit by 2 and convert to string
        product = str(cDigit * 2)

        # Iterate over each digit of product
        for pDigit in product:

            # Add product's digit to total
            total += int(pDigit)

    # Iterate over every other digit starting from the last
    for dLoc in range(len(cNum), 0, -2):

        # Retrive digit from card number
        digit = int(cNum[dLoc - 1])

        # Add digit total
        total += digit

    # Initive varaible to store validity of card
    valid = False

    # Check card number is valid
    if (total % 10) == 0:

        # Initiate dictionaries to hold valid card types
        cNumLens = {13: "V", 15: "A", 16: "M/V"}
        cTypes = {'A': ["34", "37"],
                  'M': ["51", "52", "53", "54", "55"],
                  'V': ["4"]}

        # Determine card type from card number's length
        cType = cNumLens[len(cNum)]

        # Check if possible to be American Express
        if 'A' in cType:

            # Check if valid American Express
            if cNum[:2] in cTypes['A']:

                # Set validty to true and print card type
                valid = True
                print("AMEX")

        # Check if possible to be Mastercard
        if 'M' in cType:

            # Check if valid Mastercard
            if cNum[:2] in cTypes['M']:

                # Set validty to true and print card type
                valid = True
                print("MASTERCARD")

        # Check if possible to be Visa
        if 'V' in cType:

            # Check if valid Visa
            if cNum[:1] in cTypes['V']:

                # Set validty to true and print card type
                valid = True
                print("VISA")

    # Check if card is not valid
    if not valid:

        # Print card is invalid
        print("INVALID")

    # Return 0 if successful
    return 0


if __name__ == "__main__":
    # If program is run call main
    main()