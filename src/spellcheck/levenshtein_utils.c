#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include "levenshtein_utils.h"





int compute_levenshtein(char ** story_one, char** story_two){
	//print_string_array(story_one);
	//printf("len: %d\n",get_string_array_length(story_one));
	int one_length = get_string_array_length(story_one);
	int two_length = get_string_array_length(story_two);
	int ** trellis = build_string_trellis(story_one,story_two);
	return trellis[two_length-1][one_length-1];
}





char** get_null_prefix_dictionary(char* dictionary_filepath, int dict_length){
    //235886        
    FILE * filepath = fopen(dictionary_filepath,"r");
    int dictionary_length = dict_length + 1;
    char ** dictionary = (char **)malloc(dictionary_length * sizeof(char*));
    if(!filepath){ printf("could not open dictionary...\n"); exit(1); }
    if(!dictionary){ printf("malloc failed..\n"); exit(1);}

    int line_length = 80; 
    char line[line_length];
    int i = 1;
    while(fgets(line, line_length, filepath) != NULL && i<dict_length){
        int word_length = strlen(line);
        char * word = (char *)malloc(word_length+1);
        int k;
        for(k=0; k<word_length; k++){
            word[k]=line[k];
        }   
        word[word_length-1]='\0';
        dictionary[i]=word;
        i++;    
    } 
  
    //Add null prefix '*' to the beginning of the story
    char * null_prefix = (char*)malloc(strlen(null_prefix)+1);
    null_prefix[0] = '*';
    null_prefix[1] = '\0';  	
    dictionary[0] = null_prefix; 

    fclose(filepath);
    return dictionary;
}


/*
 *	CODE FOR ONLY ONE CHARACTER
*/

char** get_dictionary(char* dictionary_filepath, int dict_length){
    //235886        
    FILE * filepath = fopen(dictionary_filepath,"r");
    int dictionary_length = dict_length;
    char ** dictionary = (char **)malloc(dictionary_length * sizeof(char*));
    if(!filepath){ printf("could not open dictionary...\n"); exit(1); }
    if(!dictionary){ printf("malloc failed..\n"); exit(1);}

    int line_length = 80;
    char line[line_length];
    int i = 0;
    while(fgets(line, line_length, filepath) != NULL && i<dict_length){
        int word_length = strlen(line);
        char * word = (char *)malloc(word_length+1);
        int k;
        for(k=0; k<word_length; k++){
            word[k]=line[k];
        }   
        word[word_length-1]='\0';
        dictionary[i]=word;
        i++;    
    }   
    fclose(filepath);
    return dictionary;
}


word_and_score* get_best_n_words(char* word_one, char** dictionary,
                                 int dict_length, int n){ 
    //N Best on first pass
    word_and_score * n_best_words = (word_and_score *) malloc(n*sizeof(word_and_score));
    for(int i=0; i<n; i++){
        n_best_words[i].word=NULL;
        n_best_words[i].score=INT_MAX;
    }   

    for(int i=0; i < dict_length ; i++){
        search_trellis* trellis = get_levenshtein_distance(word_one, dictionary[i]);
        int levenshtein_distance = trellis->distance;
	
        if(levenshtein_distance < n_best_words[n-1].score){
            int k=0;
            while(n_best_words[k].score<=levenshtein_distance){
                k++;    
            }   
            char * new_word = (char *)malloc(strlen(dictionary[i])+1);
            strcpy(new_word,dictionary[i]);
            n_best_words[k].score = levenshtein_distance;   
            n_best_words[k].trellis = trellis;
            free(n_best_words[k].word);
            n_best_words[k].word = new_word;
        }    
    }
   
    return n_best_words;
}


search_trellis* get_levenshtein_distance (char* word_one, char* word_two){
    int word_one_length = strlen(word_one);
    int word_two_length = strlen(word_two);

    char * null_prefix_word_one = add_null_prefix(word_one);
    char * null_prefix_word_two = add_null_prefix(word_two);

    search_trellis* trellis = build_trellis(null_prefix_word_one,
        null_prefix_word_two, word_one_length+1, word_two_length+1,
        ABSOLUTE);
    return trellis;
}


char* add_null_prefix(char* word){
    char null_prefix = '*';
    int new_word_length = strlen(word)+1;
    char * null_prefix_word = (char*) malloc(new_word_length+1);
    if(!null_prefix_word){ printf("malloc failed..\n"); exit(1);}
    null_prefix_word[0]=null_prefix;
    for(int i=1; i<new_word_length; i++){
        null_prefix_word[i]=word[i-1];
    }
    null_prefix_word[new_word_length]='\0';
    return null_prefix_word;
}


void print_n_best_words(word_and_score* n_best_words, int n){
    printf("\n");
    for(int p=0; p<n; p++){
        printf("%s: %d\n",n_best_words[p].word,n_best_words[p].score);
        print_trellis(n_best_words[p].trellis);
    }   
    printf("\n");
}

void free_dictionary(char** dictionary, int dictionary_size){
    for(int i=0; i<dictionary_size; i++){
        free(dictionary[i]);
    }
    free(dictionary);
}

