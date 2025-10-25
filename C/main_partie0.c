#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "data.h"
#include "fonctions_calcul.h"

int main(int argc, char * argv[]){
    if (argc != 4 || (strcmp(argv[1],"-f")!=0) || (strcmp(argv[3],"-c")!=0)){
        fprintf(stderr, "Usage : %s -f fichier.tsp -c\n", argv[0]);
        exit(1);
    }

    //création et initialisation du fichiers tour canonique et tsp

    char path[256];
    snprintf(path, sizeof(path), "../JDD/ALL_tsp/%s", argv[2]);

    FichierTour *tour = malloc(sizeof(FichierTour));
    FichierTSP  *tsp  = malloc(sizeof(FichierTSP));
    readTSP(path, tsp);

    strcpy(tour->name,tsp->name);
    tour->dimension = tsp->dimension;
    tour->nodes = (int*)malloc((tour->dimension+1) * sizeof(int));
    for (int i=0; i<tour->dimension;i++){
        tour->nodes[i]=i;
    }
    tour->nodes[tour->dimension]=-1;

    int longueur;

    switch(tsp->edge_type){
        case EUC_2D:
            //Distance EUC_2D

            longueur =  longueur_tournee(tsp,tour,distance_euc_2d);
            printf("EUC_2D : \n\nDistance : %d\n",longueur);
            break;


        case GEO:
            //Distance GEO

            longueur = longueur_tournee(tsp,tour,distance_geo);
            printf("GEO : \n\nDistance : %d\n", longueur);
            break;

        case ATT:
            //Distance ATT

            longueur = longueur_tournee(tsp,tour,distance_att);
            printf("ATT : \n\nDistance : %d\n", longueur);
            break;

    }

    exit(0);

}
