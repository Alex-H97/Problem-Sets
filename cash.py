"""Cash homework problem"""
from cs50 import get_float


def main():
    """Use a greedy algorithm to obtain the fewest number of coins required for change specified."""

    # Obtain change amount from user
    change = get_float("Change owed: ")

    # Ensure input is positive
    while change < 0:
        change = get_float("Change owed: ")

    # Convert change to cent and round to 2 d.p.
    change *= 100
    change = round(change)

    # Initiate variable for number of coins and list of coin values
    coins = 0
    values = [25, 10, 5, 1]

    # Iterate over coin values
    for coin in values:

        # Determine number of coins used and change left for each coin
        coins += change // coin
        change = change % coin

    # Print number of coins used
    print(f"{coins}")


if __name__ == "__main__":
    # If program is run call main()
    main()