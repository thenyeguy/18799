#include "spell_checker_main.h"


int main(){
	
	char * word_one = "discovery"; 
	char ** dictionary = get_dictionary("words.txt");
	int n =7;
	word_and_score * best_n_words = get_best_n_words(word_one,dictionary,n);
	print_n_best_words(best_n_words,n);
	free_dictionary(dictionary,DICTIONARY_LENGTH);
	return 0;
}


char * get_input_string(){
	//Not Used Yet
	char * input_string = (char *)malloc(64);
	strcpy(input_string,"this is an input string");
	return input_string;
}

char ** endpoint_input_string(char * input_string){
	input_string=input_string;
	//Not Used Yet

	/*
	int number_of_words=1;
	int input_string_length=strlen(input_string);
	int i;
	for(i=0; i<input_string_length; i++){
		if(input_string[i]==' '||input_string[i]=='\0'){
			number_of_words++;
		}
	}
	printf("Number of words: %d\n",number_of_words);
		
	char ** input_word_array = (char **) malloc(number_of_words);
	char * word_start = input_string;
	int word_length=1;
	for(i=0; i<input_string_length; i++){
		if(input_string[i]==' '||input_string[i]=='\0'){
			printf(" length: %d\n",word_length-1);
			
			word_start+=(word_length);
			word_length=0;
		}
		word_length++;
	}
	*/
	return NULL;
}

