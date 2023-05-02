#include "../util/subsum.h"

int main(){
 subsum_t *S, *T, *U;
 word_t res, e;
 char **tab;
 char *Ai, *t;
 int dim, test, fail = 0;

 //allocation
 S = subsum_alloc(10);
 fprintf(stderr, "Alloc ok!\n");

 T = subsum_alloc(10);

 tab=malloc(11*sizeof(char*));


 //allocation de res et e
 word_init(res);
 word_init(e);
 

 //initialisation de e
 for(int i = 0; i <10; i+=2){
  word_bitFlip(e, i);
 }
 //e = 1010101010


 /* Exemple à la main pour tester  fromStr*/
 tab[0] = "576"; tab[1] = "277"; tab[2] = "210"; tab[3]="647"; tab[4]="334";
 tab[5] = "660"; tab[6] = "461"; tab[7] = "518"; tab[8]="632"; tab[9]="177";
 tab[10]= "165"; // 165 = 576+210+334+461+632


//initialisation
 test = subsum_fromStr(S, tab, 10);
 if(test==0){
  fprintf(stderr, "Initialisation ok!\n");
 }
 else{
  fprintf(stderr, "Initialisation failed\n");
  return -1;
 }

/* Même exemple avec fromStr2 */
 test = subsum_fromStr2(T, "576,277,210,647,334,660,461,518,632,177", "165");
 if(test==0){
  fprintf(stderr, "Initialisation ok!\n");
 }
 else{
  fprintf(stderr, "Initialisation failed\n");
  return -1;
 }

 /*Autre exemple à partir d'un fichier*/
 //lecture fichier -> dispatch dans tableaux
 Ai = (char*)malloc(DEFAULT_SIZE);
 t = (char*)malloc(10); //upper- bound
 dim = subsum_readInput(t, &Ai, "exemple.txt");
 if(dim == -1){
    fprintf(stderr, "ReadInput failed\n");
    fail++;
    return -1;
}else{
    U = subsum_alloc(dim); //alloc bonne dim
    test = subsum_fromStr2(U, Ai, t); //dispatch à partir de tableaux
    if(test==0){
        fprintf(stderr, "Initialisation ok!\n");
    }else{
        fprintf(stderr, "Initialisation failed\n");
        return -1;
  } 
 }
printf("%s: len %ld\n", Ai, strlen(Ai));

 //free un-necessary variables
free(Ai);
free(t);
 //dot-prod test:
 subsum_dotProd(res, S, e);
 if(word_comp(res, S->target) != 0){
  fprintf(stderr, "Dot_product failed\n");
  word_print(res);
  fail++;
 }
 
fprintf(stderr, "Number of test that failed %d\n", fail);

 //Test Affichage
 subsum_print(S);
 subsum_print(T);
 subsum_print(U);
 
 //free memory
 free(tab);
 subsum_free(U);
 subsum_free(S);
 subsum_free(T);
 word_clear(res);
 word_clear(e);

}
