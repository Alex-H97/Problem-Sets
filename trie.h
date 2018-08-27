// Declares a trie's functionality

#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>
#include <stdlib.h>

#define CHILDNODES 27

// GLOBAL VARIABLE DEFINITIONS
// Define a trie node
typedef struct node
{
    // Boolean to determine end of item
    bool endItem;

    // Array of node pointers to other letters and apostrophe
    struct node* next[CHILDNODES];
} node;


// FUNCTION DECLARATIONS
node* initiate_node(void);
bool add_item(char* item, node* rootNode);
bool find_item(const char* item, node* rootNode);
unsigned int find_size(node* parentNode);
bool remove_node(node* rmNode);

#endif // TRIE_H