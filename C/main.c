#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "data.h"
#include "fonctions_calcul.h"
#include "force_brute.h"
#include "Demi matrice.h"

int main(int argc, char* argv[]){
    clock_t start, end;
    
    if(argc!=2){
        printf("Usage: %s [nom_fichier_tsp]\n", argv[0]);
        exit(0);
    }

    char path[256];
    snprintf(path, sizeof(path), "../JDD/ALL_tsp/%s.tsp", argv[1]);

    FichierTSP* tsp = malloc(sizeof(FichierTSP));
    readTSP(path,tsp);
    char *instance = tsp->name;
    char *methode = "cannonical";
    double temps_cpu;
    int longueur;

    
    start = clock();    
    FichierTour* tour = force_brute(tsp, tsp->edge_type);
    end = clock();
    

    temps_cpu = ((double)(end - start)) / CLOCKS_PER_SEC;   
    
    longueur = longueur_tournee(tsp, tour, tsp->edge_type);
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