#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"


EdgeType matchType(char* str){
    if (!strcmp(str, "EUC_2D")){
        return EUC_2D;
    }
    else if (!strcmp(str, "ATT")){
        return ATT;
    }
    else if (!strcmp(str, "GEO")){
        return GEO;
    }
    else {        
        perror("Erreur lors de la correspondance des types.\n");
        exit(1);
    }
}

char* typeToChar(EdgeType edge){
    if (edge == EUC_2D){
        return "EUC_2D";
    }
    else if (edge == ATT){
        return "ATT";
    }
    else if (edge == GEO){
        return "GEO";
    }
    else{
        perror("Erreur lors de la correspondance des types.\n");
        exit(1);
    }
}



void readTSP(char* nomFichier, FichierTSP* tsp){   
    FILE *f = fopen(nomFichier, "r");  
    if(f == NULL) {
        perror("Erreur lors de l'ouverture du fischier.\n");
        return;
    }
    // Nous n'enregistrons que les champs qui sont nécessaires
    char buffer[256];
    while(fgets(buffer, sizeof(buffer), f)) {
    if(strncmp(buffer, "NAME", 4) == 0) {
        sscanf(buffer, "NAME : %63s", tsp->name);
    } else if(strncmp(buffer, "DIMENSION", 9) == 0) {
        sscanf(buffer, "DIMENSION : %d", &tsp->dimension);
    } else if(strncmp(buffer, "EDGE_WEIGHT_TYPE", 16) == 0) {
        char type_str[32];
        sscanf(buffer, "EDGE_WEIGHT_TYPE : %31s", type_str);
        tsp->edge_type = matchType(type_str);
    } else if(strncmp(buffer, "NODE_COORD_SECTION", 18) == 0) 
        break; 
    }   

    if(tsp->dimension <= 0) 
        return;

    tsp->nodes = malloc(sizeof(Node) * tsp->dimension);

    if(!tsp->nodes) 
    {
        perror("malloc\n");
        return;
    }

    int numero;
    if(tsp->edge_type == EUC_2D) {
        float x, y;
        for(int i = 0; i < tsp->dimension; i++) {
            if(!fgets(buffer, sizeof(buffer), f)) 
                break;
            sscanf(buffer, "%d %f %f", &numero, &x, &y);
            tsp->nodes[i].numero = numero;
            tsp->nodes[i].type = EUC_2D;
            tsp->nodes[i].coord.euc.x = x;
            tsp->nodes[i].coord.euc.y = y;
        }

    } else if(tsp->edge_type == ATT) {
        int x, y;
        for(int i = 0; i < tsp->dimension; i++) {
            if(!fgets(buffer, sizeof(buffer), f)) 
                break;

            sscanf(buffer, "%d %d %d", &numero, &x, &y);
            tsp->nodes[i].numero = numero;
            tsp->nodes[i].type = ATT;
            tsp->nodes[i].coord.att.x = x;
            tsp->nodes[i].coord.att.y = y;
        }

    } else if(tsp->edge_type == GEO) {
        float lt, lgt;
        for(int i = 0; i < tsp->dimension; i++) {
            if(!fgets(buffer, sizeof(buffer), f)) 
                break;
            sscanf(buffer, "%d %f %f", &numero, &lt, &lgt);
            tsp->nodes[i].numero = numero;
            tsp->nodes[i].type = GEO;
            tsp->nodes[i].coord.geo.lt = lt;
            tsp->nodes[i].coord.geo.lgt = lgt;
        }
    }

    printf("Succès\n");
    fclose(f); 
}

void readTour(FILE* f, FichierTour* tour) {
    char buffer[128];

    while (fgets(buffer, sizeof(buffer), f)) {
        if (strncmp(buffer, "NAME", 4) == 0) {
            sscanf(buffer, "NAME : %63s", tour->name);
        }
        else if (strncmp(buffer, "DIMENSION", 9) == 0) {
            sscanf(buffer, "DIMENSION : %d", &tour->dimension);

            tour->nodes = malloc(tour->dimension * sizeof(int));
            if (!tour->nodes) {
                fprintf(stderr, "Erreur: impossible d'allouer la mémoire pour TOUR.\n");
                exit(1);
            }
        }
        else if (strncmp(buffer, "TOUR_SECTION", 12) == 0) {
            int idx = 0;
            int node;

            while (fscanf(f, "%d", &node) == 1) {
                if (node == -1) 
                    break; 

                if (idx < tour->dimension) {
                    tour->nodes[idx++] = node;
                } else {
                    fprintf(stderr, "Erreur: plus de noeuds que la dimension annoncée (%d).\n", tour->dimension);
                    exit(1);
                }
            }
        }
    }
}

