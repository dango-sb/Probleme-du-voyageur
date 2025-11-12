#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "data.h"
#include "fonctions_calcul.h"
#include "demi_matrice.h"
#include "plus_proche_voisin.h"

int main(int argc, char * argv[]){
    if(argc!=2){
        fprintf(stderr,"Usage : %s fichier tsp\n",argv[0]);
    }

    char path[256];
    snprintf(path, sizeof(path), "../JDD/ALL_tsp/%s", argv[1]);

    FichierTSP* tsp = malloc(sizeof(FichierTSP));
    readTSP(path,tsp);

    FichierTour * tour = plus_proche_voisin(tsp);

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

    int longueur = longueur_tournee(tsp, tour, distance);

    printf("distance : %d\n",longueur);

    for (int i = 0; i<tour->dimension;i++){
        if (i==(tour->dimension-1)){
            printf("%d]\n", tour->nodes[i]);
            break;
        }
        printf("%d, ", tour->nodes[i]);
    }

    return 0;
}
