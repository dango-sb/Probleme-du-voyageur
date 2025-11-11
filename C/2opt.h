#ifndef DEUX_OPT_H
#define DEUX_OPT_H

#include <stdio.h>
#include "data.h"

void two_opt(FichierTSP *tsp, FichierTour *tour, int (*distance)(Node, Node));
void permuter(Node* nodes, int na, int nb);
#endif