#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lextree.h"


lextree* build_lextree_from_file(char* dictionary_path, int dict_size)
{
    char buffer[256];
    FILE* file = fopen(dictionary_path,"r");
    if(file==NULL){
	printf("Failed to open file %s\n",dictionary_path);
    }
    char** dictionary = malloc(dict_size*sizeof(char*));
    for(int i = 0; i < dict_size; i++)
    {
        fgets(buffer, 256, file);

        int len = strlen(buffer);
        char* word = malloc(len*sizeof(char));
        copy_word_only_lowercase(word, buffer);
        dictionary[i] = word;
    }

    //Build lextree
    printf("dictionary read.\n");
    lextree* lex = build_lextree(dictionary, dict_size);

    //Free dictionary
    for(int i = 0; i < dict_size; i++)
        free(dictionary[i]);
    free(dictionary);

    return lex;
}


void copy_word_only_lowercase(char* dest, char* source)
{
    int dest_i = 0;
    int source_i = 0;

    while(source_i < strlen(source))
    {
        char c = source[source_i];
        if('a' <= c && c <= 'z')
        {
            dest[dest_i] = c;
            dest_i++;
        }

        source_i++;
    }

    //Add null terminator
    dest[dest_i] = '\0';
}


lextree* build_lextree(char** dictionary, int dict_size)
{
    int depth = 0;
    lextree_node* head = new_lextree_node(' ');

    //Loop through every word in the dict and add it to our lextree
    //Also keep track of the longest word... this is our depth
    for(int i = 0; i < dict_size; i++)
    {
        add_word_to_lextree(head, dictionary[i]);

        int len = strlen(dictionary[i]);
        if(len > depth) depth = len;
    }

    //Create the wrapper lextree struct
    lextree* lex = malloc(sizeof(lextree));
    lex->size = dict_size;
    lex->depth = depth+1;
    lex->head = head;

    return lex;
}


void add_word_to_lextree(lextree_node* head, char* word)
{
    int len = strlen(word);

    // Look one character at a time. Traverse one node deeper
    lextree_node* current = head;
    lextree_node* next;
    for(int i = 0; i < len; i++)
    {
        int next_i = (word[i]) - 'a';
        next = current->children[next_i];
        
        //Create a new node if nessecary
        if(next == NULL)
        {
            next = new_lextree_node(word[i]);
            current->children[next_i] = next;
        }

        //Carry on
        current = next;
    }

    //Confirm that this node is the full word
    current->is_full_word = true;
}


lextree_node* new_lextree_node(char c)
{
    lextree_node* node = malloc(sizeof(lextree_node));
    node->c = c;
    node->is_full_word = false;

    for(int i = 0; i < 26; i++)
        node->children[i] = NULL;

    return node;
}


bool is_word_in_lextree(lextree* lex, char* word)
{
    int len = strlen(word);
    lextree_node* next = lex->head;

    //Traverse the children
    for(int i = 0; i < len; i++)
    {
        int next_i = (word[i]) - 'a';
        next = next->children[next_i];
        
        //If we run out of children, it can't be in the tree
        if(next == NULL)
            return false;
    }

    //If we got to the end, check whether the last character matches
    if(next->c != word[len-1]) return false;
    //and whether this is a terminating node or not
    else                       return next->is_full_word;
}


void print_lextree_node(lextree_node* node)
{
    char* not;
    if(node->is_full_word) not = "";
    else                   not = "not ";

    printf("This node represents '%c', and is %sa word end.\n", node->c, not);

    printf("It has children for: ");
    for(int i = 0; i < 26; i++)
    {
        if(node->children[i] != NULL)
            printf("%c ", 'a' + i);
    }
    printf("\n\n");
}


void print_lextree_contents(lextree* lex)
{
    char* buffer = malloc((lex->depth+2)*sizeof(char));

    printf("Traversing lextree with size %d and depth %d:\n",
           lex->size, lex->depth);
    print_lextree_contents_helper(lex->head, buffer, 0);

    free(buffer);
}


void print_lextree_contents_helper(lextree_node* node, char* buffer, int depth)
{
    //Copy this char into the buffer
    buffer[depth] = node->c;
    
    //If this is a word, print it out
    if(node->is_full_word)
    {
        buffer[depth+1] = '\0';
        printf("\t%s\n", buffer);
    }

    //Traverse the children
    for(int i = 0; i < 26; i++)
    {
        lextree_node* next = node->children[i];
        if(next != NULL)
            print_lextree_contents_helper(next, buffer, depth+1);
    }
}


void free_lextree_node(lextree_node* node)
{
    //Free the children
    for(int i = 0; i < 26; i++)
    {
        lextree_node* child = node->children[i];
        if(child != NULL)
            free_lextree_node(child);
    }

    //Free myself
    free(node);
}


void free_lextree(lextree* lex)
{
    free_lextree_node(lex->head);
    free(lex);
}
