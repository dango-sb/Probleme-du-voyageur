#ifndef TSP_GA_FCT_H
#define TSP_GA_FCT_H

#include <stdio.h>
#include "data.h"
#include "fonctions_calcul.h"


FichierTour** random_population(int population_size,FichierTSP* tsp);
FichierTour** tournament_selection(FichierTour** population, FichierTSP* tsp,int population_size,int tournament_size,int (*distance)(Node ,Node));
FichierTour* ordered_crossover(FichierTour* parent_a,FichierTour* parent_b);
void swap_mutation(FichierTour* individual,float mutation_rate);
void print_solution(FichierTour* best_individual,FichierTSP* tsp,int (*distance)(Node ,Node));

#endif // TSP_GA_FCT_H