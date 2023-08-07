#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Allan Jean-Jacques

// Trie node structure
struct TrieNode
{
    struct TrieNode *children[26];
    int word_count;
};

// Trie structure
struct Trie
{
    struct TrieNode *root;
};

// Creates a new TrieNode
struct TrieNode *createTrieNode()
{
    struct TrieNode *newNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (newNode)
    {
        newNode->word_count = 0;
        for (int i = 0; i < 26; i++)
        {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *trie = (struct Trie *)malloc(sizeof(struct Trie));
    if (trie)
    {
        trie->root = createTrieNode();
    }
    return trie;
}

// Inserts the word into the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct TrieNode *current = pTrie->root;
    int length = strlen(word);

    for (int i = 0; i < length; i++)
    {
        int index = word[i] - 'a';
        if (current->children[index] == NULL)
        {
            current->children[index] = createTrieNode();
        }
        current = current->children[index];
    }
    
    current->word_count++;
}

// Computes the number of occurrences of the word in the trie
int numberOfOccurrences(struct Trie *pTrie, char *word)
{
    struct TrieNode *current = pTrie->root;
    int length = strlen(word);

    for (int i = 0; i < length; i++)
    {
        int index = word[i] - 'a';
        if (current->children[index] == NULL)
        {
            return 0;
        }
        current = current->children[index];
    }
    return current->word_count;
}

void deallocateTrieNode(struct TrieNode *node)
{
    if (node)
    {
        for (int i = 0; i < 26; i++)
        {
            if (node->children[i] != NULL)
            {
                deallocateTrieNode(node->children[i]);
            }
        }
        free(node);
    }
}

// Deallocates the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie)
    {
        if (pTrie->root)
        {
            deallocateTrieNode(pTrie->root);
        }
        free(pTrie);
    }
    return NULL;
}

// Reads the words from a file and stores them in pInWords array
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening the file %s\n", filename);
        exit(1);
    }

    int numWords = 0;
    char word[100];

    while (fscanf(file, "%s", word) != EOF)
    {
        pInWords[numWords] = strdup(word);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char *inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Words to search for in the trie
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (pTrie == NULL)
    {
        printf("Trie deallocated successfully.\n");
    }

    // Free memory used for the input words
    for (int i = 0; i < numWords; ++i)
    {
        free(inWords[i]);
    }

    return 0;
}