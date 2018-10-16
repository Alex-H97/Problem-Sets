from enum import Enum


class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())


def distances(a, b):
    """Calculate edit distance from a to b"""

    # Create matrix to hold cost and most recent operation. The matrix is (1 + len(a) by 1 + len(b)),
    # so the top row represents no change to string 'a', and the first column represents no change for
    # string 'b'. The top left cell (always 0) reprsents how we go from string 'a' to string 'a', and
    # the bottom right cell represents how we go from string 'a' to string 'b'. Each cell holds a
    # tuple '(cost, operation)', where the integer 'cost' is the number of operations required to reach
    # this cell, and the enumeration 'operation' is the most recent operation [delete, insert, substitute]
    # used to reach this cell. A delete means the character in string 'a' which the row represents is
    # deleted. An insert means the character in string 'b' which the column represents has been inserted.
    # A substitute means the character in string 'a' which the row represents has be swapped for the
    # character in string 'b' which the column represents. A delete, insert and substitute all cost 1,
    # apart from a substitute of the same character which costs 0.

    # Initiate matrix and first row
    matrix = [[(0, 0), ] + [(i, Operation.INSERTED) for i in range(1, 1 + len(b))]]

    # Iterate over rows
    for aIndex in range(1, 1 + len(a)):

        row = [(aIndex, Operation.DELETED), ]

        # Iterate over columns
        for bIndex in range(1, 1 + len(b)):

            # List to hold all possible operations
            operations = []

            # Find deletion cost
            cellCost = matrix[aIndex - 1][bIndex][0]          # Find cost of previous cell
            cellCost += 1                                     # Add cost of operation
            operations.append((cellCost, Operation.DELETED))  # Append cost and operation type

            # Find insertion cost
            cellCost = row[bIndex - 1][0]                          # Find cost of previous cell
            cellCost += 1                                      # Add cost of operation
            operations.append((cellCost, Operation.INSERTED))  # Append cost and operation type

            # Find Substitution cost
            cellCost = matrix[aIndex - 1][bIndex - 1][0]              # Find cost of previous cell
            cellCost += 0 if (a[aIndex - 1] == b[bIndex - 1]) else 1  # Add cost of operation
            operations.append((cellCost, Operation.SUBSTITUTED))      # Append cost and operation type

            # Append smallest cost to row
            row.append(min(operations, key=lambda op: op[0]))

        # Append row to matrix
        matrix.append(row)

    return matrix
