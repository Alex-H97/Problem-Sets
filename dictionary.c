// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"
#include "trie.h"

// Define root node
node* ROOT;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Find item in trie
    return find_item(word, ROOT);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initiate memory for root node
    ROOT = initiate_node();

    // Open dictionary for reading
    FILE* dictFILE = fopen(dictionary, "r");

    // Check successful otherwise return flase
    if (dictFILE == NULL)
    {
        return false;
    }

    // Initiate varaibles for loop
    char word[LENGTH];

    // Scan dictionary word by word until End Of File or error
    while (fscanf(dictFILE, "%s", word) != EOF)
    {
        // Add word to trie
        if (!add_item(word, ROOT))
        {
            // If couldn't add word unload memory and return false
            fclose(dictFILE);
            unload();
            return false;
        }
    }

    // Close dictionary and exit with no error
    fclose(dictFILE);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // Find items below root node
    return find_size(ROOT);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Remove root node
    return remove_node(ROOT);
}
