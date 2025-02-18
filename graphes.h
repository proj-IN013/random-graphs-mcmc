#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

typedef struct _matrice_adjacence MatAdj;
struct _matrice_adjacence {
    // PAS ENCORE IMPLÉMENTÉ
};

typedef struct _vrtx_voisin VrtxVoisin;
struct _vrtx_voisin {
    int id;
    VrtxVoisin* next;
};

typedef struct _liste_adjacence LiAdj;
struct _liste_adjacence {
    int nb_vrtx;
    int nb_edges;   // tous les vrtx comptent pour 2
    VrtxVoisin** L;
};

typedef struct _graphe Graphe;
struct _graphe {
    MatAdj* mat;
    LiAdj* liste;
};

LiAdj* listeInit(int nb_vrtx);
void listeFree(LiAdj* li);
int listeSingleAdd(LiAdj* li, int vrtx, int vzn_id);
void listeAdd(LiAdj* li, int va, int vb);
LiAdj* tabEdges2Liste(int nb_vrtx, int* tab, int tab_size);
void listePrint(LiAdj* li);

VrtxVoisin* vrtxVoisinInit(int id);
void vrtxVoisinsFree(VrtxVoisin* v);
int vrtxDeg(VrtxVoisin* vrtx);
void vrtxVoisinsPrint(VrtxVoisin* v);

Graphe* grInit();
void grFree(Graphe* Gr);
Graphe* grLoad(char* fname, int loadMat, int loadListe);
void grDegDistrib(Graphe* Gr);
Graphe* erdosRenyi(int n, int m);
void grPrintStats(Graphe* Gr);

void readFile(char* fname, void (*actionOnLine)(int va, int vb, void* ret), void* ret);
int countVrtx(char* fname);
void _printline(int va, int vb, void* test);
void _unique_vrtx_liste(int va, int vb, void* li);
void _edges_liste(int va, int vb, void* li);
int bernou(double prob);
int* tabCountsOcc(int* degs, int size_degs, int* size_occ);
void shuffle(int* tab, int size);
int* tabEdgesConfig(int* occ, int size_occ, int* edges_size);
void swap(int* tab, int i, int k);
void printtab(int* tab, int tab_size);
int _edgeListeDoublon(int a, int b, int* tab, int tab_start, int tab_size);
int* sortEdgetab(int* tab, int tab_size);