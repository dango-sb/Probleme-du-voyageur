#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "demi_matrice.h"
#include "data.h"


int **demie_matrice(FichierTSP *fichier,int (*dist)(Node,Node)){
    int n=fichier->dimension;
    int **M=(int **)malloc(n*sizeof(int *));
    if(M==NULL){
        fprintf(stderr,"erreur allocation de mémoire pour la matrice");
        exit(1);
    }
    for(int i=0;i<n;i++){
        M[i]=(int*)malloc(i*sizeof(int));
        if(M[i]==NULL){
         fprintf(stderr,"erreur allocation de mémoire pour la matrice");
            exit(1);
        }
        for(int j=0;j<i;j++){
            M[i][j]=dist(fichier->nodes[i],fichier->nodes[j]);
        }
    }
    return M;
}
