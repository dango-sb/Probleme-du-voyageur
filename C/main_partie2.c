#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "fonctions_calcul.h"
#include "demi_matrice.h"
#include "plus_proche_voisin.h"
#include "random_walk.h"
#include "2opt.h"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage : %s -f fichier.tsp -m [nn|rw|2optnn|2optrw]\n", argv[0]);
        return 1;
    }

    printf("Tour ; ");

    char path[256];
    snprintf(path, sizeof(path), "%s", argv[2]);
    printf("%s ; ", argv[2]);  // fichier 
    FichierTSP* tsp = malloc(sizeof(FichierTSP));
    if (!tsp) {
        fprintf(stderr, "Erreur malloc tsp\n");
        return 1;
    }
    readTSP(path, tsp);

    FichierTour* tour = NULL;
    if (strcmp(argv[4], "nn") == 0) {
        printf("nn ; ");
        tour = plus_proche_voisin(tsp);
    } 
    else if (strcmp(argv[4], "rw") == 0) {
        printf("rw ; ");
        tour = random_walk(tsp);
    } 
    else if (strcmp(argv[4], "2optnn") == 0) {
        printf("2optnn ; ");
        tour = plus_proche_voisin(tsp);
    } 
    else if (strcmp(argv[4], "2optrw") == 0) {
        printf("2optrw ; ");
        tour = random_walk(tsp);
    } 
    else {
        fprintf(stderr, "Option algo invalide\n");
        free(tsp);
        return 1;
    }

    int (*distance)(Node, Node);
    switch (tsp->edge_type) {
        case EUC_2D:
            distance = distance_euc_2d;
            break;
        case ATT:
            distance = distance_att;
            break;
        case GEO:
            distance = distance_geo;
            break;
        default:
            fprintf(stderr, "Type de distance non supporté\n");
            return 1;
    }
    printf("0.0 ; ");
    int longueur_init = longueur_tournee(tsp, tour, distance);
    if ((strcmp(argv[4], "rw") == 0) || (strcmp(argv[4], "nn") == 0))
    printf("%d ; ", longueur_init);     //sans 2opt

    

    if ((strcmp(argv[4], "2optrw") == 0) || (strcmp(argv[4], "2optnn") == 0))
    {
        two_opt(tsp, tour, distance);
        int longueur_finale = longueur_tournee(tsp, tour, distance);
        printf("%d ; ", longueur_finale);   //apres 2opt
    }
    
    printf("[");
    for (int i = 0; i < tour->dimension; i++) {
        if (i > 0) printf(",");
        printf("%d", tour->nodes[i]+1);
    }
    printf("] ; \n");

    return 0;
}
