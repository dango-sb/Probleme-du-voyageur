CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRC_DIR = C
BIN_DIR = exec

$(shell mkdir -p $(BIN_DIR))

SRC_COMMON = $(SRC_DIR)/fonctions_calcul.c $(SRC_DIR)/data.c
SRC_PARTIE0 = $(SRC_COMMON) $(SRC_DIR)/main_partie0.c
SRC_PARTIE1 = $(SRC_COMMON) $(SRC_DIR)/force_brute.c $(SRC_DIR)/demi_matrice.c $(SRC_DIR)/main_partie1.c
SRC_PARTIE2 = $(SRC_COMMON) $(SRC_DIR)/demi_matrice.c $(SRC_DIR)/plus_proche_voisin.c $(SRC_DIR)/random_walk.c $(SRC_DIR)/2opt.c $(SRC_DIR)/main_partie2.c 
SRC_PARTIE3 = $(SRC_COMMON) $(SRC_DIR)/tsp_tri_light.c
MAIN = $(SRC_DIR)/main.c
EXECS = $(BIN_DIR)/main0 $(BIN_DIR)/main1 $(BIN_DIR)/main2 $(BIN_DIR)/main3	$(BIN_DIR)/main

all: $(EXECS)

$(BIN_DIR)/main0: $(SRC_PARTIE0)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/main1: $(SRC_PARTIE1)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/main2: $(SRC_PARTIE2)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/main3: $(SRC_PARTIE3)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/main: $(MAIN)
	$(CC) $(CFLAGS) $^ -o $@

# pseudos 
main0: $(BIN_DIR)/main0
main1: $(BIN_DIR)/main1
main2: $(BIN_DIR)/main2
main3: $(BIN_DIR)/main3
main: $(BIN_DIR)/main	# main principal 

clean:
	rm -f $(BIN_DIR)/* *.o

.PHONY: all clean main0 main1 main2 main3 main