#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lextree.h"
#include "lextree_spellcheck.h"

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Usage: ./spellcheck [file to check]\n");
        printf("    Correct \n\n");
        printf("E.G. \"./spellcheck mytest\" reads in:\n");
        printf("         text/mytest.txt\n");
        printf("     and saves files such as:\n");
        printf("         text/mytest.out\n");
        exit(1);
    }

    //Determine the specified recording out file
    char data_path[256];
	sprintf(data_path, "text/%s.txt", argv[1]);
    printf("Reading text data from:\n    %s\n\n", data_path);
	
	char name[256];
	sprintf(name, "text/%s.out", argv[1]);

    //Open the file and determine its length to read in
    //Then close it when done
    FILE* in = fopen(data_path, "r"); 
    FILE* out = fopen(name, "w"); 
    if(in == NULL || out == NULL)
    {
        printf("Could not open specified file.\n");
        exit(1);
    }


    //Read dictionary

	// TODO: make this a macro
	int dict_size = 6250;
	
	lextree* lex = build_lextree_from_file("text/dict-assn4.txt", dict_size);

    //Read line by line, then tokenize each line word by word
    //Output the corresponding best word match
    char line[5096];
    char* word;
    while(fgets(line,5096,in) != NULL)
    {
        for(word = strtok(line," \n"); word != NULL; word = strtok(NULL," \n"))
        {
            printf("%s ... ",word); fflush(stdout);
			lextree_scored_word** word_matches = lextree_closest_n_words(lex, word, 1);
            char* result = word_matches[0] ? word_matches[0]->word : NULL;

            fprintf(out, "%s ", result);
            printf("%s\n", result);
        }
        fprintf(out, "\n");
    }

    fclose(out);
    fclose(in);
    printf("Text corrected. Output stored to:\n    %s\n\n", name);

    return 0;
}


