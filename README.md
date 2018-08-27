# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

The longest word in the dictionary, any longer word is not real. This is used to create an array to hold each word.

## According to its man page, what does `getrusage` do?

It returns the pointer to a structure called `struct rusage`, which contains the measured resource usages (such as CPU time and memory), for either of: `RUSAGE_SELF` (all threads in the process), `RUSAGE_CHILDREN`, `RUSAGE_THREAD`.

## Per that same man page, how many members are in a variable of type `struct rusage`?

The structure `struct rusage` contains 16 variables, of which 2 are the structures `struct timeval` and 14 are `long` integers.

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

In case nothing was returned by `getrusage` (possibly due to an error), in which case we can prevent our program crashing by checking the pointer isn't `NULL`.

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file. In other words, convince us that you indeed understand how that function's `for` loop works.

First, we initiate some integer variables `index`, `misspellings`, `words` and set them to `0`, for use in the loop. We also initiate a string (`char` array) with the length of the longest word (plus one), this saves memory as we don't require a linked list. The extra element in the length of the array is to add the `NUL` element if we have the longest possible word.
The `for` loop iterates by obtaining the next character in the text file, until it receives the special `EOF` character (which indicates the 'End Of File' or an error).
Inside the loop, we first check the character is a letter (a-Z) or an apostrophe. If it is we add the character to the `word` array at the position given by `index`, and increment the `index` by one. We check whether that the `index` is greater than the `LENGTH` of the longest word. If true it cannot be a real word, so skip over the remainder of the word using a `while` loop, then reset the `index` to `0` for the next word.
If the character is not a letter we check if it is a digit (0-9). If true it cannot be a word so we skip over the rest of the alphanumeric word using a `while` loop, then reset `index` to `0` for the next word.
If the character is neither a letter nor a digit it may be a word, so we check that the `index` is greater than `0`. If true the string has at least one character so must be a word, so we then terminate the string by appending the `NUL` character at the end. We then increment the `words` counter by `1`. Next, we get the `before` usage resources, check the spelling through the function `check`, before obtaining the `after` usage resources. Using these resources we calculate the time taken to check the spelling of the word with `calculate`, then add this to the total time taken to spell check stored in `time_check`. Next, we check if the word was misspelt, if true we `printf` the word to the screen and increment the `misspellings` counter by `1`. Finally, we reset the `index` counter to `0` for the next word.
The `for` loop then iterates to the next character in the text file. Or if there was an error loading the character, or it was the `EOF` character, the loop breaks.
After the loop has broken, we check if there was an error with the file (hence why the loop stopped). If true we inform the user of the error and `unload` the dictionary before exiting the program with the error `1`. If not true the program continues and closes the text file.

## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?

Since this can make it easier to check the word is valid (i.e. checking for digits), but mainly to avoid any errors thrown by the alternative function `fscanf`. For example is the string is longer than the size of the array `word` it would cause a memory error.

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?

Since we do not want the function to edit/change the `word` string in `check`, or the `dictionary` pointer in `load`.This is because we do not want to lose the location of the `word` or `dictionary`, as this could cause problems such as memory leaks.
