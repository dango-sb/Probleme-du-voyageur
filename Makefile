CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRC_DIR = C
BIN_DIR = exec

$(shell mkdir -p $(BIN_DIR))

SRC_COMMON = $(SRC_DIR)/fonctions_calcul.c $(SRC_DIR)/data.c
SRC_PARTIE0 = $(SRC_COMMON) $(SRC_DIR)/main_partie0.c
SRC_PARTIE1 = $(SRC_COMMON) $(SRC_DIR)/force_brute.c $(SRC_DIR)/main_partie1.c
SRC_TESTMATRICE = $(SRC_DIR)/testMatrice.c $(SRC_DIR)/demi_matrice.c $(SRC_DIR)/data.c $(SRC_DIR)/fonctions_calcul.c

EXECS = $(BIN_DIR)/main0 $(BIN_DIR)/main1 $(BIN_DIR)/testMatrice $(BIN_DIR)/testTour $(BIN_DIR)/testTSP

all: $(EXECS)

$(BIN_DIR)/main0: $(SRC_PARTIE0)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/main1: $(SRC_PARTIE1)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/testMatrice: $(SRC_TESTMATRICE)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/testTour: $(SRC_DIR)/testTour.c
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/testTSP: $(SRC_DIR)/testTSP.c
	$(CC) $(CFLAGS) $^ -o $@

# pseudos 
main0: $(BIN_DIR)/main0
main1: $(BIN_DIR)/main1
testMatrice: $(BIN_DIR)/testMatrice
testTour: $(BIN_DIR)/testTour
testTSP: $(BIN_DIR)/testTSP

clean:
	rm -f $(BIN_DIR)/* *.o

.PHONY: all clean main0 main1 testMatrice testTour testTSP
