#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <math.h>
#include "Pcolors.h"

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

/* Fonctions principales :
 *   - Initialisation et libération :
 *       * listeInit(nb_vrtx) : Crée une liste d'adjacence pour nb_vrtx sommets.
 *       * listeFree(li) : Libère la mémoire occupée par la liste d'adjacence.
 *   - Manipulation des arêtes :
 *       * listeAdd(li, va, vb) : Ajoute une arête entre les sommets va et vb.
 *       * listeAddSingle(li, vrtx, vzn_id) : Ajoute un voisin unique à un sommet.
 *       * listeRemove(li, va, vb) : efface l'arête entre les sommets va et vb.
 *       * listeRemoveSingle(li, vrtx, vzn_id) : enlève un voisin unique d'un sommet.
 *       * areteExiste(li, va, vb) : renvoie 1 si l'arête existe, 0 sinon.
 *   - Chargement et génération :
 *       * listeLoad(fname) : Charge un graphe à partir d'un fichier.
 *       * listeSave(li, fname) : Sauvegarde un graphe dans un fichier (seulement les arrètes)
 *       * tabEdges2Liste(nb_vrtx, tab, tab_size) : Convertit un tableau d'arêtes en liste d'adjacence.
 *       * erdosRenyi(n, m) : Génère un graphe aléatoire selon le modèle d'Erdős-Rényi.
 *       * erdosRenyi2(n, m) : Génère un graphe aléatoire (non connexe) selon le modèle d'Erdős-Rényi.
 *   - Analyse :
 *       * listeDegDistrib(li) : Calcule et renvoie la distribution des degrés.
 *       * listeSaveDegDistrib(li) : Calcule et enregistre la distribution des degrés dans nom_plot.
 *       * listePrintStats(li) : Affiche le n et m du graphe.
 *       * listePrint(li) : Affiche la liste d'adjacence.
 *       * listeRender(li, graph_name, do_open) : Utilise Graphviz pour générer une représentation de li : graph_name.png
 */

LiAdj* listeInit(uint32_t nb_vrtx);
void listeFree(LiAdj* li);
int  listeAddSingle(LiAdj* li, uint32_t vrtx, uint32_t vzn_id);
void listeAdd(LiAdj* li, uint32_t va, uint32_t vb);
void listeRemoveSingle(LiAdj* li, uint32_t va, uint32_t vb);
void listeRemove(LiAdj* li, uint32_t va, uint32_t vb);
uint8_t areteExiste(LiAdj* li, uint32_t va, uint32_t vb);
LiAdj* listeLoad(char* fname);
void listeSave(LiAdj* li, char* fname);
LiAdj* tabEdges2Liste(uint32_t nb_vrtx, uint32_t* tab, uint32_t tab_size);
LiAdj* erdosRenyi(uint32_t n, uint32_t m);
LiAdj* erdosRenyi2(uint32_t n, uint32_t m);
uint32_t* listeDegDistrib(LiAdj* li, uint32_t* tab_size);
void listeSaveDegDistrib(LiAdj* li, char* nom_plot);
void listePrintStats(LiAdj* li);
void listePrint(LiAdj* li);
void listeRender(LiAdj* li, char* graph_name, int do_open);

/* Fonctions utilitaires :
 *   - Gestion des voisins :
 *       * vrtxVoisinInit(id, next) : Initialise un voisin.
 *       * vrtxVoisinsFree(v) : Libère la mémoire des voisins.
 *       * vrtxDeg(vrtx) : Calcule le degré d'un sommet.
 *       * vrtxEstVoisin(vrtx, voiz) : renvoie 1 si voiz est un voisin de vrtx, 0 sinon
 *       * vrtxVoisinsPrint(v) : Affiche les voisins d'un sommet.
 *   - Lecture et traitement de fichiers :
 *       * readFile(fname, actionOnLine, ret) : Lit un fichier et applique une fonction aux lignes.
 *       * countVrtx(fname) : Compte le nombre de sommets uniques dans un fichier.
 *       * loadConfigModel(fname, size_config_model) : Charge un modèle de config en mémoire, et enregistre sa taille dans size_config_model.
 *       * _printline(va, vb, test) : Fonction interne pour afficher une arête.
 *       * _unique_vrtx_liste(va, vb, li) : Fonction interne pour ajouter les deux sommets d'une arrète à une liste.
 *       * _edges_liste(va, vb, li) : Fonction interne pour ajouter une arête à la liste d'adjacence.
 *       * _find_max(va, vb, max) : Fonction interne pour trouver le Max dans la première col d'un fichier.
 *       * _config_model_tab(va, vb, tab) : Fonction interne pour ajouter valeur vb à un tableau, en va.
 *       * startLog(fname) : ouvre et vide un fichier.
 *   - Manipulations et transformations de tableaux :
 *       * swapTab(tab, i, k) : Échange deux éléments d'un tableau.
 *       * shuffle(tab, size) : Mélange un tableau.
 *       * sortEdgetab(tab, tab_size) : Trie les semi-arêtes, renvoie NULL si echec.
 *       * iterSortEdgetab(tab, tab_size, max_iter, nb_tries) : Appelle sortEdgetab jusqu'a son succès, max_iter fois. renvoie NULL sinon.
 *       * printtab(tab, tab_size) : Affiche le contenu d'un tableau.
 *       * duptab(tab, tab_size) : Duplique et renvoie le contenu d'un tableau.
 *   - Analyse et génération de structures de graphes :
 *       * tabCountsOcc(degs, size_degs, size_occ) : Calcule l'occurrence des degrés.
 *       * tabEdgesConfig(occ, size_occ, edges_size, graph_size) : Génère un tableau de semi-arêtes à partir d'une distribution de degrés.
 *       * _edgeListeDoublon(a, b, tab, tab_start, tab_size) : Vérifie si une arête est déjà présente dans un tableau de semi-arêtes.
 *       * swap(li, a, b, c, d, do_test) : échange deux arrêtes (a, b) et (c, d) qui deviennent (a, c) et (b, d), do_test=1 teste les arêtes d'arrivée, do_test=2 teste aussi les arêtes en entrée.
 */

VrtxVoisin* vrtxVoisinInit(uint32_t id, VrtxVoisin* next);
void vrtxVoisinsFree(VrtxVoisin* v);
uint32_t vrtxDeg(VrtxVoisin* vrtx);
uint8_t vrtxEstVoisin(VrtxVoisin* vrtx, uint32_t voiz);
void vrtxVoisinsPrint(VrtxVoisin* v);

void readFile(char* fname, void (*actionOnLine)(uint32_t va, uint32_t vb, void* ret), void* ret, int do_remove_header);
uint32_t countVrtx(char* fname);
uint32_t* loadConfigModel(char* fname, uint32_t* size_config_model);
void _printline(uint32_t va, uint32_t vb, void* test);
void _unique_vrtx_liste(uint32_t va, uint32_t vb, void* li);
void _edges_liste(uint32_t va, uint32_t vb, void* li);
void _find_max(uint32_t va, uint32_t vb, void* max);
void _config_model_tab(uint32_t va, uint32_t vb, void* tab);
FILE* startLog(char* fname);

void swapTab(uint32_t* tab, uint32_t i, uint32_t k);
void shuffle(uint32_t* tab, uint32_t size);
uint32_t* sortEdgetab(uint32_t* tab, uint32_t tab_size);
uint32_t* iterSortEdgetab(uint32_t* tab, uint32_t tab_size, uint32_t max_iter, uint32_t* nb_tries);
void printtab(uint32_t* tab, uint32_t tab_size);
uint32_t* duptab(uint32_t* tab, uint32_t tab_size);

uint32_t* tabCountsOcc(uint32_t* degs, uint32_t size_degs, uint32_t* size_occ);
uint32_t* tabEdgesConfig(uint32_t* occ, uint32_t size_occ, uint32_t* edges_size, uint32_t* graph_size);
int _edgeListeDoublon(uint32_t a, uint32_t b, uint32_t* tab, uint32_t tab_start, uint32_t tab_size);
void swap(LiAdj* li, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint8_t do_test);

int bernou(double prob);

uint32_t compterTriangles(LiAdj* li);