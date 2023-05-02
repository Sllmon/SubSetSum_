#include "../util/subsum.h"

int main(){
 word_t w, a, b, c, mask;
 int test, failed =0;

//Test fonction word_init
 word_init(w);
 word_init(b);
 word_init(c);
 word_init(mask);

//Test fonction word_init_str
 word_init_str(a, "123456789");

//Test fonction word_set_str
 word_set_str(w, "921212121");

//Test fonction word_cpy
 word_cpy(b, a);

//affichage
 word_print(a);
 word_print(w);
 word_print(b);

//initialisation du mask à 2**29 -1 
for(int i = 0; i< 29; i++){
    word_bitFlip(mask, i);
}

//test add_modexp
  word_add_modexp(b, a, w, 29); //b<-a+w mod 2**29
 
//test addAndMask
  word_addAndMask(c, a, w, mask); //c<-(a+w)&mask
 

//Normalement b=c vérifions en calculant le xor
 word_xor(c,b,c); //permet de vérifier également le xor
  
 if(!word_isNull(c)){
   fprintf(stderr, "Test function addAndMask failed!\n");
   failed++;
 }


 //En utilisant les fonctions gmp
 mpz_add(c, a, w); //c<-a+w
 mpz_ui_pow_ui(a, 2, 29); //a <-2**29
 mpz_mod(c, c, a); // c<- c mod a

 if(word_comp(b,c)!=0){
   fprintf(stderr, "Test function add_modexp failed!\n");
   failed++;
 }

//Shift test
 word_set_str(w, "16");
 word_print(w);
 word_rshift(c, w, 1);
 word_print(c);
 word_lshift(c, w, 2, 10);
 word_print(c);

 //test getBitVal
 test = word_getBitVal(w, 4);
 if(!test){
   fprintf(stderr, "Test function getBitVal failed!\n");
   failed++;
 } 


//Test fonction free
 word_clear(a);
 word_clear(w);
 word_clear(b);
 word_clear(c);
 word_clear(mask);

fprintf(stderr, "Number of failed test: %d\n", failed);

}
