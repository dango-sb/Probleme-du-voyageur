#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "data.h"

int distance_euc_2d(Node node1, Node node2){
    if ((node1.type) != EUC_2D){
        fprintf(stderr, "type EUC_2D attendu \n");
        exit(1);
    }
    float xd = (node1.coord.euc.x) - (node2.coord.euc.x);
    float yd = (node1.coord.euc.y) - (node2.coord.euc.y);
    float distance = sqrt(xd*xd +yd*yd);
    int res = round(distance);
    return res;
}

int distance_att(Node node1, Node node2){
    if ((node1.type) != ATT){
        fprintf(stderr, "type ATT attendu \n");
        exit(1);
    }
    int xd = (node1.coord.att.x) - (node2.coord.att.x);
    int yd = (node1.coord.att.y) - (node2.coord.att.y);
    float rij = sqrt((xd*xd +yd*yd)/10.0);
    int tij = (int)(rij);
    if(tij<rij)
        return tij + 1;
    else
        return tij;
}

int distance_geo(Node node1, Node node2){
    if ((node1.type) != GEO){
        fprintf(stderr, "type GEO attendu \n");
        exit(1);
    }
    float pi = 3.141592;
    float rrr = 6378.388;
    float coords[4] = {node1.coord.geo.lt,node1.coord.geo.lgt,node2.coord.geo.lt,node2.coord.geo.lgt};
    float coord_calc[4];
    for (int i=0;i<4;i++){
        float coo = coords[i];
        int deg = round(coo);
        float min= coo - deg;
        coord_calc[i] = pi * (deg + 5.0 * min / 3.0) / 180.0;
    }
    float q1 = cos(coord_calc[1] - coord_calc[3]);
    float q2 = cos(coord_calc[0] - coord_calc[2]);
    float q3 = cos(coord_calc[0] + coord_calc[2]);
    int distance = (int)(rrr*acos(0.5*((1.0+q1)*q2 - (1.0-q1)*q3))+1.0);
    return distance;
}


int longueur_tournee(FichierTSP * tsp, FichierTour * tour, int (*distance)(Node,Node)){

    if(tsp->dimension < tour->dimension){
        fprintf(stderr, "fichier tour et fichier tsp non compatible");
        exit(1);
    }
    int longueur = 0;
    int indice1;
    int indice2;
    int i=0;

    while(tour->nodes[i]!=-1){
        indice1 = tour->nodes[i];
        indice2 = tour->nodes [(i+1)%(tour->dimension)];
        longueur+= distance(tsp->nodes[indice1],tsp->nodes[indice2]);
        i++;
    }

    return longueur;
}












