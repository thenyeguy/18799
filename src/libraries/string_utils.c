#include "string_utils.h"


string_array * split_string(char * string,char del){
	string_array * array = (string_array *) malloc(sizeof(string_array));
	int string_length = (int)strlen(string);
//	printf("parsing: %s\n",string);
//	printf("strlen: %d\n",(int)strlen(string));	
	//Count the number of words we will be splitting
	int num_words=1;
	for(int i = 0; i<strlen(string); i++){
		if(string[i]==del){
			num_words++;
		}	
	}
//	printf("word count: %d\n",num_words);

	//Malloc space for the words
	char ** words = (char **) malloc(sizeof(char*) * num_words);
	
	//Add each word into the char ** array
	int word_length=0;
	int word_index = 0;
	int string_index = 0;
	do{
		word_length++;
		if(string[string_index] == del || string_index> string_length){
			//Allocate space based on the word length
//			printf("Word %d has length: %d\n",word_index,word_length);
			char * word = (char*) malloc(sizeof(char) * word_length +1);
			memcpy(word, &(string[string_index-word_length]), word_length);
			word[word_length] = '\0';
//			printf("Copied: %s\n",word);

			//Add word into the array 
			words[word_index] = word;
			word_index++;

			//Reset the word length to 0
			word_length = -1;
		}
		string_index++;
	}while(word_index<num_words);
	
	array->strings = words;
	array->num_strings = num_words; 

	string_array_print(array);
	return array;
}


void string_array_print(string_array * array){
	printf("===========STRING ARRAY=========\n");
	for(int i = 0; i<array->num_strings; i++){
		printf("%s\n",array->strings[i]);
	}
	printf("================================\n");
}




