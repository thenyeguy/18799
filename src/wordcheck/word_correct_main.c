#include <stdlib.h>
#include <stdio.h>
#include "word_levenshtein_utils.h"


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
    char** dictionary = word_get_dictionary("text/smalldict.txt", SMALLDICT_LENGTH);

    //Read line by line, then tokenize each line word by word
    //Output the corresponding best word match
    char line[5096];
    char* word;
    while(fgets(line,5096,in) != NULL)
    {
        for(word = strtok(line," "); word != NULL; word = strtok(NULL," "))
        {
            printf("%s ... ",word); fflush(stdout);
            word_and_score* best =
                word_get_best_n_words(word, dictionary, SMALLDICT_LENGTH, 1);

            char* result;
            if(best->word == NULL) result = word;
            else               result = best->word;



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


