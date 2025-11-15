#ifndef FONCTIONS_PYTHON_H
#define FONCTIONS_PYTHON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

FichierTour** sample(int tournament_size,FichierTour** population,int population_size);
FichierTour* min(FichierTour** population,int population_size,FichierTSP* tsp,int (*distance)(Node ,Node));
FichierTour* max(FichierTour** population,int population_size,FichierTSP* tsp,int (*distance)(Node ,Node));
void sorted(FichierTour** tours,int taille,FichierTSP *tsp,int (*distance)(Node, Node));
int index(FichierTour** population,FichierTour* tour,int population_size);
#endif