#ifndef FORCE_BRUTE_H
#define FORCE_BRUTE_H

#include <stdio.h>
#include "data.h"
#include "fonctions_calcul.h"


void handler_signal(int sig);
void echanger(int *a,int *b);
void permuter(int *perm, int debut, int fin,FichierTSP *tsp, FichierTour *tour_actuel,EdgeType type,FichierTour *tour_best, int *best_len,FichierTour *tour_worst, int *worst_len,int (*distance)(Node,Node));
FichierTour* force_brute(FichierTSP *tsp,EdgeType type);


#endif // FORCE_BRUTE_H
