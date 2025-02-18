#include "graphes.h"

// LISTES D'ADJACENCES

LiAdj* listeInit(int nb_vrtx) {
    LiAdj* liste = (LiAdj*)malloc(sizeof(LiAdj));
    liste->nb_vrtx = nb_vrtx;
    liste->nb_edges = 0;
    liste->L = (VrtxVoisin**)malloc(nb_vrtx * sizeof(VrtxVoisin));

    for (int i=0;i<nb_vrtx;i++) {
        liste->L[i] = NULL;
    }
    return liste;
}

void listeFree(LiAdj* li) {
    for (int i=0;i < li->nb_vrtx;i++) {
        vrtxVoisinsFree(li->L[i]);
    }
    free(li->L);
    free(li);
}

int listeSingleAdd(LiAdj* li, int vrtx, int vzn_id) {
    if (li->L[vrtx] == NULL) {
        li->L[vrtx] = vrtxVoisinInit(vzn_id);
        li->nb_edges += 1;
        return 1;
    }

    VrtxVoisin* voisin = li->L[vrtx];

    if (voisin->id == vzn_id) return 0;
    while (voisin->next != NULL) {
        if (voisin->next->id == vzn_id) return 0;
        voisin = voisin->next;
    }

    VrtxVoisin* new = vrtxVoisinInit(vzn_id);
    voisin->next = new;
    li->nb_edges += 1;
    return 1;
}

void listeAdd(LiAdj* li, int va, int vb) {
    listeSingleAdd(li, va, vb);
    listeSingleAdd(li, vb, va);
}

LiAdj* tabEdges2Liste(int nb_vrtx, int* tab, int tab_size) {
    LiAdj* li = listeInit(nb_vrtx);
    for (int i=0; i<tab_size/2; i++) {
        printf("adding : %d et %d\n", tab[2*i], tab[2*i+1]);
        listeAdd(li, tab[2*i], tab[2*i+1]);
    }

    return li;
}

void listePrint(LiAdj* li) {
    printf("liste d'ajdacence : \n");
    for (int i=0; i<li->nb_vrtx; i++) {
        printf("%d :\t", i +1);
        vrtxVoisinsPrint(li->L[i]);
    }
    printf("\n");
}

// VOISINS : LISTE CHAINÉE

VrtxVoisin* vrtxVoisinInit(int id) {
    VrtxVoisin* new = (VrtxVoisin*)malloc(sizeof(VrtxVoisin));
    new->id = id;
    new->next = NULL;
    return new;
}

void vrtxVoisinsFree(VrtxVoisin* vrtx) {
    VrtxVoisin* tmp;
    while (vrtx != NULL) {
        tmp = vrtx;
        vrtx = vrtx->next;
        free(tmp);
    }
}

int vrtxDeg(VrtxVoisin* vrtx) {
    int i = 0;
    while (vrtx != NULL) {
        vrtx = vrtx->next;
        i++;
    }
    return i;
}

void vrtxVoisinsPrint(VrtxVoisin* vrtx) {
    while (vrtx != NULL) {
        printf("%d ", vrtx->id +1);
        vrtx = vrtx->next;
    }
    printf("\n");
}

// GRAPHES

Graphe* grInit() {
    Graphe *Gr = (Graphe*)malloc(sizeof(Graphe));

    Gr->liste = NULL;
    Gr->mat = NULL;
    return Gr;
}

void grFree(Graphe* Gr) {
    if (Gr->liste != NULL) listeFree(Gr->liste);
    // if (Gr->mat != NULL) matFree(Gr->mat)   MATRICE D'ADJACENCE PAS ENCORE IMPLÉMENTÉE;
    free(Gr);
}

Graphe* grLoad(char* fname, int loadMat, int loadListe) {
    Graphe *Gr = grInit();
    if (loadMat) {}
    if (loadListe) {
        LiAdj* li = listeInit( countVrtx(fname) );
        readFile(fname, &_edges_liste, li);
        Gr->liste = li;
    }
    return Gr;
}

void grDegDistrib(Graphe* Gr) {
    char *nom_plot = "test.txt";
    fclose(fopen(nom_plot, "w"));
    FILE *plot = fopen(nom_plot, "a");

    if (Gr->liste != NULL) {
        int *degs = (int*)malloc(Gr->liste->nb_vrtx * sizeof(int));
        int sizeOcc = 0;
        for (int i=0; i < Gr->liste->nb_vrtx; i++) {
            degs[i] = vrtxDeg(Gr->liste->L[i]);
        }

        int *occ = tabCountsOcc(degs, Gr->liste->nb_vrtx, &sizeOcc);
        free(degs);
        printtab(occ, sizeOcc);

        for (int i=0; i<sizeOcc; i++) {
            if (occ[i] == 0 && i!=0 && i!=sizeOcc-1) continue;
            fprintf(plot, "%d %d\n", i, occ[i]);
        }
        free(occ);
    } //else    MATRICE D'ADJACENCE PAS ENCORE IMPLÉMENTÉE
    fclose(plot);
}

Graphe* erdosRenyi(int n, int m) {
    Graphe* Gr = grInit();
    LiAdj* li = listeInit(n);
    Gr->liste = li;

    srand(time(NULL));
    double prob = 2*m;
    prob /= n*(n+1); //     m ÷ (2 parmi n)
    printf("prob : %f\n", prob);

    for (int va=0; va<n; va++) {
        for (int vb=va +1; vb<n; vb++) {  // on liste toutes les paires de vrtx possibles, sans doublons, sans boucles : n(n-1)/2
            if (bernou(prob) == 1) listeAdd(li, va, vb);
        }
    }
    return Gr;
}

void grPrintStats(Graphe* Gr) {
    if (Gr->liste != NULL) {
        printf("GRAPHE :\n");
        printf("\tn = %d\n", Gr->liste->nb_vrtx);
        printf("\tm = %d\n", Gr->liste->nb_edges/2);
    } // else {} MATRICE PAS IMPLÉMENTÉE
}

// OUTILS

void readFile(char* fname, void (*actionOnLine)(int va, int vb, void* ret), void* ret) {
    FILE* f = fopen(fname, "r");
    char header[256];  // pas de données sur la première ligne
    fgets(header, 256, f);

    int va = -1, vb = -1;
    fscanf(f, "%d\t%d ", &va, &vb);
    while(va != -1) {
        actionOnLine(va-1, vb-1, ret);
        va = -1; vb = -1;
        fscanf(f, "%d\t%d ", &va, &vb);
    }
}

int countVrtx(char* fname) {
    LiAdj* li = listeInit(1); // on utilise une liste d'adjacence de taille 1:N pour compter les voisins
    readFile(fname, &_unique_vrtx_liste, li);
    int nb = li->nb_edges;
    listeFree(li);
    return nb;
}

void _printline(int va, int vb, void* test) {
    printf("%d %d\n", va, vb);
}

void _unique_vrtx_liste(int va, int vb, void* li) {
    listeSingleAdd((LiAdj*)li, 0, va);
    listeSingleAdd((LiAdj*)li, 0, vb);
}

void _edges_liste(int va, int vb, void* li) {
    listeAdd(li, va, vb);
}

int bernou(double prob) {
    double randFl = rand();
    randFl /= RAND_MAX;
    if (randFl <= prob) return 1;
    else return 0;
}

int* tabCountsOcc(int* degs, int size_degs, int* size_occ) {
    int min = degs[0], max = degs[0], i;
    for (i=1; i<size_degs; i++) {
        if (degs[i] < min) min = degs[i];
        if (degs[i] > max) max = degs[i];
    }

    int *occ = (int*)malloc((max+1) * sizeof(int));
    for (i=0; i<max+1; i++) {
        occ[i] = 0;
    }
    for (i=0; i<size_degs; i++) {
        occ[degs[i]] += 1;
    }

    *size_occ = max+1;
    return occ;
}

void shuffle(int* tab, int size) {
    srand(time(NULL));
    int* shuffled = (int*)malloc(size * sizeof(int));
    for (int i=0;i<size;i++) shuffled[i] = -1;
    int k = rand() % size;
    for (int i=0;i<size;i++) {
        while (shuffled[k] != -1) k = rand() % size;
        shuffled[k] = tab[i];
    }
    for (int i=0;i<size;i++) tab[i] = shuffled[i];
    free(shuffled);
}

int* tabEdgesConfig(int* occ, int size_occ, int* edges_size) {
    int nb_edges = 0;
    for (int i=0; i<size_occ; i++) {
        nb_edges += occ[i] * i;
    }
    int* edges = (int*)malloc(nb_edges * sizeof(int));

    int vrtx_id = 0, edge_id = 0;
    for (int i=0; i<size_occ; i++) {
        for (int k=0; k<occ[i]; k++) {
            for (int j=0; j<i; j++) {
                edges[edge_id] = vrtx_id;
                edge_id++;
            }
            vrtx_id++;
        }
    }
    shuffle(edges, nb_edges);
    *edges_size = nb_edges;
    return edges;
}

void swap(int* tab, int i, int k) {
    int tmp = tab[i];
    tab[i] = tab[k];
    tab[k] = tmp;
}

void printtab(int* tab, int tab_size) {
    printf("[");
    for (int i=0; i<tab_size-1; i++) {
        printf("%d, ", tab[i]);
    }
    printf("%d]\n", tab[tab_size-1]);
}

int _edgeListeDoublon(int a, int b, int* tab, int tab_start, int tab_size) {
    if (tab_size == 0) return 0;
    tab += tab_start;
    for (int i=0; i<tab_size/2; i++) {
        if (tab[2*i] == a && tab[2*i+1] == b) {
            return 1;
        }
        if (tab[2*i] == b && tab[2*i+1] == a) {
            return 1;
        }
    }
    return 0;
}

int* sortEdgetab(int* tab, int tab_size) {
    int j;
    for (int i=0; i<tab_size/2; i++) {
        j = 1;
        while ((tab[0] == tab[j] ||
                _edgeListeDoublon(tab[0], tab[j], tab, tab_size-2*i, 2*i))
               && j < tab_size-2*i) j++;
        swap(tab, 0, tab_size -2*(i+1));
        swap(tab, j, tab_size -2*i- 1);
        swap(tab, 1, j);
    }

    return tab;
}