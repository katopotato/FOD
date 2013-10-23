#include "game.h"
#include "HunterView.h"
#include "hunter.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void makeRandomMove (HunterView gameState);

void decideMove (HunterView gameState) {
    if(getCurrentPlayer(gameState) == PLAYER_LORD_GODALMING) {
        if(getLocation(gameState, PLAYER_LORD_GODALMING) == UNKNOWN_LOCATION) {
            //pick a starting position for 1st turn
            registerBestPlay("GW", "hi");
        } else {
            char *array[NUM_LOCATIONS] = {
             "AL", "AM", "AT", "BA", "BI", "BE", "BR", "BO", "BU", "BC", 
             "BD", "CA", "CG", "CD", "CF", "CO", "CN", "DU", "ED", "FL",
             "FR", "GA", "GW", "GE", "GO", "GR", "HA", "JM", "KL", "LE",
             "LI", "LS", "LV", "LO", "MA", "MN", "MR", "MI", "MU", "NA",
             "NP", "NU", "PA", "PL", "PR", "RO", "SA", "SN", "SR", "SJ",
             "SO", "ST", "SW", "SZ", "TO", "VA", "VR", "VE", "VI", "ZA",
             "ZU",
            //seas
             "NS", "EC", "IS", "AO", "BB", "MS", "TS", "IO", "AS", "BS",
            //dracula
            "C?", "S?", "HI", "D1", "D2", "D3", "D4", "D5", "TP"};
            int *size = malloc (sizeof (int));
            srand (time (NULL));
            int random = (rand() % (*size-0) + 0);
            LocationID *adjacentGold = connectedLocations (gameState, size, getLocation (gameState, PLAYER_LORD_GODALMING), PLAYER_LORD_GODALMING, getRound (gameState), 1, 1, 1);
            registerBestPlay (array [adjacentGold[random]], "");
        }
    } else if (getCurrentPlayer(gameState) == PLAYER_DR_SEWARD){
        if(getLocation(gameState, PLAYER_DR_SEWARD) == UNKNOWN_LOCATION) {
            registerBestPlay("KL", "");
        } else {
        char *array[NUM_LOCATIONS] = {
             "AL", "AM", "AT", "BA", "BI", "BE", "BR", "BO", "BU", "BC", 
             "BD", "CA", "CG", "CD", "CF", "CO", "CN", "DU", "ED", "FL",
             "FR", "GA", "GW", "GE", "GO", "GR", "HA", "JM", "KL", "LE",
             "LI", "LS", "LV", "LO", "MA", "MN", "MR", "MI", "MU", "NA",
             "NP", "NU", "PA", "PL", "PR", "RO", "SA", "SN", "SR", "SJ",
             "SO", "ST", "SW", "SZ", "TO", "VA", "VR", "VE", "VI", "ZA",
             "ZU",
            //seas
             "NS", "EC", "IS", "AO", "BB", "MS", "TS", "IO", "AS", "BS",
            //dracula
            "C?", "S?", "HI", "D1", "D2", "D3", "D4", "D5", "TP"};
    int *size = malloc (sizeof (int));
    srand (time (NULL));
    int random = (rand() % (*size-0) + 0);
            LocationID *adjacentSeward = connectedLocations (gameState, size, getLocation (gameState, PLAYER_LORD_GODALMING), PLAYER_LORD_GODALMING, getRound (gameState), 1, 1, 1);
            registerBestPlay (array [adjacentSeward[random]], "");
        }
    } else if (getCurrentPlayer (gameState) == PLAYER_VAN_HELSING) {
        if(getLocation(gameState, PLAYER_VAN_HELSING) == UNKNOWN_LOCATION) { //case for the first round
            registerBestPlay("AT", "i hate you");
        } else {
        char *array[NUM_LOCATIONS] = {
             "AL", "AM", "AT", "BA", "BI", "BE", "BR", "BO", "BU", "BC", 
             "BD", "CA", "CG", "CD", "CF", "CO", "CN", "DU", "ED", "FL",
             "FR", "GA", "GW", "GE", "GO", "GR", "HA", "JM", "KL", "LE",
             "LI", "LS", "LV", "LO", "MA", "MN", "MR", "MI", "MU", "NA",
             "NP", "NU", "PA", "PL", "PR", "RO", "SA", "SN", "SR", "SJ",
             "SO", "ST", "SW", "SZ", "TO", "VA", "VR", "VE", "VI", "ZA",
             "ZU",
            //seas
             "NS", "EC", "IS", "AO", "BB", "MS", "TS", "IO", "AS", "BS",
            //dracula
            "C?", "S?", "HI", "D1", "D2", "D3", "D4", "D5", "TP"};
    int *size = malloc (sizeof (int));
    srand (time (NULL));
    int random = (rand() % (*size-0) + 0);
            LocationID *adjacentHelsing = connectedLocations (gameState, size, getLocation (gameState, PLAYER_VAN_HELSING), PLAYER_VAN_HELSING, getRound (gameState), 1, 1, 1);
            registerBestPlay (array [adjacentHelsing[random]], "");
            }
    }else if(getCurrentPlayer(gameState) == PLAYER_MINA_HARKER) {
        if (getLocation(gameState, PLAYER_MINA_HARKER) == UNKNOWN_LOCATION) {
            registerBestPlay("MA", "go die"); 
        } else {
        char *array[NUM_LOCATIONS] = {
             "AL", "AM", "AT", "BA", "BI", "BE", "BR", "BO", "BU", "BC", 
             "BD", "CA", "CG", "CD", "CF", "CO", "CN", "DU", "ED", "FL",
             "FR", "GA", "GW", "GE", "GO", "GR", "HA", "JM", "KL", "LE",
             "LI", "LS", "LV", "LO", "MA", "MN", "MR", "MI", "MU", "NA",
             "NP", "NU", "PA", "PL", "PR", "RO", "SA", "SN", "SR", "SJ",
             "SO", "ST", "SW", "SZ", "TO", "VA", "VR", "VE", "VI", "ZA",
             "ZU",
            //seas
             "NS", "EC", "IS", "AO", "BB", "MS", "TS", "IO", "AS", "BS",
            //dracula
            "C?", "S?", "HI", "D1", "D2", "D3", "D4", "D5", "TP"};
    int *size = malloc (sizeof (int));
    srand (time (NULL));
    int random = (rand() % (*size-0) + 0);
            LocationID *adjacentHarker = connectedLocations(gameState, size, getLocation(gameState, PLAYER_MINA_HARKER), PLAYER_MINA_HARKER, getRound(gameState),1,1,1);
            registerBestPlay (array [adjacentHarker[random]], "I'm coming to catch you ..");
        }
    }

}

