#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "data.h"
#include "fonctions_calcul.h"
#include "force_brute.h"
#include "Demi matrice.h"

int main(int argc, char* argv[]){
    if(argc!=2){
        printf("Usage: %s [nom_fichier_tsp]\n", argv[0]);
        exit(0);
    }

    char path[256];
    snprintf(path, sizeof(path), "../JDD/ALL_tsp/%s.tsp", argv[1]);

    FichierTSP* tsp = malloc(sizeof(FichierTSP));
    readTSP(path,tsp);
    printf("Instance ; algo ; long (C) ; long (Python) ; temps ; tour ; valid ; mêmes longueurs");
    char *instance = tsp->name;
    char *algo = "cannonical";
    long long_C;
    long long_Python = 0;
    int temps;
    // tour;
    int valid = 0;
    int memes_longueurs = (long_C==long_Python);
    force_brute(tsp, tsp->edge_type);
    return 0;
}