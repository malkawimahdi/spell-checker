
// Implements a dictionary's functionality
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//Number of buckets in hash table
//Faster to store in 64-bit rather than 32-bit
const uint64_t N = 999983;

// Hash table
node *table[N];

uint32_t word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    uint32_t index = hash(word);

    for (node *iterator = table[index]; iterator != NULL; iterator = iterator->next)
    {
        if ((strcasecmp(word, iterator->word) == 0))
        {
            return true;
        }
    }
    return false;
}

// Hashes string to a number
// Source: djib2 by Dan Bernstein (http://www.cse.yorku.ca/~oz/hash.html)
uint64_t hash(const char *word)
{
    unsigned long hash = 5381;
    int c = *word;
    c = tolower(c);

    while (*word != 0)
    {
        hash = ((hash << 5) + hash) + c;
        c = *++word;
        c = tolower(c);
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *input = fopen(dictionary, "r");
    if (input == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];

    while (fscanf(input, "%s", word) != EOF)
    {
        node *new_node = malloc(sizeof(node));

        if (new_node == NULL)
        {
            free(input);
            return false;
        }
        strcpy(new_node->word, word);

        new_node->next = NULL;

        unsigned int index = hash(word);

        if (table[index] == NULL)
        {
            table[index] = new_node;
        }
        else
        {
            new_node->next = table[index];
            table[index] = new_node;
        }
        ++word_count;
    }
    fclose(input);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
uint32_t size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (unsigned int counter = 0; counter < N; ++counter)
    {
        if (table[counter] == NULL)
        {
            continue;
        }
        else
        {
            node *iterator = table[counter];
            while (iterator != NULL)
            {
                node *temporary = iterator->next;
                free(iterator);
                iterator = temporary;
            }
        }
    }
    return true;
}