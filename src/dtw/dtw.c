#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include "dtw.h"


void show_best_path(double** trellis,int one_length,int two_length){
	int i = one_length-1;
	int j = 0;
	int insertions = 0;
	int deletions = 0;
	int substitutions = 0;

	while(X(i)>0 && Y(j,two_length)>0){
		int down = trellis[i][j+1];
		int left = trellis[i-1][j];
		int down_left = trellis[i-1][j+1];
		int min = MIN(down,left,down_left);
		if(down==min){
			j++;
			insertions++;
		}
		else if(left==min){
			i--;
			deletions++;
		}
		else{
			if(trellis[i][j]!=trellis[i-1][j+1]){
				substitutions++;
			}
			i--;
			j++;
			
		}
	}
	printf("I: %d\tD: %d\tS: %d\n",insertions,deletions,substitutions);
	
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
    word_and_score * n_best_words = 
		(word_and_score *) malloc(n*sizeof(word_and_score));
    for(int i=0; i<n; i++){
        n_best_words[i].word=NULL;
        n_best_words[i].score=INT_MAX;
    }   

    for(int i=0; i < dict_length ; i++){
        search_trellis* trellis = 
			get_levenshtein_distance(word_one, dictionary[i]);
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


search_trellis* get_dtw_distance (feature_vectors word_one, feature_vectors 
	word_two) {
    int word_one_length = strlen(word_one);
    int word_two_length = strlen(word_two);

    char * null_prefix_word_one = add_null_prefix(word_one);
    char * null_prefix_word_two = add_null_prefix(word_two);

    search_trellis* trellis = build_trellis(null_prefix_word_one,
        null_prefix_word_two, word_one_length+1, word_two_length+1,
        ABSOLUTE);
    return trellis;
}


feature_vectors add_null_prefix(feature_vectors data) {
    feature_vector null_prefix = NULL;
    int new_num_vectors = data.num_vectors+1;
	feature* null_prefix_data = 
		(feature_vectors) malloc((new_num_vectors)*sizeof(feature));
    if(!null_prefix_data){ printf("malloc failed..\n"); exit(1);}
    null_prefix_data[0]=null_prefix;
    for(int i=1; i<new_num_vectors; i++) {
        null_prefix_data[i]=data[i-1];
    }
    return null_prefix_data;
}

/* TODO: update this function for DTW */
void print_n_best_words(word_and_score* n_best_words, int n){
    printf("\n");
    for(int p=0; p<n; p++){
        printf("%s: %d\n",n_best_words[p].word,n_best_words[p].score);
        print_dtw_trellis(n_best_words[p].trellis);
    }   
    printf("\n");
}

void free_dictionary(feature* dictionary, int dictionary_size){
    for(int i=0; i<dictionary_size; i++){
        free(dictionary[i]);
    }
    free(dictionary);
}


double feature_distance(feature v1, feature v2) {
	double total;

	for (int i = 0; i < CEPSTRUM_DIMENSION; i++) {
		total += pow((v1.values[i] - v2.values[i]), 2);
		total += pow((v1.deltas[i] - v2.values[i]), 2);
		total += pow((v1.doubles[i] - v2.doubles[i]), 2);
	}

	return sqrt(total);
}
