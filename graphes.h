#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <stdint.h>
#include <inttypes.h>

typedef struct _vrtx_voisin VrtxVoisin;
struct _vrtx_voisin {
    uint32_t id;
    VrtxVoisin* next;
};

typedef struct _liste_adjacence LiAdj;
struct _liste_adjacence {
    uint32_t nb_vrtx;
    uint32_t nb_edges;   // tous les vrtx comptent pour 2
    VrtxVoisin** L;
};

typedef struct _graphe Graphe;
struct _graphe {
    LiAdj* liste;
};

LiAdj* listeInit(uint32_t nb_vrtx);
void listeFree(LiAdj* li);
int listeSingleAdd(LiAdj* li, uint32_t vrtx, uint32_t vzn_id);
void listeAdd(LiAdj* li, uint32_t va, uint32_t vb);
LiAdj* tabEdges2Liste(uint32_t nb_vrtx, uint32_t* tab, uint32_t tab_size);
void listePrint(LiAdj* li);

VrtxVoisin* vrtxVoisinInit(uint32_t id);
void vrtxVoisinsFree(VrtxVoisin* v);
uint32_t vrtxDeg(VrtxVoisin* vrtx);
void vrtxVoisinsPrint(VrtxVoisin* v);

Graphe* grInit();
void grFree(Graphe* Gr);
Graphe* grLoad(char* fname);
void grDegDistrib(Graphe* Gr);
Graphe* erdosRenyi(uint32_t n, uint32_t m);
void grPrintStats(Graphe* Gr);

void readFile(char* fname, void (*actionOnLine)(uint32_t va, uint32_t vb, void* ret), void* ret);
uint32_t countVrtx(char* fname);
void _printline(uint32_t va, uint32_t vb, void* test);
void _unique_vrtx_liste(uint32_t va, uint32_t vb, void* li);
void _edges_liste(uint32_t va, uint32_t vb, void* li);
int bernou(double prob);
uint32_t* tabCountsOcc(uint32_t* degs, uint32_t size_degs, uint32_t* size_occ);
void shuffle(uint32_t* tab, uint32_t size);
uint32_t* tabEdgesConfig(uint32_t* occ, uint32_t size_occ, uint32_t* edges_size);
void swap(uint32_t* tab, uint32_t i, uint32_t k);
void printtab(uint32_t* tab, uint32_t tab_size);
int _edgeListeDoublon(uint32_t a, uint32_t b, uint32_t* tab, uint32_t tab_start, uint32_t tab_size);
uint32_t* sortEdgetab(uint32_t* tab, uint32_t tab_size);