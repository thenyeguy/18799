#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include "dtw.h"
#include "feature.h"

void show_best_dtw_path(dtw_t trellis,int one_length,int two_length){
/*	int i = one_length-1;
	int j = 0;
	int insertions = 0;
	int deletions = 0;
	int substitutions = 0;

	while(X(i)>0 && Y(j,two_length)>0){
		double left = 
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
*/	


}

char** get_null_prefix_dictionary_dtw(char* dictionary_filepath, 
	int dict_length){
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



feature_vectors* get_features_from_file(char* recording_name, int num_filters){
	
	char cepstrum_filename[FILENAME_BUF_LENGTH];
	char line_buffer[LINE_BUF_LENGTH];

	int num_vectors = 0;

	feature_vectors* vectors = malloc(sizeof(feature_vectors));
	vectors->features = malloc(sizeof(feature)*MAX_FEATURE_VECTORS);

	sprintf(cepstrum_filename, "../../analysis/%s-%d-cepstrum.out", 
		recording_name, num_filters);
	
	FILE* cepstrum_file = fopen(cepstrum_filename, "r");

	while(fgets(line_buffer, LINE_BUF_LENGTH, cepstrum_file)) {
		/// delimit line with spaces
		/// atod each word
		/// load into cepstrum feature vector
		num_vectors++;
	}
	
	vectors->num_vectors = num_vectors;

	return vectors;
}

/// TODO: make this compile first then fix it
features_and_score* get_best_n_words_dtw(char* word_one, char** dictionary,
                                 int dict_length, int n){ 
    //N Best on first pass
	/*
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
	*/

	return NULL;
}


dtw_t* get_dtw_distance (feature_vectors word_one, feature_vectors 
	word_two) {
	/*
    int word_one_length = strlen(word_one);
    int word_two_length = strlen(word_two);

    char * null_prefix_word_one = add_null_prefix(word_one);
    char * null_prefix_word_two = add_null_prefix(word_two);

    search_trellis* trellis = build_trellis(null_prefix_word_one,
        null_prefix_word_two, word_one_length+1, word_two_length+1,
        ABSOLUTE);
    return trellis;
	*/
	return NULL;

}


feature_vectors* add_null_prefix_dtw(feature_vectors data) {
    int new_num_vectors = data.num_vectors+1;
	
	feature* null_prefix_data = 
		(feature*) malloc((new_num_vectors)*sizeof(feature));
    
	if(!null_prefix_data){ printf("malloc failed..\n"); exit(1);}
    
	/* this doesn't work, how do we encode a null prefix */
	//null_prefix_data[0]=NULL_PREFIX;

    /* 
	/// Have to memcpy this
	
	for(int i=1; i<new_num_vectors; i++) {
        null_prefix_data[i]=data[i-1];
    }
	*/
    
	feature_vectors* null_prefix_features = 
		(feature_vectors*) malloc(sizeof(feature_vectors));
	null_prefix_features->features = null_prefix_data;
	null_prefix_features-> num_vectors = new_num_vectors;
	
	return null_prefix_features;
}

/* TODO: update this function for DTW */
void print_n_best_words_dtw(features_and_score* n_best_words, int n){
	/*
    printf("\n");
    for(int p=0; p<n; p++){
        printf("%s: %d\n",n_best_words[p].word,n_best_words[p].score);
        print_dtw_trellis(n_best_words[p].trellis);
    }   
    printf("\n");
	*/
}

void free_dictionary_dtw(feature* dictionary, int dictionary_size){
   /* for(int i=0; i<dictionary_size; i++){
        free(dictionary);
    }
	*/
    free(dictionary);
}


double score(feature* test, feature* template, int test_index, 
	int template_index, dtw_trellis_dir direction) {

	feature test_feature = test[test_index];
	feature template_feature = template[template_index];

	return feature_distance(test_feature, template_feature);	

}
