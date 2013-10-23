#include "game.h"
#include "HunterView.h"
#include "hunter.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void makeRandomMove (HunterView gameState, int road, int rail, int sea);

void decideMove (HunterView gameState) {
    LocationID recentEncounters[TRAIL_SIZE] = {-2}; //-2 is an invalid LocationID
    getHistory (gameState, PLAYER_DRACULA, recentEncounters);
    if (recentEncounters[0] == UNKNOWN_LOCATION) {

    } else if (recentEncounters[0] == CITY_UNKNOWN) {
        makeRandomMove (gameState, 1, 1, 0);
    } else if (recentEncounters[0] == SEA_UNKNOWN) {
        makeRandomMove (gameState, 0, 0, 1);
    } else {
        PlayerID us = getCurrentPlayer (gameState);
        int *size = malloc (sizeof (int));
        LocationID *adjacent = connectedLocations (gameState, size, getLocation (gameState, us), us, getRound (gameState), 0, 0, 0);
        int i;
        for (i=0; i<*size; i++) {
            if (adjacent[i] == recentEncounters[0]) { //check this line works
                registerBestPlay (idToLoc(recentEncounters[0]), "");
                break;
            }
        }
        if (i == *size) { //not adjacent
            //change to shortest path
            makeRandomMove (gameState, 0, 0, 0);
        } 
    }
}

static void makeRandomMove (HunterView gameState, int road, int rail, int sea) {
    PlayerID us = getCurrentPlayer (gameState);
    int *size = malloc (sizeof (int));
    LocationID *adjacent = connectedLocations (gameState, size, getLocation (gameState, us), us, getRound (gameState), road, rail, sea);

    srand (time (NULL));
    int random = rand () % (*size);

    registerBestPlay (idToLoc (adjacent[random]), "");
}
