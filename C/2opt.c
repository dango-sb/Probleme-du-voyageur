#include <stdio.h>
#include "data.h"
#include "fonctions_calcul.h"

void permuter(int *nodes, int i, int j) {
    while (i < j) {
        int tmp = nodes[i];
        nodes[i] = nodes[j];
        nodes[j] = tmp;
        i++;
        j--;
    }
}

void two_opt(FichierTSP *tsp, FichierTour *tour, int (*distance)(Node, Node)) {
    int improved = 1;
    int n = tour->dimension;

    while (improved) {
        improved = 0;

        for (int i = 1; i < n; i++) {
            for (int j = i + 1; j < n; j++) {

                int p1 = tour->nodes[i - 1];
                int p2 = tour->nodes[i];
                int p3 = tour->nodes[j];
                int p4 = tour->nodes[(j + 1) % n];

                int d1 = distance(tsp->nodes[p1], tsp->nodes[p2]) + distance(tsp->nodes[p3], tsp->nodes[p4]);
                int d2 = distance(tsp->nodes[p1], tsp->nodes[p3]) + distance(tsp->nodes[p2], tsp->nodes[p4]);

                if (d2 < d1) {
                    permuter(tour->nodes, i, j);
                    improved = 1;
                }
            }
        }
    }
}
