#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "random_walk.h"
#include "fonctions_calcul.h"
#include "fonctions_pythons.h"
#include "force_brute.h"

FichierTour** random_population(int population_size,FichierTsp* tsp){
    static FichierTour* population[population_size];
    for(int i=0;i<population_size;i++){
        FichierTour* tour=random_walk(tsp);
        population[i]=tour;
    }
    return population; 
}

FichierTour** tournament_selection(FichierTour** population, FichierTsp* tsp,int population_size,int tournament_size,int (*distance)(Node ,Node)){
    static FichierTour* selected[population_size];
    for(int i=0;i<population size){
        static FichierTour* competitors[tournament_size]= sample(tournament_size,population,);
        FichierTour* winner = min(competitors,tournament_size,tsp,distance); # on prend le meilleur
        selected[i]=winner;
    }
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
        child->nodes[i]=parend_a->nodes[i]
        tab[i-start]=parent_a->nodes;
    }
    int partie_inferieure=0;
    int partie_superieur=end;
    for(int n=0;n<parent_b->dimension,n++){
        int node=parent_b->nodes[n];
        if(!contains(tab,end,node)){
            if(partie_inferieur<start){
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
    return child
}


void swap_mutation(FichierTour* individual,float mutation_rate){
    # mutation au hasard
    for(int i=0;i<individual->dimension;i++){
        float random=(float)rand;
        if (random/(float)RAND_MAX < mutation_rate):
            int j = rand()%individual->dimension;
            echanger(individual->nodes[i],individual->nodes[j]);
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