#include "subsum.h"

/***********************************/
/*   Fonctions de base             */
/* sur les pair_t et les triple_t  */
/***********************************/


/*Initialisation*/

//Allocate the dimension
pair_t * pair_alloc(){
pair_t *P;

    P = malloc(sizeof(subsum_t));
    if(P==NULL){
        fprintf(stderr, "Pair allocation failed");
        return NULL;
    }

	//init x and val
    word_init(P->val);
    word_init(P->x); //default value = 0
    return P;
}

//Allocate the dimension and set to value val and corresponding x
pair_t * pair_allocToValue(word_t x, word_t val){
pair_t *P;

	P = pair_alloc();
	word_cpy(P->val, val);
	word_cpy(P->x, x);
    return P;
}

//Set pair to value and corresponding x:
void pair_setValue(pair_t * P, word_t x, word_t val){
	word_cpy(P->val, val);
	word_cpy(P->x, x);
	return;
}


/*Free memory*/

void pair_free(pair_t *P){
	word_clear(P->val);
	word_clear(P->x);
	free(P);
}

