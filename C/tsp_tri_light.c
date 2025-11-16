#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "data.h"
#include "fonctions_calcul.h"
#include "tsp_ga_fct.h"
#include "fonctions_python.h"

#define population_size 30
#define generations 1000
#define mutation_rate 0.10
// #define tournament_size int(0.5*population_size)
#define tournament_size 15

int main(int argc, char *argv[]){
    char path[256];
    snprintf(path, sizeof(path), "../JDD/ALL_tsp/%s", argv[2]);
    FichierTSP* tsp = malloc(sizeof(FichierTSP));
    if (!tsp) {
        fprintf(stderr, "Erreur malloc tsp\n");
        return 1;
    }
    readTSP(path, tsp);
    FichierTour** population=random_population(population_size,tsp);
    int (*distance)(Node,Node);
    switch(tsp->edge_type){
        case EUC_2D:
            distance = distance_euc_2d;
            break;
        case ATT:
            distance = distance_att;
            break;
        case GEO:
            distance = distance_geo;
            break;
    }
    FichierTour* best_individual = population[0]->nodes;
    for(int i=0; i<generations;i++){
        FichierTour** selected = tournament_selection(population, calcul_tournee(), tournament_size, distance_fct, coord);
        FichierTour** offspring;
        for(int j=0;j<population_size;j+=2){
            FichierTour* child_a = ordered_crossover(selected[j], selected[j + 1]);
            FichierTour* child_b = ordered_crossover(selected[j + 1], selected[j]);
            offspring[j]=child_a;
            offspring[j+1]=child_b;
        }
        for(int n=0;n<population_size;i++){
            swap_mutation(offspring[n], mutation_rate);
        }
        // offspring = sorted(offspring,population_size,tsp,distance)
        sorted(offspring,population_size,tsp,distance);
        population = offspring;
        FichierTour* worst= max(population,population_size,tsp,distance);
        int ind = index(population,worst,population_size);
        population[ind] = random_walk(tsp);
        FichierTour* best_generation = min(population,population_size,tsp,distance);
        FichierTour* worst_generation = max(population,population_size,tsp,distance);
        ind = index(population,worst_generation,population_size);
        if(longueur_tournee(tsp,best_generation,distance)<longueur_tournee(tsp,best_individual,distance)){
            best_individual = best_generation;
        }
        population[ind] = best_individual;
    }
    print_solution(best_individual,tsp,distance);
    return 0;
}