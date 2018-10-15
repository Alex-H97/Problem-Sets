"""Mario less experinced"""
from cs50 import get_int


def main():
    """Print some steps with a height specified by the user."""

    # Ask user for input
    while True:
        height = get_int("Height: ")

        # If height is a positive integer, 23 or less, break out of loop
        if (height >= 0) and (height <= 23):
            break

    # Intiate variables
    blocks = 2
    spaces = height - 1

    # Iterate over number of rows
    for row in range(height):

        # Print spaces
        print(' ' * spaces, end='')

        # Print blocks
        print('#' * blocks)

        # Increment varaibles
        blocks += 1
        spaces -= 1



# Implement main function
if __name__ == "__main__":
    main()