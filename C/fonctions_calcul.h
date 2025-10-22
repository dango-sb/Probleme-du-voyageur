#ifndef fonction_calcul_h
#define fonction_calcul_h
#include"data.h"

int distance_euc_2d(Node node1, Node node2);

int distance_att(Node node1, Node node2);

int distance_geo(Node node1, Node node2);

int longueur_tournee(FichierTSP * tsp, FichierTour * tour, int (*distance)(Node ,Node));


#endif
