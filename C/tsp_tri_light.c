#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include "data.h"
#include "fonctions_calcul.h"

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

// -----------------CROISEMENT DXP ET FONCTIONS AUXILIAIRES A CELUI CI------------------

// Trouve l'indice du plus proche voisin parmi les extrémités des fragments
int closest_fragment_end(int node, int num_fragments, int fragment_starts[], int fragment_ends[], int** fragments, int dimension, int (*distance)(Node, Node), Node* nodes) {
    int best_idx = -1;
    int best_dist = INT_MAX;
    for (int f = 0; f < num_fragments; f++) {
        int start = fragment_starts[f];
        int end = fragment_ends[f];
        if (start == -1) continue;

        int d_start = distance(nodes[node], nodes[fragments[f][start]]);
        if (d_start < best_dist) { best_dist = d_start; best_idx = f * 2; } // 2*f = start

        int d_end = distance(nodes[node], nodes[fragments[f][end]]);
        if (d_end < best_dist) { best_dist = d_end; best_idx = f * 2 + 1; } // 2*f+1 = end
    }
    return best_idx;
}

// DXP Crossover
void dxp_crossover(int* parent_a, int* parent_b, int* child, int dimension, int (*distance)(Node, Node), Node* nodes) {
    // 1. Copier parent A
    for (int i = 0; i < dimension; i++) child[i] = parent_a[i];
    child[dimension] = -1;

    // 2. Détecter les arêtes non communes
    int edges_to_remove[dimension][2]; // Liste des arêtes à enlever
    int num_remove = 0;
    for (int i = 0; i < dimension; i++) {
        int next = (i + 1) % dimension;
        int a = parent_a[i], b = parent_a[next];

        // Vérifier si (a,b) ou (b,a) est présent dans parent B
        int found = 0;
        for (int j = 0; j < dimension; j++) {
            int jb = (j + 1) % dimension;
            if ((parent_b[j] == a && parent_b[jb] == b) || (parent_b[j] == b && parent_b[jb] == a)) {
                found = 1;
                break;
            }
        }
        if (!found) {
            edges_to_remove[num_remove][0] = a;
            edges_to_remove[num_remove][1] = b;
            num_remove++;
        }
    }

    // 3. Supprimer les arêtes → créer des fragments
    int fragment_indices[dimension];
    for (int i = 0; i < dimension; i++) fragment_indices[i] = -1;

    int num_fragments = 0;
    int fragments[dimension][dimension]; // fragments[f][i] = node
    int frag_sizes[dimension];
    for (int f = 0; f < dimension; f++) frag_sizes[f] = 0;

    for (int i = 0; i < dimension; i++) {
        if (fragment_indices[i] != -1) continue;

        // Créer un nouveau fragment
        int f = num_fragments++;
        int current = i;
        while (fragment_indices[current] == -1) {
            fragments[f][frag_sizes[f]] = child[current];
            fragment_indices[current] = f;
            frag_sizes[f]++;
            
            // Avancer au suivant si l'arête n'est pas à supprimer
            int next = (current + 1) % dimension;
            int remove = 0;
            for (int r = 0; r < num_remove; r++) {
                if ((child[current] == edges_to_remove[r][0] && child[next] == edges_to_remove[r][1]) ||
                    (child[current] == edges_to_remove[r][1] && child[next] == edges_to_remove[r][0])) {
                    remove = 1; break;
                }
            }
            if (remove) break;
            current = next;
        }
    }

    // 4. Reconnecter les fragments par PPV
    int assembled[dimension];
    int assembled_size = 0;

    // Commencer avec le fragment contenant le premier noeud
    for (int i = 0; i < frag_sizes[0]; i++) assembled[assembled_size++] = fragments[0][i];

    int used_fragments[dimension];
    for (int f = 0; f < num_fragments; f++) used_fragments[f] = (f==0 ? 1 : 0);

    while (assembled_size < dimension) {
        int last_node = assembled[assembled_size - 1];
        int best_f = -1, best_end = -1;
        int best_dist = INT_MAX;

        for (int f = 0; f < num_fragments; f++) {
            if (used_fragments[f]) continue;

            // comparer start et end du fragment
            int start = fragments[f][0];
            int end = fragments[f][frag_sizes[f]-1];
            int d_start = distance(nodes[last_node], nodes[start]);
            int d_end = distance(nodes[last_node], nodes[end]);

            if (d_start < best_dist) { best_dist = d_start; best_f = f; best_end = 0; }
            if (d_end < best_dist)   { best_dist = d_end; best_f = f; best_end = 1; }
        }

        // Ajouter le fragment sélectionné
        if (best_end == 0) { // ordre normal
            for (int i = 0; i < frag_sizes[best_f]; i++) assembled[assembled_size++] = fragments[best_f][i];
        } else { // ordre inversé
            for (int i = frag_sizes[best_f]-1; i >= 0; i--) assembled[assembled_size++] = fragments[best_f][i];
        }
        used_fragments[best_f] = 1;
    }

    // Copier dans child
    for (int i = 0; i < dimension; i++) child[i] = assembled[i];
    child[dimension] = -1;
}
// ------------------- MUTATION -------------------

void swap_mutation(int* individual, int dimension,double mutation_rate) {
    for (int i = 0; i < dimension; i++) {
        double r = (double)rand() / RAND_MAX;
        if (r < mutation_rate) {
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

    if(argc <6) {
        printf("Usage: %s -f fichier.tsp -m ga/gadxp (default ou (population_size generations mutation_rate)\n", argv[0]);
        exit(0);
    }
    int POPULATION_SIZE = 30;
    int GENERATIONS = 1000;
    double MUTATION_RATE = 0.10;
    bool dxp;
   //Choix dxp ou ordered 
   if (strcmp(argv[4], "-ga") == 0) {
        dxp=false;
    }else{
        dxp=true;
    }
    //si choix de population_size generations et mutation_rate
    if(argc==8){
        POPULATION_SIZE=atoi(argv[5]);
        GENERATIONS=atoi(argv[6]);
        MUTATION_RATE=atof(argv[7]);
    }
    char path[256];
    snprintf(path, sizeof(path), "%s", argv[2]);
    FichierTSP* tsp = malloc(sizeof(FichierTSP));
    if (!tsp) { printf("Erreur malloc tsp\n"); return 1; }
    readTSP(path, tsp);

    if (!tsp->nodes) { printf("Erreur : tsp->nodes non initialisé\n"); return 1; }

    // printf("Fichier TSP lu : %s, dimension = %d, edge_type = %d\n", path, tsp->dimension, tsp->edge_type);

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
                if(dxp){
                    dxp_crossover(selected[i], selected[i+1], offspring[i], dimension,distance,tsp->nodes);
                    dxp_crossover(selected[i+1], selected[i], offspring[i+1], dimension,distance,tsp->nodes);
                }else{
                    ordered_crossover(selected[i], selected[i+1], offspring[i], dimension);
                    ordered_crossover(selected[i+1], selected[i], offspring[i+1], dimension);
                }
                
                
            } else {
                copy_individual(offspring[i], selected[i], dimension);
            }
        }

        // Mutation
        for (int i = 0; i < POPULATION_SIZE; i++) swap_mutation(offspring[i], dimension,MUTATION_RATE);

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

    printf("Tour ; ");
    printf("%s ; ", argv[2]);
    printf("ga ; ");
    printf("%d ; ", final_distance);
    printf("0.0");

    printf("[");
    for (int i = 0; i < dimension; i++) {
        if (i > 0) printf(",");
        printf("%d", best_individual[i]);
    }
    printf("]; \n");
    free(population);
    free(offspring);
    free(selected);
    free(fitness_values);
    free(best_individual);
    free(tsp);

    return 0;
}
