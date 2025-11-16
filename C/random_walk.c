#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <stdbool.h>
#include "data.h"
#include "fonctions_calcul.h"

bool contains(int tab[], int taille, int valeur) {
    for (int i = 0; i < taille; i++) {
        if (tab[i] == valeur) {
            return true;
        }
    }
    return false;
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
    int nombres[tsp->dimension];
    int i=0;
    while(i<tsp->dimension){
        random=rand()%tsp->dimension;
        if(!contains(nombres,i,random)){
            tour->nodes[i]=random;
            nombres[i]=random;
            i++;
    }
    }
    tour->nodes[tsp->dimension]=-1;
    
    return tour;
}

