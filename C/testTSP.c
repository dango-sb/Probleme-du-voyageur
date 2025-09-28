#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"
#include "data.c"


int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char path[256];
    snprintf(path, sizeof(path), "../JDD/ALL_tsp/%s", argv[1]);

    FischierTSP* f;
    readTSP(path, f);
    if (f->edge_type==ATT){
        printf("EDGE_WEIGHT_TYPE: ATT\n");
        for (int i = 0; i< f->dimension; i++){
        printf("%d %d %d\n", f->nodes[i].numero, f->nodes[i].coord.att.x, f->nodes[i].coord.att.y);
        }
    }
    else if (f->edge_type==EUC_2D){
        printf("EDGE_WEIGHT_TYPE: EUC_2D\n");
        for (int i = 0; i< f->dimension; i++){
        printf("%d %f %f\n", f->nodes[i].numero, f->nodes[i].coord.euc.x, f->nodes[i].coord.euc.y);
        }
    }
    else if (f->edge_type==GEO){
        printf("EDGE_WEIGHT_TYPE: GEO\n");
        for (int i = 0; i< f->dimension; i++){
        printf("%d %f %f\n", f->nodes[i].numero, f->nodes[i].coord.geo.lt, f->nodes[i].coord.geo.lgt);
        }
    }
    
    printf("fin\n");
    return 0;
}