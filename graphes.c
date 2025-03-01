#include "graphes.h"

// LISTES D'ADJACENCES

LiAdj* listeInit(uint32_t nb_vrtx) {
    assert(nb_vrtx > 0);
    LiAdj* liste = (LiAdj*)malloc(sizeof(LiAdj));
    assert(liste);

    liste->nb_vrtx = nb_vrtx;
    liste->nb_edges = 0;
    liste->L = (VrtxVoisin**)malloc(nb_vrtx * sizeof(VrtxVoisin));
    assert(liste->L);

    for (uint32_t i=0;i<nb_vrtx;i++) {
        liste->L[i] = NULL;
    }
    return liste;
}

void listeFree(LiAdj* li) {
    if (li == NULL) return;

    for (uint32_t i=0;i < li->nb_vrtx;i++) {
        vrtxVoisinsFree(li->L[i]);
    }
    free(li->L);
    free(li);
}

int listeSingleAdd(LiAdj* li, uint32_t vrtx, uint32_t vzn_id) {
    assert(li != NULL);
    assert(vrtx >= 0 && vzn_id >= 0);
    assert(vrtx < li->nb_vrtx);

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

void listeAdd(LiAdj* li, uint32_t va, uint32_t vb) {
    listeSingleAdd(li, va, vb);
    listeSingleAdd(li, vb, va);
}

LiAdj* listeLoad(char* fname) {
    LiAdj* li = listeInit( countVrtx(fname) );
    readFile(fname, &_edges_liste, li);
    return li;
}

LiAdj* tabEdges2Liste(uint32_t nb_vrtx, uint32_t* tab, uint32_t tab_size) {
    assert(tab != NULL && nb_vrtx > 0);

    LiAdj* li = listeInit(nb_vrtx);
    for (uint32_t i=0; i<tab_size/2; i++) {
        listeAdd(li, tab[2*i], tab[2*i+1]);
    }

    return li;
}

LiAdj* erdosRenyi(uint32_t n, uint32_t m) {
    LiAdj* li = listeInit(n);

    srand(time(NULL));
    double prob = 2*m;
    prob /= n*(n+1); //     m ÷ (2 parmi n)
    printf("prob : %f\n", prob);

    for (uint32_t va=0; va<n; va++) {
        for (uint32_t vb=va +1; vb<n; vb++) {  // on liste toutes les paires de vrtx possibles, sans doublons, sans boucles : n(n-1)/2
            if (bernou(prob) == 1) listeAdd(li, va, vb);
        }
    }
    return li;
}

void listeDegDistrib(LiAdj* li) {
    char *nom_plot = "test.txt";
    FILE *plot = fopen(nom_plot, "w");

    assert(li);
    assert(plot);
    fclose(plot);
    plot = fopen(nom_plot, "a");

    uint32_t *degs = (uint32_t*)malloc(li->nb_vrtx * sizeof(uint32_t));
    assert(degs);
    uint32_t size_occ = 0;
    for (uint32_t i=0; i < li->nb_vrtx; i++) {
        degs[i] = vrtxDeg(li->L[i]);
    }
    uint32_t *occ = tabCountsOcc(degs, li->nb_vrtx, &size_occ);
    free(degs);
    for (uint32_t i=0; i<size_occ; i++) {
        if (occ[i] == 0 && i!=0 && i!=size_occ-1) continue;
        fprintf(plot, "%d %d\n", i, occ[i]);
    }
    free(occ);

    fclose(plot);
}

void listePrintStats(LiAdj* li) {
    if (li == NULL) return;
    printf("GRAPHE :\n");
    printf("\tn = %" PRIu32 "\n", li->nb_vrtx);
    printf("\tm = %" PRIu32 "\n", li->nb_edges/2);
}

void listePrint(LiAdj* li) {
    if (li == NULL) return;

    printf("liste d'ajdacence : \n");
    for (uint32_t i=0; i<li->nb_vrtx; i++) {
        printf("%d :\t", i +1);
        vrtxVoisinsPrint(li->L[i]);
    }
    printf("\n");
}

// VOISINS : LISTE CHAINÉE

VrtxVoisin* vrtxVoisinInit(uint32_t id) {
    assert(id >= 0);

    VrtxVoisin* new_vrtx = (VrtxVoisin*)malloc(sizeof(VrtxVoisin));
    assert(new_vrtx);

    new_vrtx->id = id;
    new_vrtx->next = NULL;
    return new_vrtx;
}

void vrtxVoisinsFree(VrtxVoisin* vrtx) {
    VrtxVoisin* tmp;
    while (vrtx != NULL) {
        tmp = vrtx;
        vrtx = vrtx->next;
        free(tmp);
    }
}

uint32_t vrtxDeg(VrtxVoisin* vrtx) {
    assert(vrtx);

    uint32_t i = 0;
    while (vrtx != NULL) {
        vrtx = vrtx->next;
        i++;
    }
    return i;
}

void vrtxVoisinsPrint(VrtxVoisin* vrtx) {
    if (vrtx == NULL) return;

    while (vrtx != NULL) {
        printf("%d ", vrtx->id +1);
        vrtx = vrtx->next;
    }
    printf("\n");
}


void readFile(char* fname, void (*actionOnLine)(uint32_t va, uint32_t vb, void* ret), void* ret) {
    FILE* fic = fopen(fname, "r");
    assert(fic);

    char header[256];  // pas de données sur la première ligne
    fgets(header, 256, fic);

    uint32_t va = UINT32_MAX, vb = UINT32_MAX;
    fscanf(fic, "%" PRIu32 "\t%" PRIu32 " ", &va, &vb);
    while(va != UINT32_MAX) {
        actionOnLine(va-1, vb-1, ret);
        va = UINT32_MAX; vb = UINT32_MAX;
        fscanf(fic, "%" PRIu32 "\t%" PRIu32 " ", &va, &vb);
    }
}

uint32_t countVrtx(char* fname) {
    LiAdj* li = listeInit(1); // on utilise une liste d'adjacence de taille 1:N pour compter les voisins
    readFile(fname, &_unique_vrtx_liste, li);
    uint32_t nb = li->nb_edges;
    listeFree(li);
    return nb;
}

void _printline(uint32_t va, uint32_t vb, void* test) {
    printf("%d %d\n", va, vb);
}

void _unique_vrtx_liste(uint32_t va, uint32_t vb, void* li) {
    listeSingleAdd((LiAdj*)li, 0, va);
    listeSingleAdd((LiAdj*)li, 0, vb);
}

void _edges_liste(uint32_t va, uint32_t vb, void* li) {
    listeAdd(li, va, vb);
}


void swapTab(uint32_t* tab, uint32_t i, uint32_t k) {
    assert(tab);

    uint32_t tmp = tab[i];
    tab[i] = tab[k];
    tab[k] = tmp;
}

void shuffle(uint32_t* tab, uint32_t size) {
    srand(time(NULL));
    uint32_t* shuffled = (uint32_t*)malloc(size * sizeof(uint32_t));
    assert(shuffle);
    assert(tab);
    uint32_t i, k;

    for (i=0;i<size;i++) shuffled[i] = UINT32_MAX;
    k = rand() % size;
    for (i=0;i<size;i++) {
        while (shuffled[k] != UINT32_MAX) k = rand() % size;
        shuffled[k] = tab[i];
    }
    for (i=0;i<size;i++) tab[i] = shuffled[i];
    free(shuffled);
}

uint32_t* sortEdgetab(uint32_t* tab, uint32_t tab_size) {
    assert(tab != NULL && tab_size > 0);

    uint32_t i, j;
    for (i=0; i<tab_size/2; i++) {
        j = 1;
        while ((tab[0] == tab[j] ||
                _edgeListeDoublon(tab[0], tab[j], tab, tab_size-2*i, 2*i))
               && j < tab_size-2*i) j++;
        swapTab(tab, 0, tab_size -2*(i+1));
        swapTab(tab, j, tab_size -2*i- 1);
        swapTab(tab, 1, j);
    }

    return tab;
}

void printtab(uint32_t* tab, uint32_t tab_size) {
    if (tab == NULL) return;

    printf("[");
    for (uint32_t i=0; i<tab_size-1; i++) {
        printf("%d, ", tab[i]);
    }
    printf("%d]\n", tab[tab_size-1]);
}


uint32_t* tabCountsOcc(uint32_t* degs, uint32_t size_degs, uint32_t* size_occ) {
    assert(degs);

    uint32_t min = degs[0], max = degs[0], i;
    for (i=1; i<size_degs; i++) {
        if (degs[i] < min) min = degs[i];
        if (degs[i] > max) max = degs[i];
    }

    uint32_t *occ = (uint32_t*)malloc((max+1) * sizeof(uint32_t));
    assert(occ);
    for (i=0; i<max+1; i++) {
        occ[i] = 0;
    }
    for (i=0; i<size_degs; i++) {
        occ[degs[i]] += 1;
    }

    *size_occ = max+1;
    return occ;
}

uint32_t* tabEdgesConfig(uint32_t* occ, uint32_t size_occ, uint32_t* edges_size) {
    assert(occ && size_occ != 0);

    uint32_t nb_edges = 0;
    uint32_t i, j, k;
    for (i=0; i<size_occ; i++) {
        nb_edges += occ[i] * i;
    }
    uint32_t* edges = (uint32_t*)malloc(nb_edges * sizeof(uint32_t));
    assert(edges);

    uint32_t vrtx_id = 0, edge_id = 0;
    for (i=0; i<size_occ; i++) {
        for (k=0; k<occ[i]; k++) {
            for (j=0; j<i; j++) {
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

int _edgeListeDoublon(uint32_t a, uint32_t b, uint32_t* tab, uint32_t tab_start, uint32_t tab_size) {
    assert(tab != NULL && tab_size >= 0);

    if (tab_size == 0) return 0;

    tab += tab_start;
    for (uint32_t i=0; i<tab_size/2; i++) {
        if (tab[2*i] == a && tab[2*i+1] == b) {
            return 1;
        }
        if (tab[2*i] == b && tab[2*i+1] == a) {
            return 1;
        }
    }
    return 0;
}


int bernou(double prob) {
    assert(prob <= 1 && prob >= 0);
    double randFl = rand();
    randFl /= RAND_MAX;
    if (randFl <= prob) return 1;
    else return 0;
}