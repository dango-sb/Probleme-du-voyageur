#ifndef DATA_H
#define DATA_H

#include <stdio.h>

typedef enum { EUC_2D, ATT, GEO } EdgeType;

typedef struct {
    int numero;       
    EdgeType type;
    union {
        struct { float x, y; } euc;     // distance euclidienne classique 
        struct { int x, y; } att;       // distance euclidienne modifiée
        struct { float lt,           // -90<=latitude<=90
                       lgt;          // -180<=longitude<=180
                       } geo;           // distance géographique
                                                   
    } coord;
} Node;

/*
TYPE : TSP c’est à dire le problème symétrique et complet 
EDGE_WEIGHT_TYPE : EUCL_2D, GEO, ATT 
NODE_COORD_SECTION : entiers ou flottants.
*/

typedef struct {
    char name[64];          // NAME
    int dimension;          // DIMENSION
    EdgeType edge_type;    // EDGE_WEIGHT_TYPE
    Node* nodes;           // NODE_COORD_SECTION
} FichierTSP;

/*
TYPE: TOUR
*/

typedef struct{
    char name[64];
    int dimension;
    int* nodes;         // TOUR_SECTION
} FichierTour;

/*
TYPE: TSP
EDGE_WEIGHT_TYPE : EXPLICIT
EDGE_WEIGHT_FORMAT : LOWER_DIAG_ROW, UPPER_DIAG_ROW
*/

typedef enum { FULL_MATRIX, UPPER_ROW, LOWER_ROW, UPPER_DIAG_ROW,
                LOWER_DIAG_ROW } EdgeFormat;

typedef struct {
    char name[64];          // NAME
    int dimension;          // DIMENSION
    EdgeFormat format;      // EDGE_WEIGHT_FORMAT
    int* weights;           // EDGE_WEIGHT_SECTION
} FichierExplicit;


void readTSP(char* nomFischier, FichierTSP* tsp);
void readTour(FILE* f, FichierTour* tour);
EdgeType matchType(char* str);
char* typeToChar(EdgeType edge);

#endif // DATA_H