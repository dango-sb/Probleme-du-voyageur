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
        fprintf(stderr, "Usage : %s -f fichier.tsp -m [nn|rw]\n", argv[0]);
        return 1;
    }

    char path[256];
    snprintf(path, sizeof(path), "../JDD/ALL_tsp/%s", argv[2]);

    FichierTSP* tsp = malloc(sizeof(FichierTSP));
    if (!tsp) {
        fprintf(stderr, "Erreur malloc tsp\n");
        return 1;
    }
    readTSP(path, tsp);

    FichierTour* tour = NULL;
    if (strcmp(argv[4], "nn") == 0) {
        printf("Méthode : plus proche voisin\n");
        tour = plus_proche_voisin(tsp);
    } 
    else if (strcmp(argv[4], "rw") == 0) {
        printf("Méthode : random walk\n");
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

    int longueur_init = longueur_tournee(tsp, tour, distance);
    printf("Distance initiale : %d\n", longueur_init);

    two_opt(tsp, tour, distance);

    int longueur_finale = longueur_tournee(tsp, tour, distance);
    printf("Distance après 2-opt : %d\n", longueur_finale);

    if (longueur_finale < longueur_init)
        printf("Amélioration : %d -> %d\n", longueur_init, longueur_finale);
    else
        printf("Aucun gain trouvé \n");

    printf("Tournée finale : [");
    for (int i = 0; i < tour->dimension; i++) {
        if (i == tour->dimension - 1)
            printf("%d]\n", tour->nodes[i]);
        else
            printf("%d, ", tour->nodes[i]);
    }

    return 0;
}
