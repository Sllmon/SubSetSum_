#include "word.h"

/// @brief Fonction qui inverse un nombre binaire, par exemple 100 (4 base 10) -> 001 (1 base 10)
/// @param nb nombre à inverser
/// @param lg nombre de chiffre dans le nombre
unsigned long inverse(unsigned long nb, int lg)
{
    unsigned long mask = 1 << (lg - 1), rep = 0, ok = 0;
    for (int i = 0; i < lg; ++i)
    {
        ok = mask & nb;
        if (ok)
        {
            rep = rep + (1 << i);
        }
        mask = 1 << (lg - 2 - i);
    }
    return rep;
}

/// @brief Fonction qui tri par dénombrement un tableau de pair T dans RES
void triParDenombrement(word *TS, pair *T, pair *RES, unsigned long M, unsigned long tailleT){
    // On crée le tableau de comptage
    unsigned long *C = malloc(sizeof(unsigned long) * M);
    for (unsigned long i = 0; i < M; ++i)
    {
        C[i] = 0;
    }
    for (unsigned long i = 0; i < tailleT; ++i)
    {
        C[mpz_get_ui(T[i].word)] = C[mpz_get_ui(T[i].word)] + 1;
    }
    for (unsigned long i = 1; i < M; ++i)
    {
        C[i] = C[i] + C[i - 1];
    }

    word *TStrie = malloc(sizeof(word) * tailleT);
    for (unsigned long i = 0; i < tailleT; ++i)
    {
        mpz_init(TStrie[i]); // Initialise à 0
    }

    for (unsigned long i = 0; i < tailleT; ++i)
    {
        mpz_set(RES[C[mpz_get_ui(T[i].word)] - 1].word, T[i].word);
        RES[C[mpz_get_ui(T[i].word)] - 1].indexe = T[i].indexe;
        C[mpz_get_ui(T[i].word)] = C[mpz_get_ui(T[i].word)] - 1;
    }

    // On trie TS parrallèlement à RES, les éléments aux mêmes indices
    for (unsigned long i = 0; i < tailleT; ++i)
    {
        mpz_set(TStrie[i], TS[RES[i].indexe]);
    }
    for (unsigned long i = 0; i < tailleT; ++i)
    {
        mpz_set(TS[i], TStrie[i]);
    }

    // Libération de l'espace libre
    liberation(TStrie, tailleT);
    free(C);
}


// Trie rapide tiré du livre Algorithmique de Cormen-Leiseron-Rivest-Stein:
    void permuter(triple *a, triple *b)
    {
        triple tmp;
        tmp.i = a->i;
        tmp.j = a->j;
        mpz_init_set(tmp.word, a->word);

        a->i = b->i;
        a->j = b->j;
        mpz_set(a->word, b->word);

        b->i = tmp.i;
        b->j = tmp.j;
        mpz_set(b->word, tmp.word);
        mpz_clear(tmp.word);
    }

    unsigned long partition(triple *S1, long long first, long long last)
    {
        long long i = first - 1;
        for (int j = first; j < last; j++)
        {
            if (mpz_cmp(S1[j].word, S1[last].word) <= 0)
            {
                i++;
                permuter(&S1[i], &S1[j]);
            }
        }
        permuter(&S1[i + 1], &S1[last]);
        return i + 1;
    }

    void triRapide(triple *S1, long long first, long long last)
    {
        long long q;
        if (first < last)
        {
            q = partition(S1, first, last);
            triRapide(S1, first, q - 1);
            triRapide(S1, q + 1, last);
        }
    }


/// @brief Fonction qui tri par dénombrement un tableau de triple S1
void triParDenombrement2(triple *S1, unsigned long tailleS1){

    // On cherche la valeur max que peut contenir S1
    // unsigned long max = 0;
    // if (WORD_SIZE % 4 == 0)
    // {
    //     // 2^(n+1) * (n/4)
    //     max = (1UL << (WORD_SIZE + 1)) * (WORD_SIZE / 4);
    // }
    // else
    // {
    //     if (WORD_SIZE % 4 == 1)
    //     {
    //         // 2^n * entier_sup(n/4) + 2^n * entier_inf(n/4)
    //         max = (1UL << WORD_SIZE) * (unsigned long)ceil(WORD_SIZE / 4) + (1UL << WORD_SIZE) * (unsigned long)floor(WORD_SIZE / 4);
    //     }
    //     // WORD_SIZE % 4 == 2 ou == 3
    //     else
    //     {
    //         // 2^(n+1) * entier_sup(n/4)
    //         max = (1UL << (WORD_SIZE + 1)) * (unsigned long)ceil(WORD_SIZE / 4);
    //     }
    // }

    // On supose une valeur max que peut contenir S1
    unsigned long max = (1UL << (WORD_SIZE + 1)) * (WORD_SIZE / 4);
    // Nombre de bits utilisés à l'arrondie suppérieur 
    max = (unsigned long)ceil(log2(max));
    unsigned long extraction, x, y;
    // nb de bits à extraire
    extraction =  max/4;
    // 2**2 - 1 base 10 -> 11 base 2
    // permet d'extraire avec un &
    x = (((int)pow(2,extraction))-1) << extraction;
    y = (int)pow(2,extraction);

    // On déclare le tableau de comptage
    unsigned long* C = (unsigned long*) malloc(sizeof(unsigned long)*(y));
    // Tableau des numéros de groupes
    unsigned long* G = (unsigned long*) malloc(sizeof(unsigned long)*tailleS1);
    // Tableau temporaire
    triple *S1T = (triple*) malloc(sizeof(triple)*tailleS1);
    for (unsigned long i = 0; i < (y); i++){
        C[i] = 0;
    }
    for (unsigned long i = 0; i < tailleS1; i++){
        G[i] = 0;
        S1T[i].i = 0;
        S1T[i].j = 0;
        mpz_init(S1T[i].word);
    }

    // On incrémente à chaque apparition
    unsigned long indice = 0;
    for (unsigned long i = 0; i < tailleS1; i++)
    {
        indice = (mpz_get_ui(S1[i].word) & x) >> extraction;
        C[indice] = C[indice] + 1;
        G[i]=indice;
    }

    for (unsigned long i = 1; i < (y); i++){
        C[i] = C[i] + C[i-1];
    }

    for (unsigned long i = 0; i < tailleS1; i++)
    {
        S1T[C[G[i]]-1].i = S1[i].i;
        S1T[C[G[i]]-1].j = S1[i].j;
        mpz_set(S1T[C[G[i]]-1].word,S1[i].word);
        C[G[i]] =  C[G[i]] - 1;
    }
    
    // S1 <- S1T
    for (unsigned long i = 0; i < tailleS1; i++)
    {
       S1[i].i = S1T[i].i;
       S1[i].j = S1T[i].j;
       mpz_set(S1[i].word,S1T[i].word);
    }

    // On libère la mémoire
    for (unsigned long i = 0; i < tailleS1; i++){
        mpz_clear(S1T[i].word);
    }
    free(S1T);
    free(G);
    free(C);
}

// Ancien tri rapide (+ performant)

// void triRapide(triple* S1, unsigned long first, unsigned long last){
//     unsigned long pivot, i, j;
//     if(first < last) {
//         pivot = first;
//         i = first;
//         j = last;
//         while (i < j) {
//             // <=
//             while(mpz_cmp(S1[i].word,S1[pivot].word) <= 0 && i < last){
//                 i++;
//             }
//             // >
//             while(mpz_cmp(S1[j].word,S1[pivot].word) > 0){
//                 j--;
//             }
//             if(i < j) {
//                 permuter(&S1[i], &S1[j]);
//             }
//         }
//         permuter(&S1[pivot], &S1[j]);
//         if(j > 0)
//             triRapide(S1, first, j - 1);
//         triRapide(S1, j + 1, last);
//     }
// }

// Fonction qui permet de renvoyer l'indice de l'emplacement de la première apparition de ot dans le tableau RES
unsigned long dichotomie_pair(unsigned long ot, pair *RES, unsigned long tailleTableauS)
{
    unsigned long milieu /* = ot*tailleTableauS/4*/, debut = 0, fin = tailleTableauS - 1;
    // valeur impossible (max indice : tailleTableauS*tailleTableauS-1)
    unsigned long res = tailleTableauS  + 2;
    unsigned long val;

    while (debut < fin && fin > 0)
    {
        milieu = (unsigned long)(debut + fin) / 2;
        val = mpz_get_ui(RES[milieu].word);

        if (val == ot)
        {
            res = milieu;
            if (milieu == 0)
                break;
            fin = milieu - 1;
        }
        else
        {
            if (ot > val)
            {
                debut = milieu + 1;
            }
            else
            {
                if (milieu == 0)
                    break;
                fin = milieu - 1;
            }
        }
        if (debut == fin)
        {
            if (mpz_get_ui(RES[debut].word) == ot)
            {
                res = debut;
            }
            break;
        }
    }
    return res;
}

unsigned long dichotomie_triple(unsigned long ot, triple *RES, unsigned long tailleTableauS, unsigned long extraction, unsigned long extraction2)
{
    unsigned long milieu /* = ot*tailleTableauS/4*/, debut = 0, fin = tailleTableauS - 1;
    // valeur impossible (max indice : tailleTableauS*tailleTableauS-1)
    unsigned long res = tailleTableauS * tailleTableauS;
    unsigned long val;

    while (debut < fin && fin > 0)
    {
        milieu = (unsigned long)(debut + fin) / 2;
        val = (mpz_get_ui(RES[milieu].word) & extraction) >> extraction2;

        if (val == ot)
        {
            res = milieu;
            if (milieu == 0)
                break;
            fin = milieu - 1;
        }
        else
        {
            if (ot > val)
            {
                debut = milieu + 1;
            }
            else
            {
                if (milieu == 0)
                    break;
                fin = milieu - 1;
            }
        }
        if (debut == fin)
        {
            if (((mpz_get_ui(RES[debut].word) & extraction) >> extraction2) == ot)
            {
                res = debut;
            }
            break;
        }
    }
    return res;
}


// On cherche le premier indice dans tab qui donne ot : tab[indice] = ot avec tab[indice-1] != ot, le tableau tab est trié
// Recherche par la distance
long long RechercheDistance(unsigned long ot, pair *tab, unsigned long taille)
{
    long long i = ot,
                  debut = 0,
                  fin = taille - 1;
    word xprime, x;
    mpz_init_set_ui(x, ot);
    mpz_init(xprime);
    int res;
    long long temp = 0, temp2 =0;
    if(i>fin) i = fin;
    while (debut < fin){
        mpz_set(xprime, tab[i].word);
        res = mpz_cmp(xprime, x);
        if (res == 0){
            mpz_clears(xprime, x, NULL);
            return fun(i, ot, tab, taille);
        }
        else{
            if (res > 0){
                fin = i - 1;
                temp = i;
                i = i - (mpz_get_ui(xprime) - ot);
                if (i < debut){
                    temp2 = (fin-debut)/2;
                    if(temp2 != 0){
                        i = temp - temp2;
                    }
                    else{
                        i = temp - 1;
                    }
                }
            }
            else{
                debut = i + 1;
                temp = i;
                i = i + (ot - mpz_get_ui(xprime));
                if(i>fin){
                    temp2 = (fin-debut)/2;
                   if(temp2 != 0){ //evite de boucler
                        i = temp + temp2;
                   }
                    else{
                        i = temp + 1;
                    }
                }
            }
        }
    }
    if(debut>=0 && debut < taille){
        mpz_set(xprime, tab[debut].word);
        if(mpz_cmp(xprime, x) == 0){
            mpz_clears(xprime, x, NULL);
            return debut;
        }
    }
    if(fin>=0 && fin < taille){
        mpz_set(xprime, tab[fin].word);
        if(mpz_cmp(xprime, x) == 0){
            mpz_clears(xprime, x, NULL);
            return fin;
        }
    }
    mpz_clears(xprime, x, NULL);
    return taille + 2;
}

// On cherche le premier indice qui fourni une valeur équivalente à tab[i]
long long fun(unsigned long i, unsigned long ot, pair *tab, unsigned long taille){
    if (mpz_get_ui(tab[0].word) == ot) return 0;
    while (i >= 1)
    {
        if (mpz_get_ui(tab[i - 1].word) != ot){
            return i;
        }
        i--;
    }
    return taille + 2;
}

// Merge des deux premiers tableaux :
unsigned long parcoursT1S(unsigned long om, word tmp_w, unsigned long M, unsigned long i, word *T1S){
    mpz_set_ui(tmp_w, om);
    mpz_sub(tmp_w, tmp_w, T1S[i]);
    mpz_mod_ui(tmp_w, tmp_w, M);
    return mpz_get_ui(tmp_w);
}

void parcoursT2S(unsigned long ot, pair *RES1, word *T1S, word *T2S, unsigned long tailleR1, unsigned long M, unsigned long i, triple **S1, unsigned long *tailleS1, unsigned long *nbReallocS1){
    // On cherche l'indice de l'emplacement de la première apparition de ot dans le tableau RES

    //unsigned long j = dichotomie_pair(ot,RES1,tailleR1);
    // On essaie d'améliorer le temps de recherche dichotomique par une recherche par la distance
    unsigned long j = RechercheDistance(ot, RES1, tailleR1);    

    // Pour tout les éléments ot dans RES1 faire
    while (j < tailleR1)
    {
        if (mpz_get_ui(RES1[j].word) == ot)
        {
            // Si on dépasse la mémoire alloué à S1 alors on réaloue de la mémoire
            if ((*tailleS1) >= (M + M * (*nbReallocS1)))
            {
                (*nbReallocS1)++;
                *S1 = (triple *)realloc(*S1, sizeof(triple) * (M + (M * (*nbReallocS1))));
                for (unsigned long k = M * (*nbReallocS1); k < (M + M * (*nbReallocS1)); ++k)
                {
                    mpz_init((*S1)[k].word);
                }
            }
            // On ajoute dans S1 les solutions
            (*S1)[(*tailleS1)].i = i;
            (*S1)[(*tailleS1)].j = RES1[j].indexe;
            mpz_add((*S1)[(*tailleS1)++].word, T2S[j], T1S[i]);
        }
        else
        {
            break;
        }
        j++;
    }
}

/// @brief Fonction qui permet de merge les deux premiers tableaux T1S et T2S et stocke le résultat dans S1 trié
triple *join(triple *S1, word *T1S, unsigned long tailleT1, word *T2S, unsigned long tailleT2, pair *RES1, unsigned long M, unsigned long om, unsigned long *tailleS1, unsigned long *nbReallocS1)
{
    word tmp_w;
    mpz_init(tmp_w);
    unsigned long ot;

    for (unsigned long i = 0; i < tailleT1; i++)
    {
        ot = parcoursT1S(om, tmp_w, M, i, T1S);
        // retourne le premier indice recherché (càd dont la valeur de sa cellule = ot)
        parcoursT2S(ot, RES1, T1S, T2S, tailleT2, M, i, &S1, tailleS1, nbReallocS1);
    }

    // On trie le tableau de solutions S1
    
    //triRapide(S1, 0, (*tailleS1)-1);
    // Tri plus rapidement que le tri rapide
    // On ne trie pas complétement le tableau, on trie à peu près sur l'avant dernier quart des bits de poid faible : [__-_] (sur -)
    triParDenombrement2(S1, *tailleS1);

    // Liération de l'espace libre
    mpz_clear(tmp_w);
    return S1;
}

// Merge des deux derniers tableaux :

// Fonction qui permet de merge les deux derniers tableaux T3S et T4S et retourne la solution de l'algorithme attendu
ListeSol solver(ListeSol SOL, triple *S1, word *T3S, word *T4S, pair *RES2, unsigned long nbElt1, unsigned long nbElt2, unsigned long nbElt3, unsigned long nbElt4, word TargetSum, unsigned long M, unsigned long om, unsigned long *tailleS1, word *tabGray)
{
    word ol, tmp_w, Tprime, tmp1;
    mpz_inits(ol, tmp_w, Tprime, tmp1, NULL);

    unsigned long taille3 = 1UL << nbElt3;
    unsigned long taille4 = 1UL << nbElt4;

    for (unsigned long i = 0; i < taille3; i++)
    {
        mpz_set(ol, T3S[i]);
        mpz_set_ui(tmp_w, om);

        // ot = (TargetSum-om-T3S[i]) % M
        mpz_sub(tmp_w, TargetSum, tmp_w);
        mpz_sub(tmp_w, tmp_w, ol);
        mpz_mod_ui(tmp_w, tmp_w, M);
        unsigned long ot = mpz_get_ui(tmp_w);

        // On cherche l'indice de l'emplacement de la première apparition de ot dans le tableau RES
        unsigned long j = dichotomie_pair(ot, RES2, taille4);
        // Des tests ont montré qu'en pratique le tri dichotomique est ici plus rapide
        //unsigned long j  = RechercheDistance(ot,RES2,taille4);

        while (j < (taille4))
        {
            if (mpz_get_ui(RES2[j].word) == ot)
            {
                if (RES2[j].indexe < taille4)
                { // Condition utile dans le cas où WORD_SIZE n'est pas divisible par 4, permet d'éviter l'apparition de solutions impossible
                    mpz_sub(tmp1, TargetSum, ol);
                    mpz_sub(Tprime, tmp1, T4S[j]);

                    //unsigned long k  = RechercheDistance2(mpz_get_ui(Tprime),S1,*tailleS1);
                    //unsigned long k = dichotomie_triple(mpz_get_ui(Tprime), S1, M);
                   
                    // On cherche par dichotomie 
                        unsigned long max = (1UL << (WORD_SIZE + 1)) * (WORD_SIZE / 4);
                        // Nombre de bits utilisés à l'arrondie suppérieur 
                        max = (unsigned long)ceil(log2(max));
                        unsigned long extraction, x, y;
                        // nb de bits à extraire
                        extraction =  max/4;
                        // 2**2 - 1 base 10 -> 11 base 2
                        // permet d'extraire avec un & 
                        x = (((int)pow(2,extraction))-1) << extraction;
                        y = (int)pow(2,extraction);

                        unsigned long indice = (mpz_get_ui(Tprime) & x) >> extraction;

                        // Une dicho qui trouve le premier indice, d'un tableau trié sur l'avant dernier quart des bits de poid faible : [__-_] (sur -)
                        unsigned long k = dichotomie_triple(indice, S1, M,x,extraction);

                    while (k < (*tailleS1))
                    {
                        if (mpz_cmp(S1[k].word, Tprime) == 0)
                        {
                            solution s;
                            s.i = inverse(mpz_get_ui(tabGray[S1[k].i]), nbElt1);
                            s.j = inverse(mpz_get_ui(tabGray[S1[k].j]), nbElt2);
                            s.k = inverse(mpz_get_ui(tabGray[i]), nbElt3);
                            s.l = inverse(mpz_get_ui(tabGray[RES2[j].indexe]), nbElt4);
                            SOL = AjouterListeSol(s, SOL);
                        }
                        else
                        {
                            //new
                            if(((mpz_get_ui(S1[k].word)& x) >> extraction) != indice) break;
                            //break;
                        }
                        k++;
                    }
                }
            }
            else
            {
                break;
            }
            j++;
        }
    }
    mpz_clears(ol, tmp_w, Tprime, tmp1, NULL);
    return SOL;
}

// Fonction qui permet d'afficher une idée approximative de l'avancement du processus en cours
int chargement(unsigned long pos, unsigned long max, int *anc)
{
    int p = floor((pos * 100) / max);
    if (p > *anc)
    {
        //  system("clear");
        printf("\nETAT : %d %%", p);
    }
    return p;
}

/// @brief Algorithme 3 Modular 4-Way merge routine
/// @param T1S Tableau contenant environs 2^WORD_SIZE sommations. Premier 1/4 des élements du vecteur a.
/// @param nbTableauS1 Nombre d'éléments utilisés pour les sommations obtenus dans T1S
/// @param T2S Tableau contenant environs 2^WORD_SIZE sommations. Deuxième 1/4 des élements du vecteur a.
/// @param nbTableauS2 Nombre d'éléments utilisés pour les sommations obtenus dans T2S
/// @param T3S Tableau contenant environs 2^WORD_SIZE sommations. Troisième 1/4 des élements du vecteur a.
/// @param nbTableauModular_mergeS3 Nombre d'éléments utilisés pour les sommations obtenus dans T3S
/// @param T4S Tableau contenant environs 2^WORD_SIZE sommations. Dernier 1/4 des élements du vecteur a.
/// @param nbTableauS4 Nombre d'éléments utilisés pour les sommations obtenus dans T4S
/// @param TargetSum Target sum
/// @param Sol Liste des solutions (vide).
/// @param time
/// @param time_spent
/// @param nbTime
ListeSol Modular_merge(word *T1S, short nbTableauS1, word *T2S, short nbTableauS2, word *T3S, short nbTableauS3, word *T4S, short nbTableauS4, word TargetSum, word *tabGray, ListeSol SOL, double *time, double *time_spent, short *nbTime)
{
    // Variables pour le calcul du temps d'exécution
    clock_t begin, end;

    unsigned long M = (1ULL << (WORD_SIZE / 4));

    // Si WORD_SIZE n'est pas divisible par 4
    if (WORD_SIZE % 4 != 0)
    {
        M = (1UL << (WORD_SIZE / 4 + 1)); // Permet l'utilisation du programme sur une dimension non divisible par 4
    }

    // On crée un tableau de pair, équivalent à T2S mais modulo M
    pair *T2S2 = malloc(sizeof(pair) * (1UL << nbTableauS2));
    for (unsigned long i = 0; i < (1UL << nbTableauS2); ++i)
    {
        mpz_init(T2S2[i].word);
        mpz_mod_ui(T2S2[i].word, T2S[i], M);
        T2S2[i].indexe = i;
    }

    // On crée un tableau de pair, équivalent à T4S mais modulo M
    pair *T4S2 = malloc(sizeof(pair) * (1UL << nbTableauS4));
    for (unsigned long i = 0; i < (1UL << nbTableauS4); i++)
    {
        mpz_init(T4S2[i].word);
        mpz_mod_ui(T4S2[i].word, T4S[i], M);
        T4S2[i].indexe = i;
    }

    // On crée les tableaux RES1 (T4S2 trié) et RES2 (T2S2 trié)
    pair *RES1 = malloc(sizeof(pair) * (1UL << nbTableauS2));
    for (unsigned long i = 0; i < (1UL << nbTableauS2); ++i)
        mpz_init(RES1[i].word);
    pair *RES2 = malloc(sizeof(pair) * (1UL << nbTableauS4));
    for (unsigned long i = 0; i < (1UL << nbTableauS4); ++i)
        mpz_init(RES2[i].word);

    // On va ici trier T2S2 (qui devient RES1) et T4S2 (qui devient RES2), mais également trier T2S et T4S parallèlement
    begin = clock();
    triParDenombrement(T2S, T2S2, RES1, M, (1UL << nbTableauS2));
    liberationPair(T2S2, (1UL << nbTableauS2));
    triParDenombrement(T4S, T4S2, RES2, M, (1UL << nbTableauS4));
    liberationPair(T4S2, (1UL << nbTableauS4));
    end = clock();
    (*time_spent) += (double)(end - begin) / CLOCKS_PER_SEC;
    time[(*nbTime)++] = (*time_spent);

    // Variables utile à S1
    unsigned long tailleS1 = 0, nbReallocS1 = 0;

    // Initialisation de S1
    triple *S1 = malloc(M * sizeof(triple));
    for (unsigned long i = 0; i < M; ++i)
    {
        mpz_init(S1[i].word);
    }

    // Variable pour l'affichage du pourcentage du chargement du programme
    int anc = 1;

    end = clock();
    for (unsigned long om = 0; om < M; om++)
    {
        tailleS1 = 0;
        // Merge de T1S et T2S
        S1 = join(S1, T1S, 1UL << nbTableauS1, T2S, 1UL << nbTableauS2, RES1, M, om, &tailleS1, &nbReallocS1);
        if (S1 == NULL)
        {
            printf("\nErreur");
            return NULL;
        }
        // Merge de T3S et T4S
        SOL = solver(SOL, S1, T3S, T4S, RES2, nbTableauS1, nbTableauS2, nbTableauS3, nbTableauS4, TargetSum, M, om, &tailleS1, tabGray);
        // "barre" de chargement
        if (AFFICHAGE)
            anc = chargement(om, M, &anc);
    }
    end = clock();
    (*time_spent) += (double)(end - begin) / CLOCKS_PER_SEC;
    time[(*nbTime)++] = (*time_spent);

    // Libération de la mémoire
    liberationPair(RES1, 1UL << nbTableauS2);
    liberationPair(RES2, 1UL << nbTableauS4);
    for (unsigned long i = 0; i < (nbReallocS1 + 1) * M; i++)
    {
        mpz_clear(S1[i].word);
    }
    free(S1);
    return SOL;
}
