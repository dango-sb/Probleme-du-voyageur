#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data.h"
#include "fonctions_calcul.h"
#define POPULATION_SIZE 30
#define GENERATIONS 1000
#define MUTATION_RATE 0.10

// ------------------- UTILITAIRES -------------------

void shuffle(int *array, int n) {
    for (int i = n-1; i > 0; i--) {
        int j = rand() % (i+1);
        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}

void copy_individual(int* dest, int* src, int dimension) {
    memcpy(dest, src, (dimension + 1) * sizeof(int)); // +1 pour -1
}

// ------------------- FITNESS -------------------

int fitness(int* individual, int dimension, FichierTSP* tsp, int (*distance)(Node, Node)) {
    FichierTour tour;
    tour.dimension = dimension;
    tour.nodes = individual;
    return longueur_tournee(tsp, &tour, distance);
}

// ------------------- CROISEMENT -------------------

void ordered_crossover(int* parent_a, int* parent_b, int* child, int dimension) {
    int start = rand() % dimension;
    int end = rand() % dimension;
    if (start > end) { int tmp = start; start = end; end = tmp; }

    for (int i = 0; i < dimension; i++) child[i] = -1;
    for (int i = start; i < end; i++) child[i] = parent_a[i];

    int idx = end % dimension;
    for (int i = 0; i < dimension; i++) {
        int gene = parent_b[(end + i) % dimension];
        int found = 0;
        for (int j = 0; j < dimension; j++)
            if (child[j] == gene) { found = 1; break; }
        if (!found) {
            child[idx] = gene;
            idx = (idx + 1) % dimension;
        }
    }
    child[dimension] = -1; // tour terminé par -1
}

// ------------------- MUTATION -------------------

void swap_mutation(int* individual, int dimension) {
    for (int i = 0; i < dimension; i++) {
        double r = (double)rand() / RAND_MAX;
        if (r < MUTATION_RATE) {
            int j = rand() % dimension;
            int tmp = individual[i];
            individual[i] = individual[j];
            individual[j] = tmp;
        }
    }
    individual[dimension] = -1; // garantir -1 à la fin
}

// ------------------- TOURNOI -------------------

void tournament_selection(int** population, int* fitness_values, int** selected,
                          int population_size, int dimension, int tournament_size) {
    for (int i = 0; i < population_size; i++) {
        int best_idx = rand() % population_size;
        for (int t = 1; t < tournament_size; t++) {
            int idx = rand() % population_size;
            if (fitness_values[idx] < fitness_values[best_idx])
                best_idx = idx;
        }
        copy_individual(selected[i], population[best_idx], dimension);
    }
}

// ------------------- MAIN -------------------

int main(int argc, char* argv[]) {

    if(argc != 5) {
        printf("Usage: %s -f fichier.tsp -m ga\n", argv[0]);
        exit(0);
    }

    char path[256];
    snprintf(path, sizeof(path), "%s", argv[2]);
    FichierTSP* tsp = malloc(sizeof(FichierTSP));
    if (!tsp) { printf("Erreur malloc tsp\n"); return 1; }
    readTSP(path, tsp);

    if (!tsp->nodes) { printf("Erreur : tsp->nodes non initialisé\n"); return 1; }

    printf("Fichier TSP lu : %s, dimension = %d, edge_type = %d\n", path, tsp->dimension, tsp->edge_type);

    srand((unsigned int)time(NULL));
    int dimension = tsp->dimension;

    int (*distance)(Node, Node);
    switch(tsp->edge_type) {
        case EUC_2D: distance = distance_euc_2d; break;
        case ATT:   distance = distance_att; break;
        case GEO:   distance = distance_geo; break;
        default:
            fprintf(stderr, "Type d'arête inconnu\n");
            exit(1);
    }

    // Allocation population, offspring et selected (+1 pour -1)
    int** population = malloc(POPULATION_SIZE * sizeof(int*));
    int** offspring  = malloc(POPULATION_SIZE * sizeof(int*));
    int** selected   = malloc(POPULATION_SIZE * sizeof(int*));
    int* fitness_values = malloc(POPULATION_SIZE * sizeof(int));
    int* best_individual = malloc((dimension + 1) * sizeof(int));

    for (int i = 0; i < POPULATION_SIZE; i++) {
        population[i] = malloc((dimension + 1) * sizeof(int));
        offspring[i]  = malloc((dimension + 1) * sizeof(int));
        selected[i]   = malloc((dimension + 1) * sizeof(int));
    }

    // Initialisation population
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < dimension; j++) population[i][j] = j;
        shuffle(population[i], dimension);
        population[i][dimension] = -1; // garantir -1
    }
    copy_individual(best_individual, population[0], dimension);

    int tournament_size = POPULATION_SIZE / 2;

    // Boucle principale
    for (int gen = 0; gen < GENERATIONS; gen++) {

        // Calcul fitness
        for (int i = 0; i < POPULATION_SIZE; i++)
            fitness_values[i] = fitness(population[i], dimension, tsp, distance);

        // Sélection
        tournament_selection(population, fitness_values, selected, POPULATION_SIZE, dimension, tournament_size);

        // Croisement
        for (int i = 0; i < POPULATION_SIZE; i += 2) {
            if (i + 1 < POPULATION_SIZE) {
                ordered_crossover(selected[i], selected[i+1], offspring[i], dimension);
                ordered_crossover(selected[i+1], selected[i], offspring[i+1], dimension);
            } else {
                copy_individual(offspring[i], selected[i], dimension);
            }
        }

        // Mutation
        for (int i = 0; i < POPULATION_SIZE; i++) swap_mutation(offspring[i], dimension);

        // Remplacer population par offspring
        for (int i = 0; i < POPULATION_SIZE; i++)
            copy_individual(population[i], offspring[i], dimension);

        // Remplacer le pire par un aléatoire
        for (int i = 0; i < POPULATION_SIZE; i++)
            fitness_values[i] = fitness(population[i], dimension, tsp, distance);

        int worst_idx = 0;
        int worst_fit = fitness_values[0];
        for (int i = 1; i < POPULATION_SIZE; i++)
            if (fitness_values[i] > worst_fit) { worst_fit = fitness_values[i]; worst_idx = i; }

        for (int j = 0; j < dimension; j++) population[worst_idx][j] = j;
        shuffle(population[worst_idx], dimension);
        population[worst_idx][dimension] = -1; // garantir -1

        // Mise à jour du meilleur individu
        for (int i = 0; i < POPULATION_SIZE; i++) {
            int f = fitness(population[i], dimension, tsp, distance);
            if (f < fitness(best_individual, dimension, tsp, distance))
                copy_individual(best_individual, population[i], dimension);
        }
    }

    // Affichage final
    int final_distance = fitness(best_individual, dimension, tsp, distance);
    printf("Best GA solution: [");
    for (int i = 0; i < dimension; i++) {
        if (i > 0) printf(",");
        printf("%d", best_individual[i]);
    }
    printf("] longueur = %d\n", final_distance);

    // Libération mémoire
    for (int i = 0; i < POPULATION_SIZE; i++) {
        free(population[i]);
        free(offspring[i]);
        free(selected[i]);
    }
    free(population);
    free(offspring);
    free(selected);
    free(fitness_values);
    free(best_individual);
    free(tsp);

    return 0;
}
