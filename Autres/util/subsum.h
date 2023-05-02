#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <limits.h>

//Activer l'affichage
#define DEBUG 0

//Constantes
#define BUFF_SIZE 200 //maximum size of char to read from a file
#define DEFAULT_SIZE  1344 //default size of table to allocate: required for a 64-dim subset-sum
#define REALLOC_CST 2 //constant for realloc 2?

/***** Types et Structures *****/

//Représentation d'un mot de grande taille
typedef mpz_t word_t;

typedef unsigned long word64_t; //mot de taille bornée à 64 bits

//Représentation d'une instance subset sum
typedef struct{
    int dim; // dimention du problème
    word_t *elt; // tableau des éléments a_i
    word_t target; // cible t
}subsum_t;

//Représentation d'un mot val=<a,x> et du x correpondant
typedef struct{
    word_t x;
    word_t val;
}pair_t;

//Représentation d'un mot val et l'indice i correspondant dans une table
typedef struct{
    word_t val;
    unsigned long i;
}index_t;


typedef short bool;
#define true 1
#define false 0


/***** Déclaration de Fonctions *****/

/*fonctions util.c*/

//Fonctions d'initalisation et affectation
void word_init(word_t w);
int word_set_str(word_t w, char *c);
int word_init_str(word_t w, char *c);
void word_set_ui(word_t w, unsigned long i);
void word_cpy(word_t w1, word_t w2);
void word_swap(word_t w1, word_t w2);
void word_setZero(word_t w);
//opérateurs logiques
void word_and(word_t res, word_t w1, word_t w2);
void word_xor(word_t res, word_t w1, word_t w2);
void word_bitFlip(word_t w, unsigned long i);
void word_rshift(word_t res, word_t w, unsigned long i);
void word_lshift(word_t res, word_t w, unsigned long i, unsigned long wsize);
void word_low(word_t res, word_t w, unsigned int l);
int word_getBitVal(word_t w, unsigned int i);
//comparaisons
int word_comp(word_t w1, word_t w2);
int word_isNull(word_t w);
//opérations arithmétiques
void word_add_modexp(word_t res, word_t w1, word_t w2, unsigned long n);
void word_addAndMask(word_t res, word_t w1, word_t w2, word_t mask);
//Fonctions d'affichage
void word_print(word_t w);
//Fonctions de libération de la mémoire
void word_clear(word_t w);

/*fonction binary.c*/
//Affichage binaire
void word_dispBinVector(word_t x, int n);
int word_BinVectorSum(word_t res, word_t x1, word_t x2);


/* fonctions basics.c */
//Allocation mémoire et initialisation
subsum_t * subsum_alloc(int n);
int subsum_fromStr(subsum_t *S, char** tab, int n);
int subsum_fromStr2(subsum_t *S, char* a, char *t);
//Lecture
int subsum_readInput(char* target, char ** tab_pt, char * file_name);
//Affichage
void subsum_print(subsum_t *S);
//Dot product with binary vector
void subsum_dotProd(word_t res, subsum_t *S, word_t e);
//libération mémoire
void subsum_free(subsum_t *S);

/* fonctions tuple.c */
//Allocation and initialisation
pair_t * pair_alloc();
pair_t * pair_allocToValue(word_t x, word_t val);
void pair_setValue(pair_t * P, word_t x, word_t val);
//libération mémoire
void pair_free(pair_t *P);




