#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "random_walk.h"
#include "fonctions_calcul.h"
#include "fonctions_python.h"
#include "force_brute.h"

int t_size=15;
int p_size=30;

void init_var(int t,int p){
    t_size=t;
    p_size=p;
}
FichierTour** random_population(int population_size,FichierTSP* tsp){
    t_size=20;
    FichierTour** population = malloc(population_size * sizeof(FichierTour*));
    for(int i=0;i<population_size;i++){
        FichierTour* tour=random_walk(tsp);
        population[i]=tour;
    }
    return population; 
}

FichierTour** tournament_selection(FichierTour** population, FichierTSP* tsp,int population_size,int tournament_size,int (*distance)(Node ,Node)){
    
    FichierTour** selected= malloc(population_size * sizeof(FichierTour*));
    FichierTour** competitors = malloc(tournament_size * sizeof(FichierTour*));
    for(int i=0;i<population_size;i++){
        competitors= sample(tournament_size,population,population_size);
        FichierTour* winner = min(competitors,tournament_size,tsp,distance); 
        selected[i]=winner;
    }
    free(competitors);
    return selected;
}

FichierTour* ordered_crossover(FichierTour* parent_a,FichierTour* parent_b){
    int start=rand()%parent_a->dimension;
    int end=rand()%parent_a->dimension;
    if(start>end){
        int temp=start;
        start=end;
        end=temp;
    }
    if(start==end){
        if(start==0){
            end++;
        }else{
            start--;
        }
    }
    int tab[parent_a->dimension];
    FichierTour* child;
    child->dimension=parent_a->dimension;
    for(int i=start;i<end;i++){
        child->nodes[i]=parent_a->nodes[i];
        tab[i-start]=parent_a->nodes[i];
    }
    int partie_inferieure=0;
    int partie_superieur=end;
    for(int n=0;n<parent_b->dimension;n++){
        int node=parent_b->nodes[n];
        if(!contains(tab,end,node)){
            if(partie_inferieure<start){
                child->nodes[partie_inferieure]=node;
                partie_inferieure++;
            }else{
                if(partie_superieur<parent_a->dimension){
                    child->nodes[partie_superieur]=node;
                    partie_superieur++;
                }
            }
            tab[end]=node;
            end++;
        }
    }
    return child;
}


void swap_mutation(FichierTour* individual,float mutation_rate){
    
    for(int i=0;i<individual->dimension;i++){
        float random=(float)rand();
        if (random/(float)RAND_MAX < mutation_rate){
            int j = rand()%individual->dimension;
            echanger(&individual->nodes[i],&individual->nodes[j]);
        }
    }
}

void print_solution(FichierTour* best_individual,FichierTSP* tsp,int (*distance)(Node ,Node)){
    int dist = longueur_tournee(tsp,best_individual,distance);
    printf("Best GA solution: ");
    for(int i=0;i<best_individual->dimension;i++){
        printf("%d",best_individual->nodes[i]);
    }
    printf("\n");
    printf(" longueur = %d\n",dist);
}