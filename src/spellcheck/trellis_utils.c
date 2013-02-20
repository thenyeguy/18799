#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include "trellis_utils.h"


int ** build_string_trellis(char ** string_array_one,char ** string_array_two){
	int ** trellis = init_string_trellis(string_array_one,string_array_two);
	populate_string_trellis(trellis,string_array_one,string_array_two);
	
	return trellis;
}

void print_string_trellis(int ** trellis,int height,int width){
	for(int j=0; j<height; j++){
		for(int i=0; i<width; i++){
			printf("%d\t",trellis[i][j]);
		}
		printf("\n");
	}
	printf("\n"); 

}

void populate_string_trellis(int ** trellis, char ** string_array_one,char ** string_array_two){
	int str_one_len = get_string_array_length(string_array_one);
        int str_two_len = get_string_array_length(string_array_two);
	
	//int insertions = 0;
	//int deletions = 0;
	//int substitutions = 0;
	
	//printf("one: %d\t two: %d\n",str_one_len,str_two_len);

	for(int i=0; i<str_one_len; i++){	
		for(int j=str_two_len-1; j>=0; j--){
			int x = X(i);
			int y = Y(j,str_two_len);
			char * one = string_array_one[x];
                        char * two = string_array_two[y];
			if(x==0){	
				trellis[i][j]=y;
			}
			else if(y==0){
				trellis[i][j]=x;
			}
			else{
				int diag_score;
				if(strcmp(one,two)==0){
					diag_score = 0;
				}
				else{
					diag_score = 1;
				}
				int weight = 1;
				int down_score = trellis[i][j+1];
				int left_score = trellis[i-1][j];
				int down_left_score = trellis[i-1][j+1];
				down_score+=weight;
				left_score+=weight;
				down_left_score+=diag_score;			

					
				int next_score = MIN(down_score,left_score,down_left_score);
				/*
				if(next_score==down_score){
					insertions++;
				}
				else if(next_score==left_score){
					deletions++;
				}
				else{
					if(diag_score==0){
						
					}
					else{
						substitutions++;
					}
				}
				*/
				trellis[i][j]=next_score;
				
			}
			//printf("comparing: %s - %s\n",one,two);
			//printf("(%d,%d) = %d\n",x,y,trellis[i][j]);
		}
	}
	//print_string_trellis(trellis,str_two_len,str_one_len);
	//printf("Insertions: %d\t Deletions: %d\t Substitutions: %d\n",insertions,deletions,substitutions);
}

int ** init_string_trellis(char ** string_array_one,char ** string_array_two){
	int str_one_len = get_string_array_length(string_array_one);
	int str_two_len = get_string_array_length(string_array_two);
	int ** trellis = (int **) malloc(sizeof(int*)*str_two_len);
	if(!trellis){
		printf("problem\n");	
		exit(1);
	}
	for(int i=0; i<str_two_len; i++){
		trellis[i] = (int*)malloc(sizeof(int)*str_one_len);
	}

        return trellis;
}


void print_string_array(char ** string_array){
        int i=0;
        while(string_array[i]!=NULL){
                printf("%d: %s\n",i,string_array[i]);
                i++;
        }   
}

int get_string_array_length(char ** string_array){
        int i=0;
        while(string_array[i]!=NULL){
                i++;
        }  
        return i;
}



/*
 *      CODE FOR ONLY ONE CHARACTER
*/


trellis_node** build_trellis(char* word_one, char* word_two,
                             int word_one_length, int word_two_length,
                             prune_t pruning)
{
    trellis_node** trellis = alloc_trellis(word_one_length, word_two_length);
    populate_trellis(trellis, word_one, word_two, word_one_length,
                     word_two_length, pruning);

	if(PRINT_TRELLIS){
		print_trellis(trellis,word_one,word_two,word_one_length,word_two_length);
	} 
    return trellis;
}


trellis_node** alloc_trellis(int word_one_length, int word_two_length)
{
    int malloc_height_size = word_two_length * sizeof(trellis_node*);
    int malloc_width_size = word_one_length * sizeof(trellis_node);

    trellis_node ** trellis = (trellis_node**) malloc(malloc_height_size);

    for(int n = 0; n < word_two_length; n++)
    {
        trellis_node * temp_trellis_array = (trellis_node *) malloc(malloc_width_size);
        for(int i = 0; i < word_one_length; i++)
        {
            //Assume a node has been pruned to begin with
            //And we will unprune nodes that we wish to explore
            temp_trellis_array[i].score = INT_MAX;
            temp_trellis_array[i].pruned = false;
            temp_trellis_array[i].dir = DIR_NONE;
        }
        trellis[n] = temp_trellis_array;
    }
    return trellis;
}


void populate_trellis(trellis_node** trellis, char* word_one, char* word_two,
                      int word_one_length, int word_two_length,
                      prune_t pruning)
{
    int threshold = INT_MAX;
    int column_min = INT_MAX;
    if(pruning == ABSOLUTE)
        threshold = TRELLIS_ABSOLUTE_THRESHOLD;
    else if(pruning == BEAM)
        threshold = TRELLIS_BEAM_THRESHOLD;

    //Loop over every trellis node
    trellis[0][0].pruned = false;
    for(int i = 0; i < word_one_length; i++)
    {
        column_min = INT_MAX;

        for(int j = 0; j < word_two_length; j++)
        {
            //If the node has been pruned, carry on
            if(trellis[j][i].pruned) continue;

            //Score this node
            get_best_score(trellis, i, j, word_one, word_two,
                           word_one_length, word_two_length);

            //Keep track of the beam threshold
            if(pruning == BEAM && trellis[j][i].score < column_min)
                column_min = trellis[j][i].score;
        }

        //Recompute beam threshold
        if(pruning == BEAM)
            threshold = column_min + TRELLIS_BEAM_THRESHOLD;

        //Perform threshold pruning
        if(i < word_one_length-1 && pruning != NONE)
        {
            for(int j = 0; j < word_two_length; j++)
            {
                bool check = false;
                //Left
                if(trellis[j][i].score < threshold)
                    check = true;
                //Downleft
                if(j > 0 && trellis[j-1][i].score < threshold)
                    check = true;
                trellis[j][i+1].pruned = !check;
            }
        }
    }
}


void get_best_score(trellis_node** trellis, int i, int j,
                    char* word_one, char* word_two,
                    int word_one_length, int word_two_length)
{
    (void) word_one_length;

    int down = INT_MAX;
    int left = INT_MAX;
    int down_left = INT_MAX;

    bool same = word_one[i] == word_two[j];
    bool seen = false;

    //Down
    if(j > 0 && !trellis[j-1][i].pruned){
        down = trellis[j-1][i].score + 1;
        seen = true;
    }
    //Left
    if(i>0 && !trellis[j][i-1].pruned){
        left = trellis[j][i-1].score + 1;
        seen = true;
    }
    //Down Left
    if(j>0 && i>0 && !trellis[j-1][i-1].pruned){
        down_left = trellis[j-1][i-1].score;
        if(!same) down_left++;
        seen = true;
    }
    if(!seen){
        trellis[j][i].score = 0;
    }else if(down < left && down < down_left){
        trellis[j][i].score = down;
        trellis[j][i].dir = DIR_DOWN;
    }else if(left < down_left){
        trellis[j][i].score = left;
        trellis[j][i].dir = DIR_LEFT;
    }else{
        trellis[j][i].score = down_left;
        trellis[j][i].dir = DIR_DOWNLEFT;
    }
}


void print_trellis(trellis_node** trellis, char* word_one, char* word_two,
                   int word_one_length, int word_two_length)
{
    for(int j=word_two_length-1; j>=0; j--){
        printf("%c\t",word_two[j]);

        for(int i=0; i<word_one_length; i++){
            char dir;
            switch(trellis[j][i].dir)
            {
                case DIR_NONE:
                    dir = 'x';
                    break;
                case DIR_DOWN:
                    dir = '|';
                    break;
                case DIR_LEFT:
                    dir = '-';
                    break;
                case DIR_DOWNLEFT:
                    dir = '/';
                    break;
            }
            printf("%c,%d\t",dir,trellis[j][i].score);
        }
        printf("\n");
    }
    printf("\t");

    for(int i=0; i<word_one_length; i++){
        printf("%c\t",word_one[i]);
    }
    printf("\n");
}


void free_trellis(trellis_node** trellis, int word_one_length, int word_two_length)
{
    word_one_length=word_one_length;
    int j;
    for(j=0; j < word_two_length; j++){
        free(trellis[j]);
    }   
    free(trellis);
}
