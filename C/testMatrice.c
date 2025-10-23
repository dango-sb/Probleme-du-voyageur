#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "data.h"
#include "fonctions_calcul.h"
#include "demi_matrice.h"

int main(int argc, char *argv[]){
    if(argc!=2){
        fprintf(stderr,"usage : %s fichier.tsp",argv[0]);
        exit(1);
    }

    FichierTSP  *tsp  = malloc(sizeof(FichierTSP));
    readTSP(argv[1], tsp);

    int (*distance)(Node,Node);

    switch(tsp->edge_type){
        case EUC_2D:
            //Distance EUC_2D

            distance = distance_euc_2d;

            break;


        case GEO:
            //Distance GEO

            distance = distance_geo;

            break;

        case ATT:
            //Distance ATT

            distance = distance_att;

            break;

    }

    int ** M = demie_matrice(tsp,distance);


    for (int i = 0; i < tsp->dimension; i++) {
        for (int j = 0; j < i; j++) {
            printf("%d ",M[i][j]); // Affiche les éléments de la ligne i
        }
        printf("\n"); // Retour à la ligne après chaque ligne

}



    return 0;
}
