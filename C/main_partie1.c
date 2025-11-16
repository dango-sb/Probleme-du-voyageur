#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "data.h"
#include "fonctions_calcul.h"
#include "force_brute.h"
#include "demi_matrice.h"

int main(int argc, char* argv[]){
    
    if(argc!=5){
        printf("Usage: %s  -f fichier.tsp -m bf\n", argv[0]);
        exit(0);
    }

    char path[256];
    snprintf(path, sizeof(path), "%s", argv[2]);
    
    FichierTSP* tsp = malloc(sizeof(FichierTSP));
    readTSP(path,tsp);
    int longueur;

    FichierTour* tour = force_brute(tsp, tsp->edge_type);
        
    int (*distance)(Node,Node);
    switch(tsp->edge_type){
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

    longueur = longueur_tournee(tsp, tour, distance);
    printf("Tour ; ");
    printf("%s ; ", argv[2]); //fichier 
    printf("bf ; ");

    printf("0.0 ; ");
    printf("%d ; ", longueur);

    printf("[");
    for (int i = 0; i < tour->dimension; i++) {
        if (i > 0) printf(",");
        printf("%d", tour->nodes[i]);
    }
    printf("] ; ");
    
    return 0;
}
