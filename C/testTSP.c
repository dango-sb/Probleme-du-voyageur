#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"
#include "data.c"


int main(int argc, char* argv[]) {
    
    if (argc < 2) {
       tspprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char path[256];
    snprintf(path, sizeof(path), "../JDD/ALL_tsp/%s.tsp", argv[1]);

    FischierTSP* tsp;
    readTSP(path,tsp);
    printf("Tour name: %s\n",tsp->name);
    printf("Dimension: %d\n",tsp->dimension);
    if (tsp->edge_type==ATT){
        printf("EDGE_WEIGHT_TYPE: ATT\n");
        for (int i = 0; i<tsp->dimension; i++){
        printf("%d %d %d\n",tsp->nodes[i].numero,tsp->nodes[i].coord.att.x,tsp->nodes[i].coord.att.y);
        }
    }
    else if (tsp->edge_type==EUC_2D){
        printf("EDGE_WEIGHT_TYPE: EUC_2D\n");
        for (int i = 0; i<tsp->dimension; i++){
        printf("%d %f %f\n",tsp->nodes[i].numero,tsp->nodes[i].coord.euc.x,tsp->nodes[i].coord.euc.y);
        }
    }
    else if (tsp->edge_type==GEO){
        printf("EDGE_WEIGHT_TYPE: GEO\n");
        for (int i = 0; i<tsp->dimension; i++){
        printf("%d %f %f\n",tsp->nodes[i].numero,tsp->nodes[i].coord.geo.lt,tsp->nodes[i].coord.geo.lgt);
        }
    }
    
    printf("fin\n");
    return 0;
}