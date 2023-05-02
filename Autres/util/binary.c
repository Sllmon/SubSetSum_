#include "subsum.h"

/***********************************/
/*   Fonctions binaires             */
/*     sur les word_t              */
/***********************************/

/*Affichage*/
//x: word seen as a vector in {0,1}^n
//vector print [v_0,v_1,...]
void word_dispBinVector(word_t x, int n){
	int b;
	printf("[");
	for(int i = 0; i< n-1; i++){
		b = word_getBitVal(x, i);
		printf("%d,",b);
	}
	b = word_getBitVal(x, n-1);
	printf("%d]\n", b);
	return;
}


/* Somme */
// x1 et x2 deux vecteurs de {0,1}^n
// res: somme des deux si res in {0,1}^n
// retourne 0 si res in {0,1}^n -1 sinon
int word_VectorSum(word_t res, word_t x1, word_t x2){

	// x1+x2 in {0,1}^n iff x1&x2 = 0
	word_and(res, x1, x2);
	if(!word_isNull(res)) //not good vector
		return -1;

	//otherwise x1+x2 = x1 xor x2
	word_xor(res, x1, x2);
	return 0;
}

//i1 et i2: deux unsigned long représentant des vect de {0,1}^(n -sh)
//sh: entier représentant un décalage à gauche.
//n: taille du vecteur
//res: word_t tq res = (i1) + (i2 << sh2) dans {0,1}^n
// rentourne 0 si res dans {0,1}^n -1 sinon
int word_VectorSum_ui(word_t res, unsigned long i1, unsigned long i2, int sh, int n){
	word_t tmp;
	int ok;

	word_init(tmp); // to store i2 << sh2
	word_set_ui(tmp, i2);
	word_lshift(tmp, tmp, sh, n);

	word_set_ui(res, i1); // store i1

	ok = word_VectorSum(res, res, tmp); // sum

	//free memory
	word_clear(tmp);

	return ok;

}





