#include "subsum.h"

/***********************************/
/*   Fonctions de base             */
/*     sur les subsum_t            */
/***********************************/


/****** Initialisation et affectation ******/

//allocation d'une subsum_t de dimension n
//by default every word is set to 0
subsum_t * subsum_alloc(int n){
subsum_t *S;

    S = malloc(sizeof(subsum_t));
    if(S==NULL){
        fprintf(stderr, "Subsum allocation failed");
        return NULL;
    }
    
    S->dim = n;
    S->elt = malloc(n  * sizeof(word_t));
    if(S->elt== NULL){
        fprintf(stderr, "Subsum allocation failed");
        return NULL;
    }
    
//init table of elements
    for(int i = 0; i< n; i++){ 
      word_init(S->elt[i]);
    }
    
    word_init(S->target); //default value = 0
    return S;
}

//initialise une subsum_t à partir d'un tableau de chaîne de n+1 caractères.
//le dernier élément du tableau est la target.
//retourne 0 si succès, -1 si échec
int subsum_fromStr(subsum_t *S, char** tab, int n){
   word_t *Elt;
   int i, res = 1;
   assert(n == S->dim); //OSQ n dimention du problème.

   Elt = S->elt;
   for(i = 0; i < n; i++){
     res &= word_set_str(Elt[i], tab[i])+1; //res <- 1 si succès 0 sinon
   }
   res &= word_set_str(S->target, tab[i]); //dernier élément = target
   return res;
}

//initialise une subsum_t à partir de deux chaines a et t.
//a: n entiers en base 10 séparés par des "," représentant les éléments pb
//t: 1 entier en base 10 (la cible)
//retourne 0 si succès, -1 si échec
int subsum_fromStr2(subsum_t *S, char* a, char *t){
   word_t *Elt;
   int i=0, res = 1;
   const char* delim = ",";  // delimiter "," character
   char* token;
   char* cp = strdup(a);

   Elt = S->elt;
 

    if (cp == NULL) {
        fprintf(stderr, "Error in subsum_fromstr2: dup failed");
        return -1;
    }

    token = strtok (cp, delim);
    while (token != NULL) {
        res &= word_set_str(Elt[i], token)+1; //res <- 1 si succès 0 sinon
        token = strtok (NULL, delim);
 	i++;
    }

   res &= word_set_str(S->target, t); //t = target


 //libération de la mémoire additionnelle
  free(cp);   
  return res;
}

//lit les données du fichier file_name et stocke les valeurs des ai dans la chaine tab
//et la valeur de la cible dans la chaine target
//la dernière case du tableau correspond à la target
//Retourne -1 si le fichier n'est pas valide
//Retourne la valeur de la dimention sinon
int subsum_readInput(char* target, char ** tab_pt, char * file_name){
 FILE * input;
 int n, i, bufl, len = DEFAULT_SIZE;
 char last;
 char buffer[BUFF_SIZE];
 char *tab;

 tab = *tab_pt; // pointer needed for reallocation

    
    input = fopen( file_name, "r" );
    if ( input == NULL ) {
        fprintf(stderr, "Error in readInput: Cannot open file %s\n", file_name);
        return -1;
    }
    
    //Get the first line: dimension of the pb
    fgets(buffer, BUFF_SIZE, input); // at most 4 < BUFF_SIZE char: line complete.
    sscanf(buffer, "%d", &n); // store the value inside n
 
    //check dimension value
    if(n<= 0){
        fprintf(stderr, "Error in readInput: invalid dimension n = %d\n",n);
        return -1;
    }

    i = 0; // iterator over table tab initalised to 0

    last = 'a'; //arbitrary char != \n

    //Get the second line: the ai
    while ( last != '\n') {
        fgets( buffer, BUFF_SIZE, input );
        bufl = strlen(buffer); //get the actual length of the buffer
        if(i + bufl >= len){
            //Need to realloc memory
            tab = realloc( tab, len + REALLOC_CST * bufl); //CST to avoid realloc at each step.
            if(tab == NULL){
                fprintf(stderr, "Error in readInput: realloc failed!\n");
                return -1;
            }
            len += REALLOC_CST*bufl; // update len
        }
        //copy buffer at the end of tab.
        for(int j = 0; j < bufl; j++){
            tab[i] = buffer[j];
            i++;
        }
        last = tab[i-1]; //get last char of the string. if \n end of line reached. 
    }

    tab[i-1] = '\0'; //last character of the string
    //tab_new= (char*)realloc(tab, strlen(tab)+1); //Resize tab
    *tab_pt =tab;

    //Get the last line: the target
    fgets(buffer, BUFF_SIZE, input); // should be < BUFF_SIZE char: line complete.
    strcpy(target, buffer);


    //Close file
    fclose( input );
       
    return n;

}


//Affichage d'une subset_sum
void subsum_print(subsum_t *S){
word_t *Elt;
int n = S->dim;

Elt = S->elt; 

 for(int i = 0; i<n-1; i++){
   gmp_printf("%Zd, ", Elt[i]);
 }
 gmp_printf("%Zd\n", Elt[n-1]);
 gmp_printf("%Zd\n", S->target);

  return;
}

//Entrée: subsum S, vecteur binaire e (word_t)
//e= (e_0, .., e_n-1) avec e_0 bit de poids faible de e et ainsi de suite.
//Sortie: produit scalaire <a,e> mod 2**n (word_t) avec
//a = S->elt, n = S->dim. resultat dans res.
void subsum_dotProd(word_t res, subsum_t *S, word_t e){
 word_t *Elt;
 int n = S->dim;
 
 Elt = S->elt;

 //initialise res à zéro
 word_setZero(res);
 
 //main loop: slow?
 for(int i = 0; i < n; i++){
   if(word_getBitVal(e, i)){
     word_add_modexp(res, res, Elt[i], n);
   }
 }
 
 return;
 
}

//Free subsum_t
void subsum_free(subsum_t *S){
word_t * Elt;
    if(S!=NULL){
      Elt = S->elt;
      if(Elt != NULL){
        for(int i= 0; i< S->dim; i++){
         word_clear(Elt[i]);
	}
       free(Elt);
      }
     word_clear(S->target);
     free(S);

    }
}
