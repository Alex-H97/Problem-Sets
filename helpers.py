from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""

    # Split string into unique lines, removing leading and trailing whitespace
    aLines = {line.strip() for line in a.split("\n")}
    bLines = {line.strip() for line in b.split("\n")}

    # Return lines in both a and b
    return list(aLines & bLines)


def sentences(a, b):
    """Return sentences in both a and b"""

    # Split string into sentances, removing leading and trailing whitespace
    aSent = set(sent_tokenize(a.strip()))
    bSent = set(sent_tokenize(b.strip()))

    # Return sentances in both a and b
    return list(aSent & bSent)


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    # Obtain unique substrings of length n
    aSubs = {a[i:i+n] for i in range(len(a)-n + 1)}
    bSubs = {b[i:i+n] for i in range(len(b)-n + 1)}

    # Return substrings in both a and b
    return list(aSubs & bSubs)