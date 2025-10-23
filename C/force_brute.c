#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <signal.h>
#include "force_brute.h"
#include "data.h"
#include "fonctions_calcul.h"


/* Drapeau global pour intercepter Ctrl-C */
volatile sig_atomic_t interrompre = 0;


void handler_signal(int sig) {
    (void)sig;
    interrompre = 1;
}

void echanger(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Génération récursive de toutes les permutations, calcul des longueurs, mise à jour du meilleur/pire */
void permuter(int *perm, int debut, int fin,FichierTSP *tsp, FichierTour *tour_actuel,EdgeType type,FichierTour *tour_best, int *best_len,FichierTour *tour_worst, int *worst_len,int (*distance)(Node,Node)){
    if (interrompre) return;

    if (debut == fin) {
        
        for (int i = 0; i < tsp->dimension; i++){
            tour_actuel->nodes[i] = perm[i];
        }
        tour_actuel->nodes[tsp->dimension] = -1;


        int longueur = longueur_tournee(tsp, tour_actuel, distance);

        if (longueur < *best_len) {
            *best_len = longueur;
            memcpy(tour_best->nodes, tour_actuel->nodes, sizeof(int) * (tsp->dimension + 1));
        }

        if (longueur > *worst_len) {
            *worst_len = longueur;
            memcpy(tour_worst->nodes, tour_actuel->nodes, sizeof(int) * (tsp->dimension + 1));
        }
        return;
    }

    for (int i = debut; i < fin; i++) {
        echanger(&perm[debut], &perm[i]);
        permuter(perm, debut + 1, fin, tsp, tour_actuel, type, tour_best, best_len, tour_worst, worst_len,distance);
        echanger(&perm[debut], &perm[i]);
        if (interrompre) return;
    }
}


FichierTour* force_brute(FichierTSP *tsp, EdgeType type){
    signal(SIGINT, handler_signal);

    int n = tsp->dimension;
    int perm[n];
    for (int i = 0; i < n; i++) perm[i] = i;  

    FichierTour *tour_actuel = malloc(sizeof(FichierTour));
    FichierTour *tour_best   = malloc(sizeof(FichierTour));
    FichierTour *tour_worst  = malloc(sizeof(FichierTour));

    tour_actuel->dimension = n;
    tour_best->dimension = n;
    tour_worst->dimension = n;
    tour_actuel->nodes = malloc(sizeof(int) * (n + 1));
    tour_best->nodes   = malloc(sizeof(int) * (n + 1));
    tour_worst->nodes  = malloc(sizeof(int) * (n + 1));

    for (int i = 0; i<n;i++){
        tour_best->nodes[i] = i;
    }

    tour_best->nodes[n] = -1;

    int (*distance)(Node,Node);
    switch(type){
        case EUC_2D:
            distance = distance_euc_2d;
            break;
        case ATT:
            distance = distance_att;
            break;
        case GEO:
            distance = distance_geo;
            break;
    }

    int best_len = longueur_tournee(tsp, tour_best, distance);
    int worst_len = 0;

    printf("Lancement de l’algorithme de force brute sur %d villes...\n", n);

    permuter(perm, 1, n, tsp, tour_actuel, type, tour_best, &best_len, tour_worst, &worst_len,distance);

    if (interrompre) {
        printf("\n--- Interruption (Ctrl-C) ---\n");
        printf("Meilleure tournée trouvée jusqu’ici (longueur %d) : ", best_len);
        for (int i = 0; i < n; i++) printf("%d ", tour_best->nodes[i] + 1);
        printf("\nSouhaitez-vous continuer (o/n) ? ");
        char rep = getchar();
        if (rep == 'o' || rep == 'O') {
            interrompre = 0;
            force_brute(tsp, type); 
            return NULL;
        } else {
            printf("Arrêt demandé.\n");
        }
    }
    
    printf("Meilleur tour longueur %d :\n",best_len);
    printf("[");
    for(int p=0;p<tour_best->dimension-1;p++){
        printf("%d, ",tour_best->nodes[p]);
    }
    printf("%d]\n",tour_best->nodes[tour_best->dimension-1]);

    printf("Pire tour longueur %d :\n",worst_len);
    printf("[");
    for(int p=0;p<tour_worst->dimension-1;p++){
        printf("%d, ",tour_worst->nodes[p]);
    }
    printf("%d]\n",tour_worst->nodes[tour_worst->dimension-1]);
    return tour_best;

    free(tour_actuel->nodes);
    //free(tour_best->nodes);
    free(tour_worst->nodes);
    free(tour_actuel);
    //free(tour_best);
    free(tour_worst);
}

 


