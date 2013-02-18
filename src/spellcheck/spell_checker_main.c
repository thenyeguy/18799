#include "spell_checker_main.h"


int main(){
	//Open input file
	FILE * filepath = fopen("text/prepped-Indian.txt","r");
	if(!filepath){ printf("Couldn't open input text\n"); exit(0);}

	//Build dictionary for comparison
	char ** dictionary = get_dictionary(DICTIONARY_PATH);

	int buffer_size = 80;
	char line[buffer_size];
	while(fgets(line,buffer_size,filepath) != NULL){
		int string_length = strlen(line);
		line[string_length-1]='\0';
		//printf("Line: %s\t\tLen: %d\n",line,string_length);
		int n =7;
		word_and_score * best_n_words = get_best_n_words(line,dictionary,n);
		//print_n_best_words(best_n_words,n);
		char * best_fit = best_n_words[0].word;
		printf("%s --> %s\n",line,best_fit);
	}

	/*
	char * word_one = "discovery"; 
	int n =7;
	word_and_score * best_n_words = get_best_n_words(word_one,dictionary,n);
	print_n_best_words(best_n_words,n);
	*/
	
	//Free dictionary from memory 
	free_dictionary(dictionary,DICTIONARY_LENGTH);

	return 0;
}
