#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include "dtw.h"
#include "feature.h"

void dtw_main(int argc, char* argv[]){
	
	if (argc!= 3) {
		dtw_usage();
	}

	char* input_name = argv[1];
	//char* dictionary_fp = argv[2];
	int n = atoi(argv[3]);

	/// build the feature vector dictionary
	
	 /// FIXME: need to build a dictionary from recording library, I'm not sure
	 /// how to call features_from_all_files in this way
	feature_vectors** dictionary = features_from_all_files(1, argv+2);

	/// This expects a cepstrum file for now, maybe in the future we want to
	/// allow the user to record a new file and perform the analysis and 
	/// matching all in one step.
	feature_vectors* input = features_from_file(input_name);

	get_best_n_words_dtw(input, dictionary, DICTIONARY_LENGTH, n);

}



dtw_t** get_best_n_words_dtw(feature_vectors* input, 
	feature_vectors** dictionary, int dict_length, int n){ 

	dtw_t** trellis_list = get_trellis_list(input, dictionary, dict_length);

	dtw_t** best_n = 
		get_best_n_from_trellis(trellis_list, dict_length, n);
	
	print_n_best_words_dtw(best_n, n);

	return best_n;
}


bool insert(dtw_t* input, dtw_t** list, int length) {
	int i = 0;
	while (i<length) {

		/// if we've gotten to empty space, add the trellis by default
		if (!list[i]) {
			list[i] = input;
			return true;
		}
		
		if (input->score > list[i]->score) {
			dtw_t* temp1 = input;
			dtw_t* temp2 = list[i];
			for (int j = i; j<length; j++) {
				list[j] = temp1;
				temp1 = temp2;
				if (j+1 < length) temp2 = list[j+1];
			}
			return true;
		}
		i++;
	}
	return false;
}


dtw_t** get_trellis_list (feature_vectors* input_data, 
							feature_vectors** dictionary, int dict_length) {
	/// allocate space for list
	dtw_t** trellis_list = (dtw_t**) malloc(sizeof(void*)*dict_length);
	
	dtw_prune_t prune = SET_PRUNING;
	
	for (int i = 0; i < dict_length; i++) {
		trellis_list[i] = new_dtw(
				input_data, input_data->num_vectors,
				dictionary[i], dictionary[i]->num_vectors,
				prune, PRUNING_THRESHOLD,
				&score);
	}

	return trellis_list;
}


dtw_t** get_best_n_from_trellis(dtw_t** trellis, int trellis_length, int n) {
	bool scored = false;
	double prune_threshold = DBL_MAX;
	while (!scored) {
		/// score a column of each trellis in the list, 
		/// and update the pruning threshold
		for (int i = 0; i<trellis_length; i++) {
			scored = dtw_fill_next_col(trellis[i]);
			prune_threshold = fmin(prune_threshold, trellis[i]->score);
		}
	}

	/// allocate space for score list
	dtw_t** scores = 
		(dtw_t**) calloc(sizeof(void*), n);

	for (int i = 0; i<trellis_length; i++) {
		insert(trellis[i], scores, n);
	}

	return scores;
}


void print_n_best_words_dtw(dtw_t** n_best_words, int n){
	for (int i = 0; i<n; i++) {
		
	   feature_vectors* feature_vector = 
			(feature_vectors*)(n_best_words[i]->template_data);

		char* id = (feature_vector)->word_id;

		printf("Word: %s, Score: %f\n", 
				id, n_best_words[i]->score);	
	}
}


void free_dictionary_dtw(feature_vectors* dictionary, int dictionary_size){
    for(int i=0; i<dictionary_size; i++){
        free(dictionary[i].features);
    }
    free(dictionary);
}


// TODO: How do you incorporate direction into the score?
// Also, right now this function just computes the distance between
// two feature vectors, which is more of a cost than a score.  Translate
// this into a score to work with trellis functions
double score(void* test, void* template, int test_index, 
	int template_index, dtw_trellis_dir direction) {

	feature test_feature = ((feature_vectors*)test)->features[test_index];
	feature template_feature = 
		((feature_vectors*)template)->features[template_index];

	return feature_distance(test_feature, template_feature);	
}



dtw_t* get_dtw_distance (feature_vectors* data1, feature_vectors* data2) {
	/// add null prefix to the data
//	feature_vectors* data1_features = add_null_prefix_dtw(data1);
//	feature_vectors* data2_features = add_null_prefix_dtw(data2);

	// TODO: add an object for pruning and a pruning threshold
	dtw_prune_t prune = SET_PRUNING;
	
	/// build the trellis
	dtw_t* trellis = new_dtw(
				data1, data1->num_vectors, data2, data2->num_vectors,
				prune, PRUNING_THRESHOLD, &score);
	
	/// score the trellis
	while (!dtw_fill_next_col(trellis));
	return trellis;
}


void dtw_usage() {
	printf("USAGE: ./dtw [input_filename], [dictionary_filepath], [number of best matches]\n\nThe input file should be a cepstrum vector file.");
}



feature_vectors* add_null_prefix_dtw(feature_vectors* data) {
/*    int new_num_vectors = data->num_vectors+1;
	
	feature** null_prefix_data = 
		(feature**) malloc((new_num_vectors)*sizeof(feature));
    
	if(!null_prefix_data){ printf("malloc failed..\n"); exit(1);}
    
	/// add a null prefix
	// FIXME: can't copy NULL here, what do we do?
	//memcpy(null_prefix_data[0], 0, sizeof(feature));

	/// copy the original data
	for(int i=1; i<new_num_vectors; i++) {
        memcpy(null_prefix_data[i], (data->features)[i], sizeof(feature));
    }
	
	feature_vectors* null_prefix_features = 
		(feature_vectors*) malloc(sizeof(feature_vectors));
	null_prefix_features->features = null_prefix_data;
	null_prefix_features-> num_vectors = new_num_vectors;
*/	
	return data;
}

feature_vectors** get_null_prefix_dictionary_dtw(){
/*	char** template_filenames;
	feature_vectors** dictionary = 
		features_from_all_files(DICTIONARY_LENGTH, template_filenames); 
	
	/// allocate space for null prefix dictionary
	feature_vectors** null_prefix_dictionary = 
		malloc(sizeof(feature_vectors) * DICTIONARY_LENGTH);
 
 	/// create null prefix features for each template
 	for (int i = 0; i<DICTIONARY_LENGTH; i++) {
		null_prefix_dictionary[i] = add_null_prefix_dtw(dictionary[i]);	
	}
*/
//	return null_prefix_dictionary;
	return NULL;

}

