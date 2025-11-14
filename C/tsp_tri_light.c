#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <signal.h>
#include "data.h"
#include "fonctions_calcul.h"
#include "random_walk.h"
#include "tsp_ga_fct_.h"
#include "tsp_tools.h"

#define population_size=30
#define generations=1000
#define mutation_rate=0.10
#define tournament_size=int(0.5*population_size)
#define cities

#define coord
#define edge_type 

#define distance_fct,graphique

int main(int argc, char *argv[]){
    char path[256];
    snprintf(path, sizeof(path), "../JDD/ALL_tsp/%s", argv[2]);
    FichierTSP* tsp = malloc(sizeof(FichierTSP));
    if (!tsp) {
        fprintf(stderr, "Erreur malloc tsp\n");
        return 1;
    }
    readTSP(path, tsp);
    FichierTour* population = random_walk(tsp);
    best_individual = population->nodes[0];
    for(int i=0; i<generations;i++){
  /*      selected = tournament_selection(population, calcul_tournee(), tournament_size,distance_fct, coord)
        offspring = []
        for i in range(0, population_size, 2):
            child_a = ordered_crossover(selected[i], selected[i + 1])
            child_b = ordered_crossover(selected[i + 1], selected[i])
            offspring.append(child_a)
            offspring.append(child_b)

        for child in offspring:
            swap_mutation(child, mutation_rate)
        offspring = sorted(offspring, key=lambda indiv: fitness(indiv,distance_fct, coord)) # on trie
        population = offspring
        worste = max(population,key=lambda indiv: fitness(indiv,distance_fct,coord)) # la pire
        ind = population.index(worste)
        population[ind] = random.sample(cities, len(cities)) # on la remplace par du neuf
        best_generation = min(population, key=lambda indiv:fitness(indiv,distance_fct, coord)) #la meilleure
        worste_generation = max(population,key=lambda indiv: fitness(indiv,distance_fct,coord)) # la pire le retour
        ind = population.index(worste_generation)
        if fitness(best_generation,distance_fct, coord) < fitness(best_individual,distance_fct, coord): # mémoire générationnelle
            best_individual = best_generation
        population[ind] = best_individual # on le remet dans la piscine*/
    }
   // print_solution(best_individual,fitness,distance_fct,coord,file,instance,graphique)


    return 0;
}