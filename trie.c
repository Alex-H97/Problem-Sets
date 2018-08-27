// Implements a trie's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"


// FUNCTION DEFINITIONS

// Initiate a new node for trie, returns pointer to node
node* initiate_node(void)
{
    // Initiate memory for root node
    node* newNode = malloc(sizeof(node));

    // Check successful otherwise return flase
    if (newNode == NULL)
    {
        return NULL;
    }

    // Remove junk memroy allocations
    newNode->endItem = false;

    // Iterate over next array
    for (int i = 0 ; i < 27 ; i++)
    {
        // Remove junk memroy allocations
        newNode->next[i] = NULL;
    }

    return newNode;
}

// Adds new item to the trie at given node, returns false if any errors occur
bool add_item(char* item, node* parentNode)
{
    // Create pointer to keep track of location in node
    node* nodePtr = parentNode;

    // Iterate over characters in item
    for (int i = 0; i < strlen(item); i++)
    {
        // Determine next node index
        int nextIndex = (item[i] == '\'') ? 26 : item[i] - 'a';

        // Check if next node is blocked
        if (nodePtr->next[nextIndex] == NULL)
        {
            // Create new node
            node* newNode = initiate_node();

            //Return false if newNode not allocated
            if (newNode == NULL)
            {
                return false;
            }

            // Unblock next node
            nodePtr->next[nextIndex] = newNode;
        }

        // Move node pointer along
        nodePtr = nodePtr->next[nextIndex];
    }

    // Set item end
    nodePtr->endItem = true;

    // Return with no error
    return true;
}

// Find item in trie below given node, return false if not found
bool find_item(const char* item, node* parentNode)
{
    // Create pointer to keep track of location in node
    node* nodePtr = parentNode;

    // Iterate over characters in item
    for (int i = 0 ; i < strlen(item) ; i++)
    {
        // Determine node index
        int nextIndex = (item[i] == '\'') ? 26 : tolower(item[i]) - 'a';

        // Check node isn't blocked
        if (nodePtr->next[nextIndex] == NULL)
        {
            // Item not found
            return false;
        }

        // Move node pointer along
        nodePtr = nodePtr->next[nextIndex];
    }

    // Return if item present
    return nodePtr->endItem;
}

// Find the number of items below a given node
unsigned int find_size(node* parentNode)
{
    // Define varaible to hold number of items
    unsigned int items = (parentNode->endItem == true) ? 1 : 0;

    for (int i = 0 ; i < CHILDNODES ; i++)
    {
        // Check if child branch is blocked
        if (parentNode->next[i] != NULL)
        {
            // If not add number of items in child branch
            items += find_size(parentNode->next[i]);
        }
    }

    // Return number of items
    return items;
}

// Remove the given node from memory, return false if any errors occur
bool remove_node(node* rmNode)
{
    // Iterate over all child branches
    for (int i = 0 ; i < CHILDNODES ; i++)
    {
        // Check if branch is blocked
        if (rmNode->next[i] != NULL)
        {
            // If not blocked remove node
            if  (!remove_node(rmNode->next[i]))
            {
                // If couldn' remove node return false
                return false;
            }
        }
    }

    // Once all brances are blocked remove node
    free(rmNode);

    // If no errors return true
    return true;
}