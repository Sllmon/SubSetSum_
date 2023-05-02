#include "subsum.h"

/***********************************/
/*   Fonctions de base             */
/*     sur les word_t              */
/***********************************/


/****** Initialisation et affectation ******/

//alloue la mémoire de w  et l'initialise à 0.
void word_init(word_t w){
    mpz_init(w);
    if(w == NULL){
	fprintf(stderr, "Error in word_init: initialisation failed\n");
    }
    return;
}

//Entrée: mot w et une chaîne c représentant un entier en base 10
//affecte à w la valeur numérique de c
//retourne 0 si ça s'est bien passé et -1 sinon
int word_set_str(word_t w, char *c){
    int val;
    val = mpz_set_str(w, c, 10);
    if(val == -1)
        fprintf(stderr, "Error in word_set_str: affectation failed. string might be in wrong format\n");
    return val;
}

//Entrée: mot w et une chaîne c représentant un entier en base 10
//Alloue la mémoire pour w l'initialise avec la valeur numérique de c
//retourne 0 si ça s'est bien passé et -1 sinon
int word_init_str(word_t w, char *c){
    int val;
    val = mpz_init_set_str (w, c, 10);
    if(val == -1)
	fprintf(stderr,"Error in word_init_str: initialisation failed. string might be in wrong format\n");
    return val;
}

//Entrée: mot w et un unsigned long i.
//convertit i dans w.
void word_set_ui(word_t w, unsigned long i){
    mpz_set_ui(w, i);
    return;
}

//Copie la valeur de w2 dans w1
void word_cpy(word_t w1, word_t w2){
    mpz_set(w1, w2);
    return;
}

//échange les valeurs de w1 et w2
void word_swap(word_t w1, word_t w2){
    mpz_swap(w1, w2);
    return;
}

//ré-initialise w à zéro
void word_setZero(word_t w){
  mpz_set_ui(w, 0);
}

/****** Affichage ******/


//affiche un mot
void word_print(word_t w){
    gmp_printf("%Zd\n", w);
    return;
}

/****** Opérations logiques ******/
//affecte à res la valeur de w1&w2
void word_and(word_t res, word_t w1, word_t w2){
    mpz_and(res, w1, w2);
    return;
}

void word_and_ui(word_t res, word_t w1, word_t w2){
    mpz_and(res, w1, w2);
    return;
}

//affecte à res la valeur de w1 xor w2
void word_xor(word_t res, word_t w1, word_t w2){
    mpz_xor(res, w1, w2);
    return;
}

//flip bit i in w
void word_bitFlip(word_t w, unsigned long i){
    mpz_combit(w, i);
    return;
}

//right shift word_t w by i. result put in res.
void word_rshift(word_t res, word_t w, unsigned long i){

  mpz_fdiv_q_2exp(res, w, i); //done with bitwise shift. 

  return;
}

//left shift word_t w by i. result put in res. 
//wsize: size of res in bits (mandatory since left shift would increase the size of the data otherwise)
void word_lshift(word_t res, word_t w, unsigned long i, unsigned long wsize){

  mpz_mul_2exp(res, w, i); //done with bitwise shift. shift w by i position to the left.
  mpz_tdiv_r_2exp(res,res, wsize); // truncate the result at wsize bits.

  return;
}

//get the l lowest bit of w in res.
//same as computing res = w mod 2**l
void word_low(word_t res, word_t w, unsigned int l){
  mpz_tdiv_r_2exp(res,w, l);
  return;
}

//get the value (1 or 0) of bit i of w
int word_getBitVal(word_t w, unsigned int i){
  return  mpz_tstbit(w, i);
}


//retourne 1 si w1 > w2, 0 si w1=w2, -1 si w1<w2.
int word_comp(word_t w1, word_t w2){
   return mpz_cmp(w1, w2);
}

//return 1 if w=0 and 0 otherwise
int word_isNull(word_t w){
  return (mpz_cmp_ui(w,0)==0);
}

/****** Arithmétique ******/

//affecte à res la valeur de w1+w2 mod 2**n
// /!\ possiblement trop long. Privilégier la fonction word_addAndMask en pratique.
void word_add_modexp(word_t res, word_t w1, word_t w2, unsigned long n){
    
    mpz_add(res, w1, w2); //res<-w1+w2
    word_low(res, res, n); //res <- res mod 2**n

   return;
}

//affecte à res la valeur de (w1+w2)&mask  
void word_addAndMask(word_t res, word_t w1, word_t w2, word_t mask){
  mpz_add(res, w1, w2); //res<- w1+w2
  word_and(res, res, mask);// res <- res & mask

  // dans le cas où mask = 0000001111...1111, ça fait la même chose que word_add_modexp

  return; 
}



/****** Liberation mémoire ******/

//clear word at the end.
void word_clear(word_t w){
    if(w != NULL){
        mpz_clear(w);
    }
    return;
}

