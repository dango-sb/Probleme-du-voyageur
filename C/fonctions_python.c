#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "data.h"
#include "random_walk.h"
#include "fonctions_calcul.h"

FichierTour** sample(int tournament_size,FichierTour** population,int population_size){
    FichierTour** sample = (FichierTour**)malloc(tournament_size * sizeof(FichierTour*));
    int tab[tournament_size];
    int i=0;
    while(i<tournament_size){
        int random=rand()%population_size;
        if(!contains(tab,i,random)){
            tab[i]=random;
            sample[i]=population[random];
            i++;
        }
    }
    return sample;
}

FichierTour* min(FichierTour** population,int population_size,FichierTSP* tsp,int (*distance)(Node ,Node)){
    FichierTour* meilleur=population[0];
    for(int i=1;i<population_size;i++){
        if(longueur_tournee(tsp,meilleur,distance)>longueur_tournee(tsp,population[i],distance)){
            meilleur=population[i];
        }
    }
    return meilleur;
}

FichierTour* max(FichierTour** population,int population_size,FichierTSP* tsp,int (*distance)(Node ,Node)){
    FichierTour* pire=population[0];
    for(int i=1;i<population_size;i++){
        if(longueur_tournee(tsp,pire,distance)<longueur_tournee(tsp,population[i],distance)){
            pire=population[i];
        }
    }
    return pire;
}



void sorted(FichierTour** tours,int taille,FichierTSP *tsp,int (*distance)(Node, Node)){
    for (int i = 1; i < taille; i++) {

        FichierTour* key = tours[i];  // copie locale
        int key_len = longueur_tournee(tsp, key, distance);

        int j = i - 1;
        while (j >= 0 &&longueur_tournee(tsp, tours[j], distance) > key_len) {
            tours[j + 1] = tours[j];
            j--;
        }
        tours[j + 1] = key;
    }
}

bool equal_tour(FichierTour* tour1,FichierTour* tour2){
    for(int i=0;i<tour1->dimension;i++){
        if(tour1->nodes[i]!=tour2->nodes[i]){
            return false;
        }
    }
    return true;
}

int pindex(FichierTour** population,FichierTour* tour,int population_size){
    int ind=0;
    for(int i=0;i<population_size;i++){
        if(equal_tour(population[i],tour)){
            ind=i;
        }
    }
    return ind;
}