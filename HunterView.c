#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "game.h"
#include "HunterView.h"
#include "Graph.h"

#define LENGTH_OF_PLAYS 7
#define CITY_NAME 2

static char idToAscii (PlayerID player);
static PlayerID asciiToId (char player);
static LocationID locToId (char location[]);
static char *revstrstr (char *string1, char *string2);

struct hunterView {
    Round round;
    int deathCount; //needed for score
    int hospRound;

    //make backup of pastPlays & messages so the other fns can access
    char *pastPlays;
    playerMessage messages[];
};

HunterView newHunterView (char *pastPlays, playerMessage messages[]) {
    HunterView newHV = malloc (sizeof (struct hunterView));
    newHV->pastPlays = pastPlays;    

    newHV->round = getRound (newHV);
    newHV->hospRound = -1; //unknown round;
    newHV->deathCount = 0;

    //initialising deathcount via getHealth
    int i;
    for (i=0; i<(NUM_PLAYERS-1); i++) {
        getHealth (newHV,i); 
    }

    return newHV;
}

void disposeHunterView (HunterView toBeDeleted) {
    free(toBeDeleted);
}

Round getRound (HunterView currentView) {
    assert (currentView->pastPlays != NULL);
    int turnCount = (strlen (currentView->pastPlays)+1)/(LENGTH_OF_PLAYS+1); //derp = no. of player go's
    Round returnval = turnCount/NUM_PLAYERS;

    return returnval;
}

PlayerID getCurrentPlayer (HunterView currentView) {
    PlayerID prev = -1;
    int prevASCII;
    if (strlen (currentView->pastPlays) >= LENGTH_OF_PLAYS) {
        prevASCII = currentView->pastPlays[strlen (currentView->pastPlays) - LENGTH_OF_PLAYS];
        prev = asciiToId (prevASCII);
    } else {
        prev = -1; //before game starts
    }

    PlayerID current;
    if (prev == PLAYER_DRACULA) { //dracula's the last player
        current = PLAYER_LORD_GODALMING;
    } else {
        current = prev + 1;
    }

    return current;
}

int getScore (HunterView currentView) {
    int score = GAME_START_SCORE;
    // (1.) each time dracula finishes turn
    //copy array, strtok " " & if D follows, count++
    int dracCount = 0;
    char *copyDrac = malloc (sizeof (char) * (strlen (currentView->pastPlays) + 1)); //copy of pastPlays
    strcpy (copyDrac, currentView->pastPlays);
    assert (strcmp (copyDrac, currentView->pastPlays) == 0);
    char *tokenPtr;
    tokenPtr = strtok (copyDrac, " ");
    while (tokenPtr != NULL) {
        if (strncmp (tokenPtr, "D", 1) == 0) {
            dracCount++;
        }

        tokenPtr = strtok (NULL, " ");
    }

    score = score - (SCORE_LOSS_DRACULA_TURN*dracCount);
    // (2.) each hunter hospitalised
    score = score - (SCORE_LOSS_HUNTER_HOSPITAL*currentView->deathCount);

    // (3.) each time vampire matures
    //copy array, strtok "V", if ' ' or '\0' follows, count++
    int vampCount = 0;
    char *copyVamp = malloc (sizeof (char) * strlen (currentView->pastPlays) + 1);
    strcpy (copyVamp, currentView->pastPlays);

    tokenPtr = strtok (copyVamp, "V");
    while (tokenPtr != NULL) {
        if (strncmp (tokenPtr, ". ", 2) == 0) {
            vampCount++;
        }
        tokenPtr = strtok (NULL, "V");
    }

    //null/last in string case
    char *ptr = revstrstr (currentView->pastPlays, "V.");
    if (ptr != '\0') {
        if (ptr[2] == '\0') {
            vampCount++;
        }
    }

    score = score - (SCORE_LOSS_VAMPIRE_MATURES*vampCount);
    return score;
}

 
//Get the current health points for a given player
int getHealth (HunterView currentView, PlayerID player) {
    //alt. implementation: read each character of pastPlays in order
    int health;

    if (player == PLAYER_DRACULA) {
        health = GAME_START_BLOOD_POINTS;
        if (getRound (currentView) == 0) {
            return health;
        }
        // (A.) find all hunter names, if elem [3-6] == 'D'
        int hunterEncounters = 0;

        char *copySpaces = malloc (sizeof (char) * strlen (currentView->pastPlays) + 1);
        strcpy (copySpaces, currentView->pastPlays);

        char *tokenPtr = strtok (copySpaces, " ");
        while (tokenPtr != NULL) {
            if ((strncmp (tokenPtr, "G", 1) == 0)
             || (strncmp (tokenPtr, "S", 1) == 0)
             || (strncmp (tokenPtr, "H", 1) == 0)
             || (strncmp (tokenPtr, "M", 1) == 0)) { //all hunters
                if ((tokenPtr[3] == 'D')
                 || (tokenPtr[4] == 'D')
                 || (tokenPtr[5] == 'D')
                 || (tokenPtr[6] == 'D')) { //last 4 elements before space = D 
                    hunterEncounters++;
                }
            }
            tokenPtr = strtok (NULL, " ");
        }
        health = health - (LIFE_LOSS_HUNTER_ENCOUNTER*hunterEncounters);

        // (B.) end turn at sea
        int atSea = 0;
        char *currentString = malloc (sizeof (char) * 5); //some extra space
        currentString[2] = '\0';

        LocationID prev = UNKNOWN_LOCATION;
        LocationID current = UNKNOWN_LOCATION;

        strcpy (copySpaces, currentView->pastPlays);
        tokenPtr = strtok (copySpaces, " ");
        while (tokenPtr != NULL) {
            if (strncmp (tokenPtr, "D", 1) == 0) { //found Drac's turns
                prev = current;
                currentString[0] = tokenPtr[1];
                currentString[1] = tokenPtr[2]; //location chars
                current = locToId (currentString);
            }
            //check for double backs T_T" *cries*, consecutive DS & DD1
            if (((current <= BLACK_SEA) && (current >= NORTH_SEA)) || (prev == SEA_UNKNOWN)) {
                if (current == DOUBLE_BACK_1) {
                    atSea += 2;
                } else {
                    atSea++;
                }
            }
            tokenPtr = strtok (NULL, " ");
        }
 

        health = health - (LIFE_LOSS_SEA*atSea);

        // (C.) end turn at castle
        int atCastle = 0;
        strcpy (copySpaces, currentView->pastPlays);
        tokenPtr = strtok (copySpaces, " ");
        while (tokenPtr != NULL) {
            if (strncmp (tokenPtr, "DCD", 3) == 0) {
                atCastle++;
            }
            tokenPtr = strtok (NULL, " ");
        }
        //check for DOUBLE BACKS again..

        health = health + (LIFE_GAIN_CASTLE_DRACULA*atCastle);
        free (copySpaces);
        return health;
    } else {
        health = GAME_START_HUNTER_LIFE_POINTS;
        if (getRound (currentView) == 0) {
            return health;
        }

        // (D-F.):
        char *copySpaces = malloc (sizeof (char) * strlen (currentView->pastPlays) + 1);
        strcpy (copySpaces, currentView->pastPlays);

        char playerString[2];
        playerString[0] = idToAscii (player);
        playerString[1] = '\0';

        char *currentString = malloc (sizeof (char) * 5); //some extra space
        currentString[2] = '\0';
        LocationID prev = UNKNOWN_LOCATION;
        LocationID current = UNKNOWN_LOCATION;

        char *tokenPtr = strtok (copySpaces, " ");

        int turnCount = 1; //counts no. of player gos
        while (tokenPtr != NULL) {
            if (strncmp (tokenPtr, playerString, 1) == 0) { //wanted player
                // (D.) encountering traps
                if (tokenPtr[3] == 'T') {
                    health = health - LIFE_LOSS_TRAP_ENCOUNTER;
                    if (health <= 0) {
                        currentView->deathCount++;
                        health = GAME_START_HUNTER_LIFE_POINTS;
                        currentView->hospRound = turnCount/NUM_PLAYERS;
                    }
                }
                if (tokenPtr[4] == 'T') {
                    health = health - LIFE_LOSS_TRAP_ENCOUNTER;
                    if (health <= 0) {
                        currentView->deathCount++;
                        health = GAME_START_HUNTER_LIFE_POINTS;
                        currentView->hospRound = turnCount/NUM_PLAYERS;
                    }
                }

                if (tokenPtr[5] == 'T') {
                    health = health - LIFE_LOSS_TRAP_ENCOUNTER;
                    if (health <= 0) {
                        currentView->deathCount++;
                        health = GAME_START_HUNTER_LIFE_POINTS;
                        currentView->hospRound = turnCount/NUM_PLAYERS;
                    }
                }
                if (tokenPtr[6] == 'T') {
                    health = health - LIFE_LOSS_TRAP_ENCOUNTER;
                    if (health <= 0) {
                        currentView->deathCount++;
                        health = GAME_START_HUNTER_LIFE_POINTS;
                        currentView->hospRound = turnCount/NUM_PLAYERS;
                    }
                }

                // (E.) drac encounter
                if ((tokenPtr[3] == 'D')
                 || (tokenPtr[4] == 'D')
                 || (tokenPtr[5] == 'D')
                 || (tokenPtr[6] == 'D')) {
                    health = health - LIFE_LOSS_DRACULA_ENCOUNTER;
                    if (health <= 0) {
                        currentView->deathCount++;
                        health = GAME_START_HUNTER_LIFE_POINTS;
                        currentView->hospRound = turnCount/NUM_PLAYERS;
                    }
                }

                // (F.) consecutive cities found
                prev = current;
                currentString[0] = tokenPtr[1];
                currentString[1] = tokenPtr[2];
                current = locToId (currentString);
                if ((prev == current) && (prev != UNKNOWN_LOCATION)) {
                    if (health <= 0) {
                        health = GAME_START_HUNTER_LIFE_POINTS;
                        currentView->hospRound = turnCount/NUM_PLAYERS;

                    } else if (turnCount/NUM_PLAYERS-1 == currentView->hospRound) {
                        //DIED LAST ROUND (test4 requires)
                    } else {
                        health = health + LIFE_GAIN_REST;
                    }
                }
                prev = current;
            }
            tokenPtr = strtok (NULL, " ");
            turnCount++;
        }

        if (health > 9) {
            health = GAME_START_HUNTER_LIFE_POINTS;
        }
        if (health <= 0) {
            health = GAME_START_HUNTER_LIFE_POINTS;
        }
        free (copySpaces);
        return health;
    }
}


LocationID getLocation (HunterView currentView, PlayerID player) {
    if (strlen (currentView->pastPlays) == 0) {
        return UNKNOWN_LOCATION;
    }

    if ((player != PLAYER_DRACULA) && (getRound (currentView) == currentView->hospRound + 1)) {

    } else if ((player != PLAYER_DRACULA) && (getRound (currentView) == currentView->hospRound)) {

        return ST_JOSEPH_AND_ST_MARYS;
    }
    if ((player != PLAYER_DRACULA) && ((getRound (currentView)-1) == currentView->hospRound) && (getCurrentPlayer (currentView) == PLAYER_LORD_GODALMING)) {
        //this exception is required by the tests

        return ST_JOSEPH_AND_ST_MARYS;
    }

    if (strlen (currentView->pastPlays) >= LENGTH_OF_PLAYS) {
        //find last occurrence of wanted substring
        char string1[3] = {' ', idToAscii(player), '\0'}; //e.g. " G", " D"
        char string2[2] = {idToAscii(player),'\0'}; //e.g. "G", "D"

        char *ptr = revstrstr (currentView->pastPlays, string1);
        if (ptr == NULL) {
            ptr = revstrstr (currentView->pastPlays, string2);
            if (ptr == NULL) {
                return UNKNOWN_LOCATION;
            }
        }
        assert (ptr != NULL);

        //copy the found bit/substring to a new string: round
        char *round = malloc (sizeof (char)*5); //bit more just in case
        strncpy (round, ptr, 4); //copy over 3 characters

        if (round == '\0') { //player not found in pastPlays
            return UNKNOWN_LOCATION;
        }
        //now chars->locationID
        char location[3];
        if (round[0] == ' ') {
            location[0] = round[2];
            location[1] = round[3];
            location[2] = '\0';
        } else {
            location[0] = round[1];
            location[1] = round[2];
            location[2] = '\0';
        }
        free (round);
        return locToId(location);
    } else { //zero players have played 
        return UNKNOWN_LOCATION;
    }
}

static char *revstrstr (char *string1, char *string2) {
    int length1 = strlen (string1);
    int length2 = strlen (string2);
    char *i;

    for (i=string1+length1-length2; i>=string1; i--) {
        if (strncmp (i, string2, length2) == 0) {
            return i;
        }
    }
    return NULL;
}

void getHistory (HunterView currentView, PlayerID player,LocationID trail[TRAIL_SIZE]) {
    if (strlen (currentView->pastPlays) <= (TRAIL_SIZE*LENGTH_OF_PLAYS)) {
        trail[0] = getLocation (currentView, player);
        trail[1] = UNKNOWN_LOCATION;
        trail[2] = UNKNOWN_LOCATION;
        trail[3] = UNKNOWN_LOCATION;
        trail[4] = UNKNOWN_LOCATION;
        trail[5] = UNKNOWN_LOCATION;
    } else {
        char location[3];
        location[2] = '\0';

        //string length difference between current player and wanted player
        int difference = 0;
        PlayerID prevPlayer;
        if (getCurrentPlayer (currentView) == PLAYER_LORD_GODALMING) {
            prevPlayer = PLAYER_DRACULA;
        } else {
            prevPlayer = getCurrentPlayer (currentView) - 1;
        }
        if (prevPlayer == PLAYER_LORD_GODALMING) {
            prevPlayer = 5;
            if (player == PLAYER_LORD_GODALMING) {
                player = 5;
            } else {
                difference = prevPlayer - player;
            }
        } else if (player > prevPlayer) {
            difference = player - prevPlayer;
        } else {
            difference = prevPlayer - player;
        }
        difference = difference * (LENGTH_OF_PLAYS + 1);

        int length = strlen (currentView->pastPlays);
        int start = (length - 6) - difference;
        int loopTimes = (length - difference) / (TRAIL_SIZE * LENGTH_OF_PLAYS);

        if (loopTimes == 0) {
            location[0] = currentView->pastPlays[start];
            location[1] = currentView->pastPlays[start + 1];
            trail[0] = locToId (location);
            trail[1] = UNKNOWN_LOCATION;
            trail[2] = UNKNOWN_LOCATION;
            trail[3] = UNKNOWN_LOCATION;
            trail[4] = UNKNOWN_LOCATION;
            trail[5] = UNKNOWN_LOCATION;
        } else if (loopTimes < TRAIL_SIZE) { //0 to 5
            int i = 0;
            while (i<=loopTimes) {
                location[0] = currentView->pastPlays[start]; //first letter
                location[1] = currentView->pastPlays[start + 1]; //second letter
                //write to trail array
                trail[i] = locToId(location);

                start = start - ((LENGTH_OF_PLAYS + 1)*NUM_PLAYERS); //spaces
                i++;
            }

            //fill rest of trail with -1
            int j = 0;
            while (j<(TRAIL_SIZE-loopTimes)) {
                trail[TRAIL_SIZE-j] = UNKNOWN_LOCATION;
                j++;
            }
        } else { //do default 6 times
            int i = 0;
            while (i<TRAIL_SIZE) {
                location[0] = currentView->pastPlays[start]; //first letter
                location[1] = currentView->pastPlays[start + 1]; //second letter
                //write to trail array
                trail[i] = locToId(location);

                start = start - (LENGTH_OF_PLAYS + 1);
                i++;
            }
        }
    }
}


LocationID *connectedLocations (HunterView currentView, int *numLocations, LocationID from, 
                                PlayerID player, Round round, int road, int rail, int sea) {
//must fill in: numLocations, return array of adj vertices
//player, round, type?
    Graph map = newGraph ();

    int locs[NUM_LOCATIONS] = {0};
    if (road == TRUE) {
        canReachInN (map, from, ROAD, 1, locs); //canReachInN modifies locs directly
    } else if ((rail == TRUE) && (player != PLAYER_DRACULA)) { //can travel by rail
        if ((player+round)%4 == 0) {
            canReachInN (map, from, RAIL, 0, locs);
        } else if ((player+round)%4 == 1) {
            canReachInN (map, from, RAIL, 1, locs);
        } else if ((player+round)%4 == 2) {
            canReachInN (map, from, RAIL, 2, locs);
        } else if ((player+round)%4 == 3) {
            canReachInN (map, from, RAIL, 3, locs);
        }  
    } else if (rail == TRUE) { //can't travel by rail

    } else if (sea == TRUE) {
        canReachInN (map, from, SEA, 1, locs);
    } else {
        canReachInN (map, from, ANY, 1, locs);
    }

    //initialise size of array..locations
    int i;
    int j = 0;
    for (i=0; i<NUM_LOCATIONS; i++) {
        if (locs[i] == 1) {
            j++;
        }
    }
    *numLocations = j;
    //go through locs[], if 1, put in return array
    int locations[j];
    int counter = 0; //for locations
    for (i=0; i<NUM_LOCATIONS; i++) {
        if (locs[i] == TRUE) {
            locations[counter] = i;
            counter++;
        }
    }
    LocationID *returnval = locations;
    destroyGraph(map);
    return returnval;
}

static char idToAscii (PlayerID player) {
    int returnval = ' ';
    if (player == PLAYER_LORD_GODALMING) {
        returnval = 'G';
    } else if (player == PLAYER_DR_SEWARD) {
        returnval = 'S';
    } else if (player == PLAYER_VAN_HELSING) {
        returnval = 'H';
    } else if (player == PLAYER_MINA_HARKER) {
        returnval = 'M';
    } else if (player == PLAYER_DRACULA) {
        returnval = 'D';
    }
    return returnval;
}

static PlayerID asciiToId (char player) {
    PlayerID returnval = 0;
    if (player == 'G') {
        returnval = PLAYER_LORD_GODALMING;
    } else if (player == 'S') {
        returnval = PLAYER_DR_SEWARD;
    } else if (player == 'H') {
        returnval = PLAYER_VAN_HELSING;
    } else if (player == 'M') {
        returnval = PLAYER_MINA_HARKER;
    } else if (player == 'D') {
        returnval = PLAYER_DRACULA;
    }
    return returnval;
}

static LocationID locToId (char location[]) {
    LocationID returnval = UNKNOWN_LOCATION;
    if (strcmp (location, "..") == 0) {
        return UNKNOWN_LOCATION;
    }

    //fill array w/ ascii
    char *array[NUM_LOCATIONS] = {
    //land
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
 
    //go through array, strcmp location[] & elements
    int i;
    for (i=0; i<NUM_LOCATIONS; i++) {
        if (strcmp (location, array[i]) == 0) {
            break;
        }
    }
    if (i == 80) {
        printf ("string cannot be converted to valid LocationID!\n");
        return -2; //error
    }
    returnval = i;
    return returnval;
}

char *idToLoc (LocationID location) {
    if (location == -1) {
        char *unknown = "..";
        return unknown;
    }
    char *array[NUM_LOCATIONS] = {
        //land
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

    if (location >= NUM_LOCATIONS) {
        printf ("LocationID cannot be converted to valid string! Returning NULL.\n");
        return NULL;
    } else {
        return array[location];
    }
}

//display messages
