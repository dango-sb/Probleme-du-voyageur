#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "data.h"
#include "fonctions_calcul.h"

void echanger(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

FichierTour* random_walk(FichierTSP* tsp){
    FichierTour * tour = malloc(sizeof(FichierTour));
    strcpy(tour->name,tsp->name);
    tour->dimension = tsp->dimension;
    tour->nodes = (int*)malloc((tour->dimension+1) * sizeof(int));
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
    int random;
    printf("Lancement random_walk :\n");
    printf("Tournée :\n");
    for(int i=0;i<tsp->dimension;i++){
        random=rand()%(tsp->dimension-i)+i;
        printf("%d ",random);
        tsp->nodes[i]=random;
    }
    printf("\n");
    tsp->nodes[tsp->dimension]=-1;
    printf("Longeur tournée random: %d",longueur_tournee(tsp,tour,distance));
    return tsp;
}

