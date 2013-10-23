#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "cities.h"

typedef struct vNode *VList;

struct vNode { 
    Location v; //ALICANTE etc
    Transport type; //ROAD, SEA, ANY
    VList next; 
};  

struct GraphRep { 
    int nV; //#vertices
    int nE[NUM_TRANSPORT]; //counter for number of edges of each type    
    VList connections[NUM_MAP_LOCATIONS]; //array of lists 
}; 


static void makeMap(Graph g);
static void addLink(Graph g, Location start, Location end, Transport type);


Graph newGraph() { 
    int i; 
    Graph g = malloc(sizeof(struct GraphRep));
    assert(g != NULL);
    g->nV = NUM_MAP_LOCATIONS;
    for (i = 0; i < g->nV; i++) { 
        g->connections[i] = NULL;  
    }
     
    g->nE[ROAD] = 0;
    g->nE[RAIL] = 0;
    g->nE[SEA] = 0; 
    makeMap(g);
    return g; 
} 

void destroyGraph(Graph g) {
    int i;
    VList curr;
    VList next;
    assert(g != NULL);
    assert(g->connections != NULL);
  
    for (i = 0; i < g->nV; i++) {
        curr = g->connections[i];
        while(curr !=NULL) {
            next = curr->next;
            free(curr);
            curr=next;
        }
    }
    free(g);
}   


static void addLink(Graph g, Location start, Location end, Transport type) {
    //VList startNode = malloc (sizeof (struct vNode));
    VList endNode = malloc (sizeof (struct vNode));    

    //startNode->v = start;
    endNode->v = end;

    //startNode->next = NULL;
    endNode->next = NULL;

    //startNode->type = type;
    endNode->type = type;

    //connections
    VList currStart = g->connections[start];
    int dupe = 0;
    if (currStart != NULL) {
        while (currStart->next != NULL) {
            if (currStart->v == end) {
                dupe = 1;
            }
            currStart = currStart->next;
        }
        if (dupe == 1) {

        } else {
            currStart->next = endNode;
            g->nE[type]++;
        }
    } else {
        g->connections[start] = endNode;
        
    }
    /*
    VList currEnd = g->connections[end];
    if (currEnd != NULL) {
        while (currEnd->next != NULL) {
            currEnd = currEnd->next;
        }
        currEnd->next = startNode;    
    } else {
        g->connections[end] = startNode;
    }
    */
    g->nE[type]++;
}

static void makeMap(Graph g) {
    //### ROAD Connections ###

    //ATHENS
    addLink(g, ATHENS, VALONA, ROAD);

    //VALONA
    addLink(g, VALONA, ATHENS, ROAD);
    addLink(g, VALONA, SALONICA, ROAD);
    addLink(g, VALONA, SOFIA, ROAD);
    addLink(g, VALONA, SARAJEVO, ROAD);

    //SALONICA  
    addLink(g, SALONICA, VALONA, ROAD);
    addLink(g, SALONICA, SOFIA, ROAD);

    //SOFIA
    addLink(g, SOFIA, VARNA, ROAD);
    addLink(g, SOFIA, SALONICA, ROAD);
    addLink(g, SOFIA, VALONA, ROAD);
    addLink(g, SOFIA, SARAJEVO, ROAD);
    addLink(g, SOFIA, BELGRADE, ROAD);
    addLink(g, SOFIA, BUCHAREST, ROAD);

    //VARNA
    addLink(g, VARNA, SOFIA, ROAD);
    addLink(g, VARNA, CONSTANTA, ROAD);

    //CONSTANTA
    addLink(g, CONSTANTA, VARNA, ROAD);
    addLink(g, CONSTANTA, BUCHAREST, ROAD);
    addLink(g, CONSTANTA, GALATZ, ROAD);

    //GALATZ
    addLink(g, GALATZ, CONSTANTA, ROAD);
    addLink(g, GALATZ, BUCHAREST, ROAD);
    addLink(g, GALATZ, KLAUSENBURG, ROAD);
    addLink(g, GALATZ, CASTLE_DRACULA, ROAD);

    //BUCHAREST
    addLink(g, BUCHAREST, GALATZ, ROAD);
    addLink(g, BUCHAREST, CONSTANTA, ROAD);
    addLink(g, BUCHAREST, SOFIA, ROAD);
    addLink(g, BUCHAREST, BELGRADE, ROAD);
    addLink(g, BUCHAREST, KLAUSENBURG, ROAD);

    //BELGRADE
    addLink(g, BELGRADE, SARAJEVO, ROAD);
    addLink(g, BELGRADE, ST_JOSEPH_AND_ST_MARYS, ROAD);
    addLink(g, BELGRADE, SZEGED, ROAD);
    addLink(g, BELGRADE, KLAUSENBURG, ROAD);
    addLink(g, BELGRADE, BUCHAREST, ROAD);
    addLink(g, BELGRADE, SOFIA, ROAD);

    //SARAJEVO
    addLink(g, SARAJEVO, VALONA, ROAD);
    addLink(g, SARAJEVO, ZAGREB, ROAD);
    addLink(g, SARAJEVO, ST_JOSEPH_AND_ST_MARYS, ROAD);
    addLink(g, SARAJEVO, BELGRADE, ROAD);
    addLink(g, SARAJEVO, SOFIA, ROAD);

    //ST_JOSEPH_AND_ST_MARYS
    addLink(g, ST_JOSEPH_AND_ST_MARYS, SARAJEVO, ROAD);
    addLink(g, ST_JOSEPH_AND_ST_MARYS, ZAGREB, ROAD);
    addLink(g, ST_JOSEPH_AND_ST_MARYS, SZEGED, ROAD);
    addLink(g, ST_JOSEPH_AND_ST_MARYS, BELGRADE, ROAD);

    //SZEGED
    addLink(g, SZEGED, ST_JOSEPH_AND_ST_MARYS, ROAD);
    addLink(g, SZEGED, ZAGREB, ROAD);
    addLink(g, SZEGED, BUDAPEST, ROAD);
    addLink(g, SZEGED, KLAUSENBURG, ROAD);
    addLink(g, SZEGED, BELGRADE, ROAD);

    //KLAUSENBURG
    addLink(g, KLAUSENBURG, BELGRADE, ROAD);
    addLink(g, KLAUSENBURG, SZEGED, ROAD);
    addLink(g, KLAUSENBURG, BUDAPEST, ROAD);
    addLink(g, KLAUSENBURG, CASTLE_DRACULA, ROAD);
    addLink(g, KLAUSENBURG, GALATZ, ROAD);
    addLink(g, KLAUSENBURG, BUCHAREST, ROAD);

    //CASTLE_DRACULA
    addLink(g, CASTLE_DRACULA, GALATZ, ROAD);
    addLink(g, CASTLE_DRACULA, KLAUSENBURG, ROAD);

    //ZAGREB
    addLink(g, ZAGREB, SARAJEVO, ROAD);
    addLink(g, ZAGREB, MUNICH, ROAD);
    addLink(g, ZAGREB, VIENNA, ROAD);
    addLink(g, ZAGREB, BUDAPEST, ROAD);
    addLink(g, ZAGREB, SZEGED, ROAD);
    addLink(g, ZAGREB, ST_JOSEPH_AND_ST_MARYS, ROAD);

    //BUDAPEST
    addLink(g, BUDAPEST, ZAGREB, ROAD);
    addLink(g, BUDAPEST, VIENNA, ROAD);
    addLink(g, BUDAPEST,KLAUSENBURG, ROAD);
    addLink(g, BUDAPEST, SZEGED, ROAD);

    //VIENNA
    addLink(g, VIENNA, ZAGREB, ROAD);
    addLink(g, VIENNA, MUNICH, ROAD);
    addLink(g, VIENNA, PRAGUE, ROAD);
    addLink(g, VIENNA, BUDAPEST, ROAD);

    //PRAGUE 
    addLink(g, PRAGUE, NUREMBURG, ROAD);
    addLink(g, PRAGUE, BERLIN, ROAD);
    addLink(g, PRAGUE, VIENNA, ROAD);
    
    //BERLIN
    addLink(g, BERLIN, LEIPZIG, ROAD);
    addLink(g, BERLIN, HAMBURG, ROAD);
    addLink(g, BERLIN, PRAGUE, ROAD);

    //LEIPZIG
    addLink(g, LEIPZIG, FRANKFURT, ROAD);
    addLink(g, LEIPZIG, COLOGNE, ROAD);
    addLink(g, LEIPZIG, HAMBURG, ROAD);
    addLink(g, LEIPZIG, BERLIN, ROAD);
    addLink(g, LEIPZIG, NUREMBURG, ROAD);

    //FRANKFURT
    addLink(g, FRANKFURT, STRASBOURG, ROAD);
    addLink(g, FRANKFURT, COLOGNE, ROAD);
    addLink(g, FRANKFURT, LEIPZIG, ROAD);
    addLink(g, FRANKFURT, NUREMBURG, ROAD);

    //NUREMBURG
    addLink(g, NUREMBURG, STRASBOURG, ROAD);
    addLink(g, NUREMBURG, FRANKFURT, ROAD);
    addLink(g, NUREMBURG, LEIPZIG, ROAD);
    addLink(g, NUREMBURG, PRAGUE, ROAD);
    addLink(g, NUREMBURG, MUNICH, ROAD);

    //MUNICH
    addLink(g, MUNICH, MILAN, ROAD);
    addLink(g, MUNICH, ZURICH, ROAD);
    addLink(g, MUNICH, STRASBOURG, ROAD);
    addLink(g, MUNICH, NUREMBURG, ROAD);
    addLink(g, MUNICH, VIENNA, ROAD);
    addLink(g, MUNICH, ZAGREB, ROAD);
    addLink(g, MUNICH, VENICE, ROAD);

    //VENICE
    addLink(g, VENICE, FLORENCE, ROAD);
    addLink(g, VENICE, GENOA, ROAD);
    addLink(g, VENICE, MILAN, ROAD);
    addLink(g, VENICE, MUNICH, ROAD);

    //FLORENCE
    addLink(g, FLORENCE, GENOA, ROAD);
    addLink(g, FLORENCE, VENICE, ROAD);
    addLink(g, FLORENCE, ROME, ROAD);

    //ROME
    addLink(g, ROME, FLORENCE, ROAD);
    addLink(g, ROME, NAPLES, ROAD);
    addLink(g, ROME, BARI, ROAD);

    //NAPLES
    addLink(g, NAPLES, ROME, ROAD);
    addLink(g, NAPLES, BARI, ROAD);

    //BARI
    addLink(g, BARI, NAPLES, ROAD);
    addLink(g, BARI, ROME, ROAD);

    //GENOA
    addLink(g, GENOA, VENICE, ROAD);
    addLink(g, GENOA, FLORENCE, ROAD);
    addLink(g, GENOA, MILAN, ROAD);
    addLink(g, GENOA, MARSEILLES, ROAD);

    //MILAN
    addLink(g, MILAN, GENOA, ROAD);
    addLink(g, MILAN, VENICE, ROAD);
    addLink(g, MILAN, MARSEILLES, ROAD);
    addLink(g, MILAN, ZURICH, ROAD);
    addLink(g, MILAN, MUNICH, ROAD);

    //ZURICH
    addLink(g, ZURICH, MILAN, ROAD);
    addLink(g, ZURICH, MUNICH, ROAD);
    addLink(g, ZURICH, GENEVA, ROAD);
    addLink(g, ZURICH, MARSEILLES, ROAD);
    addLink(g, ZURICH, STRASBOURG, ROAD);

    //STRASBOURG
    addLink(g, STRASBOURG, ZURICH, ROAD);
    addLink(g, STRASBOURG, MUNICH, ROAD);
    addLink(g, STRASBOURG, GENEVA, ROAD);
    addLink(g, STRASBOURG, NUREMBURG, ROAD);
    addLink(g, STRASBOURG, FRANKFURT, ROAD);
    addLink(g, STRASBOURG, COLOGNE, ROAD);
    addLink(g, STRASBOURG, BRUSSELS, ROAD);
    addLink(g, STRASBOURG, PARIS, ROAD);

    //COLOGNE
    addLink(g, COLOGNE, STRASBOURG, ROAD);
    addLink(g, COLOGNE, BRUSSELS, ROAD);
    addLink(g, COLOGNE, FRANKFURT, ROAD);
    addLink(g, COLOGNE, HAMBURG, ROAD);
    addLink(g, COLOGNE, AMSTERDAM, ROAD);
    addLink(g, COLOGNE, LEIPZIG, ROAD);

    //HAMBURG
    addLink(g, HAMBURG, COLOGNE, ROAD);
    addLink(g, HAMBURG, BERLIN, ROAD);
    addLink(g, HAMBURG, LEIPZIG, ROAD);

    //AMSTERDAM
    addLink(g, AMSTERDAM, COLOGNE, ROAD);
    addLink(g, AMSTERDAM, BRUSSELS, ROAD);

    //BRUSSELS
    addLink(g, BRUSSELS, AMSTERDAM, ROAD);
    addLink(g, BRUSSELS, COLOGNE, ROAD);
    addLink(g, BRUSSELS, STRASBOURG, ROAD);
    addLink(g, BRUSSELS, PARIS, ROAD);
    addLink(g, BRUSSELS, LE_HAVRE, ROAD);

    //GENEVA
    addLink(g, GENEVA, ZURICH, ROAD);
    addLink(g, GENEVA, STRASBOURG, ROAD);
    addLink(g, GENEVA, PARIS, ROAD);
    addLink(g, GENEVA, CLERMONT_FERRAND, ROAD);
    addLink(g, GENEVA, MARSEILLES, ROAD);

    //MARSEILLES
    addLink(g, MARSEILLES, GENEVA, ROAD);
    addLink(g, MARSEILLES, ZURICH, ROAD);
    addLink(g, MARSEILLES, MILAN, ROAD);
    addLink(g, MARSEILLES, GENOA, ROAD);
    addLink(g, MARSEILLES, CLERMONT_FERRAND, ROAD);
    addLink(g, MARSEILLES, TOULOUSE, ROAD);

    //CLERMONT_FERRAND
    addLink(g, CLERMONT_FERRAND, MARSEILLES, ROAD);
    addLink(g, CLERMONT_FERRAND, GENEVA, ROAD);
    addLink(g, CLERMONT_FERRAND, TOULOUSE, ROAD);
    addLink(g, CLERMONT_FERRAND, PARIS, ROAD);
    addLink(g, CLERMONT_FERRAND, BORDEAUX, ROAD);
    addLink(g, CLERMONT_FERRAND, NANTES, ROAD);

    //PARIS
    addLink(g, PARIS, STRASBOURG, ROAD);
    addLink(g, PARIS, GENEVA, ROAD);
    addLink(g, PARIS, BRUSSELS, ROAD);
    addLink(g, PARIS, LE_HAVRE, ROAD);
    addLink(g, PARIS, NANTES, ROAD);
    addLink(g, PARIS, CLERMONT_FERRAND, ROAD);

    //LE_HAVRE
    addLink(g, LE_HAVRE, PARIS, ROAD);
    addLink(g, LE_HAVRE, BRUSSELS, ROAD);
    addLink(g, LE_HAVRE, NANTES, ROAD);

    //NANTES
    addLink(g, NANTES, LE_HAVRE, ROAD);
    addLink(g, NANTES, PARIS, ROAD);
    addLink(g, NANTES, BORDEAUX, ROAD);
    addLink(g, NANTES, CLERMONT_FERRAND, ROAD);

    //BORDEAUX
    addLink(g, BORDEAUX, NANTES, ROAD);
    addLink(g, BORDEAUX, CLERMONT_FERRAND, ROAD);
    addLink(g, BORDEAUX, TOULOUSE, ROAD);
    addLink(g, BORDEAUX, SARAGOSSA, ROAD);

    //TOULOUSE
    addLink(g, TOULOUSE, BORDEAUX, ROAD);
    addLink(g, TOULOUSE, CLERMONT_FERRAND, ROAD);
    addLink(g, TOULOUSE, MARSEILLES, ROAD);
    addLink(g, TOULOUSE, BARCELONA, ROAD);
    addLink(g, TOULOUSE, SARAGOSSA, ROAD);

    //SARAGOSSA
    addLink(g, SARAGOSSA, TOULOUSE, ROAD);
    addLink(g, SARAGOSSA, BARCELONA, ROAD);
    addLink(g, SARAGOSSA, BORDEAUX, ROAD);
    addLink(g, SARAGOSSA, ALICANTE, ROAD);
    addLink(g, SARAGOSSA, MADRID, ROAD);
    addLink(g, SARAGOSSA, SANTANDER, ROAD);

    //BARCELONA
    addLink(g, BARCELONA, SARAGOSSA, ROAD);
    addLink(g, BARCELONA, TOULOUSE, ROAD);

    //ALICANTE
    addLink(g, ALICANTE, SARAGOSSA, ROAD);
    addLink(g, ALICANTE, GRANADA, ROAD);
    addLink(g, ALICANTE, MADRID, ROAD);

    //GRANADA
    addLink(g, GRANADA, ALICANTE, ROAD);
    addLink(g, GRANADA, MADRID, ROAD);
    addLink(g, GRANADA, CADIZ, ROAD);

    //CADIZ
    addLink(g, CADIZ, GRANADA, ROAD);
    addLink(g, CADIZ, MADRID, ROAD);
    addLink(g, CADIZ, LISBON, ROAD);

    //LISBON
    addLink(g, LISBON, CADIZ, ROAD);
    addLink(g, LISBON, MADRID, ROAD);
    addLink(g, LISBON, SANTANDER, ROAD);

    //MADRID
    addLink(g, MADRID, LISBON, ROAD);
    addLink(g, MADRID, CADIZ, ROAD);
    addLink(g, MADRID, GRANADA, ROAD);
    addLink(g, MADRID, ALICANTE, ROAD);
    addLink(g, MADRID, SANTANDER, ROAD);
    addLink(g, MADRID, SARAGOSSA, ROAD);

    //SANTANDER
    addLink(g, SANTANDER, MADRID, ROAD);
    addLink(g, SANTANDER, LISBON, ROAD);
    addLink(g, SANTANDER, SARAGOSSA, ROAD);

    //LONDON
    addLink(g, LONDON, PLYMOUTH, ROAD);
    addLink(g, LONDON, SWANSEA, ROAD);
    addLink(g, LONDON, MANCHESTER, ROAD);

    //PLYMOUTH
    addLink(g, PLYMOUTH, LONDON, ROAD);

    //SWANSEA
    addLink(g, SWANSEA, LONDON, ROAD);
    addLink(g, SWANSEA, LIVERPOOL, ROAD);

    //MANCHESTER
    addLink(g, MANCHESTER, LIVERPOOL, ROAD);
    addLink(g, MANCHESTER, EDINBURGH, ROAD);
    addLink(g, MANCHESTER, LONDON, ROAD);

    //LIVERPOOL
    addLink(g, LIVERPOOL, SWANSEA, ROAD);
    addLink(g, LIVERPOOL, EDINBURGH, ROAD);

    //DUBLIN
    addLink(g, DUBLIN, GALWAY, ROAD);

    //GALWAY
    addLink(g, GALWAY, DUBLIN, ROAD);

    //EDINBURGH
    addLink(g, EDINBURGH, MANCHESTER, ROAD);

    //### RAIL Connections ###

    //ALICANTE
    addLink(g, ALICANTE, MADRID, RAIL);
    addLink(g, ALICANTE, BARCELONA, RAIL);

    //MADRID
    addLink(g, MADRID, ALICANTE, RAIL);
    addLink(g, MADRID, LISBON, RAIL);
    addLink(g, MADRID, SANTANDER, RAIL);
    addLink(g, MADRID, SARAGOSSA, RAIL);

    //BARCELONA
    addLink(g, BARCELONA, ALICANTE, RAIL);
    addLink(g, BARCELONA, SARAGOSSA, RAIL);

    //EDINBURGH
    addLink(g, EDINBURGH, MANCHESTER, RAIL);

    //GENEVA
    addLink(g, GENEVA, MILAN, RAIL);

    //LE_HAVRE
    addLink(g, LE_HAVRE, PARIS, RAIL);

    //LISBON
    addLink(g, LISBON, MADRID, RAIL);

    //MARSEILLES
    addLink(g, BARCELONA, ALICANTE, RAIL);

    //SANTANDER
    addLink(g, SANTANDER, MADRID, RAIL);

    //SARAGOSSA
    addLink(g, SARAGOSSA, MADRID, RAIL);
    addLink(g, SARAGOSSA, BARCELONA, RAIL);
    addLink(g, SARAGOSSA, BORDEAUX, RAIL);

    //BORDEAUX
    addLink(g, BORDEAUX, SARAGOSSA, RAIL);
    addLink(g, BORDEAUX, PARIS, RAIL);

    //PARIS
    addLink(g, PARIS, BORDEAUX, RAIL);
    addLink(g, PARIS, MARSEILLES, RAIL);
    addLink(g, PARIS, LE_HAVRE, RAIL);
    addLink(g, PARIS, BRUSSELS, RAIL);


    //BRUSSELS
    addLink(g, BRUSSELS, COLOGNE, RAIL);
    addLink(g, BRUSSELS, PARIS, RAIL);

    //COLOGNE
    addLink(g, COLOGNE, BRUSSELS, RAIL);
    addLink(g, COLOGNE, FRANKFURT, RAIL);

    //FRANKFURT
    addLink(g, FRANKFURT, STRASBOURG, RAIL);
    addLink(g, FRANKFURT, LEIPZIG, RAIL);
    addLink(g, FRANKFURT, COLOGNE, RAIL);

    //STRASBOURG
    addLink(g, STRASBOURG, FRANKFURT, RAIL);
    addLink(g, STRASBOURG, ZURICH, RAIL);

    //ZURICH
    addLink(g, ZURICH, MILAN, RAIL);
    addLink(g, ZURICH, STRASBOURG, RAIL);

    //MILAN
    addLink(g, MILAN, GENEVA, RAIL);
    addLink(g, MILAN, FLORENCE, RAIL);
    addLink(g, MILAN, ZURICH, RAIL);
    addLink(g, MILAN, GENOA, RAIL);

    //FLORENCE
    addLink(g, FLORENCE, ROME, RAIL);
    addLink(g, FLORENCE, MILAN, RAIL);

    //ROME
    addLink(g, ROME, FLORENCE, RAIL);
    addLink(g, ROME, NAPLES, RAIL);

    //NAPLES
    addLink(g, NAPLES, ROME, RAIL);
    addLink(g, NAPLES, BARI, RAIL);

    //BARI
    addLink(g, BARI, NAPLES, RAIL);

    //LEIPZIG
    addLink(g, LEIPZIG, FRANKFURT, RAIL);
    addLink(g, LEIPZIG, NUREMBURG, RAIL);
    addLink(g, LEIPZIG, BERLIN, RAIL);

    //BERLIN
    addLink(g, BERLIN, HAMBURG, RAIL);
    addLink(g, BERLIN, PRAGUE, RAIL);
    addLink(g, BERLIN, LEIPZIG, RAIL);

    //HAMBURG
    addLink(g, HAMBURG, BERLIN, RAIL);

    //NUREMBURG
    addLink(g, NUREMBURG, MUNICH, RAIL);
    addLink(g, NUREMBURG, LEIPZIG, RAIL);

    //PRAGUE
    addLink(g, PRAGUE, BERLIN, RAIL);
    addLink(g, PRAGUE, VIENNA, RAIL);

    //VIENNA
    addLink(g, VIENNA, PRAGUE, RAIL);
    addLink(g, VIENNA, VENICE, RAIL);
    addLink(g, VIENNA, BUDAPEST, RAIL);

    //VENICE
    addLink(g, VENICE, VIENNA, RAIL);

    //BUDAPEST
    addLink(g, BUDAPEST, VIENNA, RAIL);
    addLink(g, BUDAPEST, SZEGED, RAIL);

    //SZEGED
    addLink(g, SZEGED, BUDAPEST, RAIL);
    addLink(g, SZEGED, BELGRADE, RAIL);
    addLink(g, SZEGED, BUCHAREST, RAIL);

    //BELGRADE
    addLink(g, BELGRADE, SZEGED, RAIL);
    addLink(g, BELGRADE, SOFIA, RAIL);


    //SOFIA
    addLink(g, SOFIA, BELGRADE, RAIL);
    addLink(g, SOFIA, SALONICA, RAIL);
    addLink(g, SOFIA, VARNA, RAIL);

    //SALONICA
    addLink(g, SALONICA, SOFIA, RAIL);

    //VARNA
    addLink(g, VARNA, SOFIA, RAIL);

    //BUCHAREST
    addLink(g, BUCHAREST, SZEGED, RAIL);
    addLink(g, BUCHAREST, GALATZ, RAIL);
    addLink(g, BUCHAREST, CONSTANTA, RAIL);

    //GALATZ
    addLink(g, GALATZ, BUCHAREST, RAIL);

    //CONSTANTA
    addLink(g, CONSTANTA, BUCHAREST, RAIL);

    //LONDON
    addLink(g, LONDON, SWANSEA, RAIL);
    addLink(g, LONDON, MANCHESTER, RAIL);

    //SWANSEA
    addLink(g, SWANSEA, LONDON, RAIL);

    //MANCHESTER
    addLink(g, MANCHESTER, LONDON, RAIL);
    addLink(g, MANCHESTER, LIVERPOOL, RAIL);
    addLink(g, MANCHESTER, EDINBURGH, RAIL);

    //LIVERPOOL
    addLink(g, LIVERPOOL, MANCHESTER, RAIL);
    
    //### SEA Connections ###

    //BLACK SEA
    addLink(g, BLACK_SEA, IONIAN_SEA, SEA);
    addLink(g, BLACK_SEA, VARNA, SEA);
    addLink(g, BLACK_SEA, CONSTANTA, SEA);

    //IONIAN SEA
    addLink(g, IONIAN_SEA, BLACK_SEA, SEA);
    addLink(g, IONIAN_SEA, VALONA, SEA);
    addLink(g, IONIAN_SEA, ATHENS, SEA);
    addLink(g, IONIAN_SEA, SALONICA, SEA);
    addLink(g, IONIAN_SEA, ADRIATIC_SEA, SEA);
    addLink(g, IONIAN_SEA, TYRRHENIAN_SEA, SEA);

    //ADRIATIC SEA
    addLink(g, ADRIATIC_SEA, VENICE, SEA);
    addLink(g, ADRIATIC_SEA, BARI, SEA);
    addLink(g, ADRIATIC_SEA, IONIAN_SEA, SEA);

    //TYRRHENIAN SEA
    addLink(g, TYRRHENIAN_SEA, MEDITERRANEAN_SEA, SEA);
    addLink(g, TYRRHENIAN_SEA, CAGLIARI, SEA);
    addLink(g, TYRRHENIAN_SEA, GENOA, SEA);
    addLink(g, TYRRHENIAN_SEA, ROME, SEA);
    addLink(g, TYRRHENIAN_SEA, NAPLES, SEA);
    addLink(g, TYRRHENIAN_SEA, IONIAN_SEA, SEA);

    //MEDITERRANEAN_SEA
    addLink(g, MEDITERRANEAN_SEA, ATLANTIC_OCEAN, SEA);
    addLink(g, MEDITERRANEAN_SEA, TYRRHENIAN_SEA, SEA);
    addLink(g, MEDITERRANEAN_SEA, CAGLIARI, SEA);
    addLink(g, MEDITERRANEAN_SEA, MARSEILLES, SEA);
    addLink(g, MEDITERRANEAN_SEA, BARCELONA, SEA);
    addLink(g, MEDITERRANEAN_SEA, ALICANTE, SEA);

    //BAY_OF_BISCAY
    addLink(g, BAY_OF_BISCAY, ATLANTIC_OCEAN, SEA);
    addLink(g, BAY_OF_BISCAY, BORDEAUX, SEA);
    addLink(g, BAY_OF_BISCAY, SANTANDER, SEA);
    addLink(g, BAY_OF_BISCAY, NANTES, SEA);

    //ATLANTIC_OCEAN
    addLink(g, ATLANTIC_OCEAN, NORTH_SEA, SEA);
    addLink(g, ATLANTIC_OCEAN, BAY_OF_BISCAY, SEA);
    addLink(g, ATLANTIC_OCEAN, IRISH_SEA, SEA);
    addLink(g, ATLANTIC_OCEAN, ENGLISH_CHANNEL, SEA);
    addLink(g, ATLANTIC_OCEAN, MEDITERRANEAN_SEA, SEA);
    addLink(g, ATLANTIC_OCEAN, CADIZ, SEA);
    addLink(g, ATLANTIC_OCEAN, LISBON, SEA);
    addLink(g, ATLANTIC_OCEAN, GALWAY, SEA);

    //ENGLISH_CHANNEL
    addLink(g, ENGLISH_CHANNEL, ATLANTIC_OCEAN, SEA);
    addLink(g, ENGLISH_CHANNEL, NORTH_SEA, SEA);
    addLink(g, ENGLISH_CHANNEL, LE_HAVRE, SEA);
    addLink(g, ENGLISH_CHANNEL, PLYMOUTH, SEA);
    addLink(g, ENGLISH_CHANNEL, LONDON, SEA);

    //IRISH_SEA
    addLink(g, IRISH_SEA, ATLANTIC_OCEAN, SEA);
    addLink(g, IRISH_SEA, LIVERPOOL, SEA);
    addLink(g, IRISH_SEA, SWANSEA, SEA);
    addLink(g, IRISH_SEA, DUBLIN, SEA);

    //NORTH_SEA
    addLink(g, NORTH_SEA, ATLANTIC_OCEAN, SEA);
    addLink(g, NORTH_SEA, ENGLISH_CHANNEL, SEA);
    addLink(g, NORTH_SEA, EDINBURGH, SEA);
    addLink(g, NORTH_SEA, AMSTERDAM, SEA);
    addLink(g, NORTH_SEA, HAMBURG, SEA);

    //### SEA->CITIES Connections ###

    addLink(g, CONSTANTA, BLACK_SEA, SEA);
    addLink(g, VARNA, BLACK_SEA, SEA);
    addLink(g, SALONICA, IONIAN_SEA, SEA);
    addLink(g, ATHENS, IONIAN_SEA, SEA);
    addLink(g, VALONA, IONIAN_SEA, SEA);
    addLink(g, BARI, ADRIATIC_SEA, SEA);
    addLink(g, VENICE, ADRIATIC_SEA, SEA);
    addLink(g, NAPLES, TYRRHENIAN_SEA, SEA);
    addLink(g, ROME, TYRRHENIAN_SEA, SEA);
    addLink(g, GENOA, TYRRHENIAN_SEA, SEA);
    addLink(g, CAGLIARI, TYRRHENIAN_SEA, SEA);
    addLink(g, CAGLIARI, MEDITERRANEAN_SEA, SEA);
    addLink(g, MARSEILLES, MEDITERRANEAN_SEA, SEA);
    addLink(g, BARCELONA, MEDITERRANEAN_SEA, SEA);
    addLink(g, ALICANTE, MEDITERRANEAN_SEA, SEA);
    addLink(g, CADIZ, ATLANTIC_OCEAN, SEA);
    addLink(g, LISBON, ATLANTIC_OCEAN, SEA);
    addLink(g, GALWAY, ATLANTIC_OCEAN, SEA);
    addLink(g, SANTANDER, BAY_OF_BISCAY, SEA);
    addLink(g, BORDEAUX, BAY_OF_BISCAY, SEA);
    addLink(g, NANTES, BAY_OF_BISCAY, SEA);
    addLink(g, LONDON, ENGLISH_CHANNEL, SEA);
    addLink(g, PLYMOUTH, ENGLISH_CHANNEL, SEA);
    addLink(g, LE_HAVRE, ENGLISH_CHANNEL, SEA);
    addLink(g, SWANSEA, IRISH_SEA, SEA);
    addLink(g, LIVERPOOL, IRISH_SEA, SEA);
    addLink(g, DUBLIN, IRISH_SEA, SEA);
    addLink(g, EDINBURGH, NORTH_SEA, SEA);
    addLink(g, AMSTERDAM, NORTH_SEA, SEA);
    addLink(g, HAMBURG, NORTH_SEA, SEA);
}

//Useful for debugging
void showGraph(Graph g) { 
    assert(g != NULL); 
    printf("V=%d, E=%d + %d + %d\n", g->nV, g->nE[ROAD],g->nE[RAIL],g->nE[SEA]); 
    int i; 
    for (i = 0; i < g->nV; i++) {
        VList n = g->connections[i]; 
        while (n != NULL) { 
            printf("%d-%d ",i,n->v); 
            if(n->type == ROAD) {
                printf("L ");
            } else if (n->type == RAIL) {
                printf ("R ");
            } else if(n->type == SEA) {
                printf("S ");
            } else {
                printf("ERROR NO SUCH TYPE");
                exit(0);
            }
            n = n->next; 
        }
        if (g->connections [i] != NULL)
            printf("\n");
    }
}

int numV(Graph g) {
    assert(g != NULL);
    return g->nV;
}

int numE(Graph g, Transport type) {
    assert(g != NULL);
    assert(type >= 0 && type <= ANY);
    if(type == ANY) {
        return g->nE[ROAD] + g->nE[RAIL] + g->nE[SEA];
    } else {
        return g->nE[type];
    }
}

//returns 1 if there is an edge from start to end of the given type
//gives 0 otherwise
int isAdjacent(Graph g,Location start, Location end, Transport type) {
    VList current = g->connections[start]; //list->first
    while (current != NULL) {
        if (current->v == end) {
            if ((current->type == type) || (type == ANY)) {
                return 1;
            }
        }
       current = current->next;
    }
    return 0;
}


//Determines which locations can be reached from the start location
//in n hops or less with a given transport type
//If a location can be reached in n hops or less locs[location] is set to 1
//otherwise it is left as 0
void canReachInN(Graph g, Location start, Transport type, int n, int locs[]) {
    locs[start] = 1;
    if (n > 0) {
        VList current = g->connections[start];

        while (current != NULL) { //CHECK
            if ((current->type == type) || (type == ANY)) {
                locs[current->v] = 1;
                //printf ("next: %d, nextT %d\n",current->next->v, current->next->type);
            }
            current = current->next;
        }

        int i;
        for (i=0; i<NUM_MAP_LOCATIONS; i++) { //check all adj vertices until n=0
            if (isAdjacent(g, start, i, type) == 1) {
                canReachInN (g, i, type, n-1, locs);
            }
        }
    }
}