#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"
#include "data.c"

int main(int argc, char* argv[]){

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char path[256];
    snprintf(path, sizeof(path), "../JDD/ALL_tsp/%s.opt.tour", argv[1]);

    FischierTour* tour;
    FILE* f = fopen(path, "r");

    readTour(f, tour);

    printf("Tour name: %s\n", tour->name);
    printf("Dimension: %d\n", tour->dimension);
    for (int i = 0; i < tour->dimension; i++) {
        printf("%d\n", tour->nodes[i]);
    }
    printf("\n");
    fclose(f);
    return 0;
}