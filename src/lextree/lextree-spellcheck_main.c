#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lextree.h"
#include "lextree_spellcheck.h"

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Usage: ./lextree-spellcheck [-s] [string]\n");
        printf("    Returns up to 10 close matches to a string.\n");
        printf("    If you provide the -s flag then it segments as well.\n\n");
        exit(1);
    }


    //Read dictionary
	// TODO: make this a macro
    printf("Building lextree...\n");
	int dict_size = 6250;
	lextree* lex = build_lextree_from_file("text/dict-assn4.txt", dict_size);
    printf("Lextree built.\n\n");


    //Determine string and segmentation properties
    bool segment = false;
    char* string = argv[1];
    if(argc > 2 && strcmp(string,"-s") == 0)
    {
        segment = true;
        string = argv[2];
    }


    //Run the spellcheck
    printf("Spellchecking \"%s\"...\n", string);
    char* result = lextree_closest(lex, string, segment);
    printf("Result: %s\n", result);


    return 0;
}


