#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "data.h"
#include "fonctions_calcul.h"
#include "demi_matrice.h"


FichierTour * plus_proche_voisin(FichierTSP * tsp){
    FichierTour * tour = malloc(sizeof(FichierTour));
    strcpy(tour->name,tsp->name);
    tour->dimension = tsp->dimension;
    tour->nodes = (int*)malloc((tour->dimension+1) * sizeof(int));
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
    int ** matrice = demie_matrice(tsp,distance);
    int point_utilise[tour->dimension];
    for (int i=0; i<tour->dimension;i++){
        point_utilise[i]=i;
    }
    tour->nodes[0]=0;
    point_utilise[0]=-1;
    int dernier_point = 0;
    int nb_points = 1;
    int meilleur_point,dist_min,dist;
    while(nb_points<tour->dimension){
        dist_min = 10000000;
        for(int j=0;j<tour->dimension;j++){
            if(point_utilise[j]!=-1){
                if(j<dernier_point)
                    //dist=matrice[j][dernier_point];
                    dist=matrice[dernier_point][j];
                else
                    //dist=matrice[dernier_point][j];
                    dist=matrice[j][dernier_point];
                if(dist<dist_min){
                    meilleur_point = j;
                    dist_min=dist;
                }
            }
        }
        //printf("%d,%d = %d\n",meilleur_point,dernier_point,dist_min);
        tour->nodes[nb_points]=meilleur_point;
        point_utilise[meilleur_point]=-1;
        dernier_point=meilleur_point;
        nb_points++;
    }
    tour->nodes[nb_points]=-1;
    return tour;
}
