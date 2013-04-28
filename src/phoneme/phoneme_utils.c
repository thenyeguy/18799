#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "phoneme_utils.h"

gaussian_cluster** get_initial_phoneme_models(feature_vectors** recordings,
											  int num_recordings) {
	
	
	return NULL;
}



int phonemeToModelIndex(char* phoneme) {
	if (!strcmp(phoneme,"AX"))
		return 0;
	else if (!strcmp(phoneme,"AH"))
		return 1;
	else if (!strcmp(phoneme,"AY"))
		return 2;
	else if (!strcmp(phoneme,"EH"))
		return 3;
	else if (!strcmp(phoneme,"EY"))
		return 4;
 	else if (!strcmp(phoneme,"F"))
		return 5;
 	else if (!strcmp(phoneme,"IH"))
		return 6;
 	else if (!strcmp(phoneme,"IY"))
		return 7;
 	else if (!strcmp(phoneme,"K"))
		return 8;
 	else if (!strcmp(phoneme,"N"))
		return 9;
 	else if (!strcmp(phoneme,"OW"))
		return 10;
 	else if (!strcmp(phoneme,"R"))
		return 11;
 	else if (!strcmp(phoneme,"S"))
		return 12;
 	else if (!strcmp(phoneme,"T"))
		return 13;
 	else if (!strcmp(phoneme,"TH"))
		return 14;
 	else if (!strcmp(phoneme,"UW"))
		return 15;
 	else if (!strcmp(phoneme,"V"))
		return 16;
 	else if (!strcmp(phoneme,"W"))
		return 17;
 	else if (!strcmp(phoneme,"Z"))
		return 18; 
 }

int wordToModelIndex(char* word)
{
    if(strcmp(word,"zero") == 0)
        return 0;
    else if(strcmp(word,"one") == 0)
        return 1;
    else if(strcmp(word,"two") == 0)
        return 2;
    else if(strcmp(word,"three") == 0)
        return 3;
    else if(strcmp(word,"four") == 0)
        return 4;
    else if(strcmp(word,"five") == 0)
        return 5;
    else if(strcmp(word,"six") == 0)
        return 6;
    else if(strcmp(word,"seven") == 0)
        return 7;
    else if(strcmp(word,"eight") == 0)
        return 8;
    else if(strcmp(word,"nine") == 0)
        return 9;
    else if(strcmp(word,"sil") == 0)
        return 10;
    else if(strcmp(word,"oh") == 0)
        return 11;

    return -1;
}
