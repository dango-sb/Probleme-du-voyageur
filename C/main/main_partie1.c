#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "data.h"
#include "fonctions_calcul.h"
#include "force_brute.h"
#include "demi_matrice.h"

int main(int argc, char* argv[]){
    clock_t start, end;
    
    if(argc!=2){
        printf("Usage: %s fichier.tsp\n", argv[0]);
        exit(0);
    }

    char path[256];
    snprintf(path, sizeof(path), "../JDD/ALL_tsp/%s", argv[1]);
    
    FichierTSP* tsp = malloc(sizeof(FichierTSP));
    readTSP(path,tsp);
    char *methode = "cannonical";
    double temps_cpu;
    int longueur;

    
    start = clock();    
    FichierTour* tour = force_brute(tsp, tsp->edge_type);
    end = clock();
    

    temps_cpu = ((double)(end - start)) / CLOCKS_PER_SEC;   
    
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
    printf("Instance ; Méthode ; Temps CPU (sec) ; Longueur ; Tour\n");
    printf("%s.tsp ; %s ; %lf ; %d ; [", argv[1], methode, temps_cpu, longueur);

    for (int i = 0; i<tour->dimension;i++){
        if (i==(tour->dimension-1)){
            printf("%d]\n", tour->nodes[i]);
            break;
        }
        printf("%d, ", tour->nodes[i]);
    }

    return 0;
}
