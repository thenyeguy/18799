#include "trellis_utils.h"

void free_trellis(trellis_node ** trellis,int word_one_length,int word_two_length){
        word_one_length=word_one_length;
        int j;
        for(j=0; j < word_two_length; j++){
                free(trellis[j]);
        }   
        free(trellis);
}

trellis_node ** build_trellis(char * word_one, char * word_two, int word_one_length, int word_two_length){
	trellis_node ** trellis = initialize_trellis(word_one_length,word_two_length);
	populate_trellis(trellis,word_one,word_two,word_one_length,word_two_length);
	if(PRINT_TRELLIS){
		print_trellis(trellis,word_one,word_two,word_one_length,word_two_length);
	} 
	return trellis;
}

void populate_trellis(trellis_node ** trellis, char * word_one, char * word_two,int word_one_length, int word_two_length){
	int j;
	int i;
	for(i=0; i<word_one_length; i++){
		for(j=word_two_length-1; j>=0; j--){
			int next_best_score = get_next_best_score(trellis,i,j,word_one,word_two,word_one_length,word_two_length);
			trellis[j][i].score= next_best_score;
		}
	}
}
int get_next_best_score(trellis_node ** trellis,int i,int j,char * word_one, char * word_two,int word_one_length,int word_two_length){
        int down;
        int left;
        int down_left;
        int next_best_score;
	int x = X(i);
	int y = Y(word_two_length,j);
	int diag_cost;

	if(x==0){
		return y;
	}
	else if(y==0){
		return x;
	}	
	else{
	        if(word_one[x] == word_two[y]){
	        	diag_cost=0;
	        }   
	        else{
			diag_cost=1;
		}
		
	        down = trellis[j+1][i].score;
	        left = trellis[j][i-1].score;
	        down_left = trellis[j+1][i-1].score;
	        next_best_score =  MIN(down+1,left+1,down_left + diag_cost);
	        return next_best_score;
	}
}
/*
int get_best_previous_score(trellis_node ** trellis,int i,int j,int word_one_length,int word_two_length){
	int down = INT_MAX;
	int left = INT_MAX;
	int down_left = INT_MAX;
	int flag=0;
	int best_previous_score;
	word_one_length=word_one_length;
	//Down
	if(Y(word_two_length,j)>0){
		down = trellis[j+1][i].score;
		flag++;
	}
	//Left
	if(X(i)>0){
		left = trellis[j][i-1].score;
		flag++;
	}
	//Down Left
	if(Y(word_two_length,j)>0 && X(i)>0){
		down_left = trellis[j+1][i-1].score;
		flag++;
	}
	if(flag==0){
		best_previous_score =  0;
	}
	else{
		best_previous_score =  MIN(down,left,down_left);
	}
	return best_previous_score;
}
*/

trellis_node ** initialize_trellis(int word_one_length, int word_two_length){
	
	int malloc_height_size = word_two_length * sizeof(trellis_node*);
	int malloc_width_size = word_one_length * sizeof(trellis_node);
	
	trellis_node ** trellis = (trellis_node**) malloc(malloc_height_size);

	int n;
	for(n=0; n<word_two_length;n++){
		trellis_node * temp_trellis_array = (trellis_node *) malloc(malloc_width_size);
		trellis[n] = temp_trellis_array;
	}
	return trellis;
}


void print_trellis(trellis_node ** trellis,char * word_one, char * word_two, int word_one_length, int word_two_length){
	int j;
	int i;
	for(j=0; j<word_two_length; j++){
		printf("%c\t",word_two[word_two_length-j-1]);

		for(i=0; i<word_one_length; i++){
			printf("%d\t",trellis[j][i].score);
		}
		printf("\n");
	}
	printf("\t");
	for(i=0; i<word_one_length; i++){
		printf("%c\t",word_one[i]);
	}
	printf("\n");
}





