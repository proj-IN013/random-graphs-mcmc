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

int listeAddSingle(LiAdj* li, uint32_t vrtx, uint32_t vzn_id) {
    assert(li != NULL);
    assert(vrtx >= 0 && vzn_id >= 0);
    assert(vrtx < li->nb_vrtx);
    if (vrtx == vzn_id) return -1;

    if (li->L[vrtx] == NULL || li->L[vrtx]->id > vzn_id) {
        li->L[vrtx] = vrtxVoisinInit(vzn_id, li->L[vrtx]);
        li->nb_edges += 1;
        return 0;
    }

    VrtxVoisin* voisin = li->L[vrtx];
    if (voisin->id == vzn_id) return -1;
    while (voisin->next != NULL && voisin->next->id <= vzn_id) {
        if (voisin->next->id == vzn_id) return -1;
        voisin = voisin->next;
    }

    VrtxVoisin* new = vrtxVoisinInit(vzn_id, voisin->next);
    voisin->next = new;
    li->nb_edges += 1;
    return 0;
}

void listeAdd(LiAdj* li, uint32_t va, uint32_t vb) {
    listeAddSingle(li, va, vb);
    listeAddSingle(li, vb, va);
}

void listeRemoveSingle(LiAdj* li, uint32_t va, uint32_t vb) {
    VrtxVoisin* tmp = li->L[va];
    if (tmp->id == vb) {
        li->L[va] = tmp->next;
        free(tmp);
        li->nb_edges--;
        return;
    }
    while (tmp->next) {
        if (tmp->next->id == vb) {
            VrtxVoisin* del = tmp->next;
            tmp->next = tmp->next->next;
            free(del);
            li->nb_edges--;
            return;
        }
        tmp = tmp->next;
    }
}

void listeRemove(LiAdj* li, uint32_t va, uint32_t vb) {
    listeRemoveSingle(li, va, vb);
    listeRemoveSingle(li, vb, va);
}

uint8_t areteExiste(LiAdj* li, uint32_t va, uint32_t vb) {
    assert(li);
    assert(va < li->nb_vrtx && vb < li->nb_vrtx);
    int a = vrtxEstVoisin(li->L[va], vb);
    int b = vrtxEstVoisin(li->L[vb], va);
    if (a==1 && b==1) return 1;
    if (a==1 || b==1) exit(EXIT_FAILURE);
    return 0;
}

LiAdj* listeLoad(char* fname) {
    LiAdj* li = listeInit( countVrtx(fname)+1 );
    readFile(fname, &_edges_liste, li, 1);
    return li;
}

void listeSave(LiAdj* li, char* fname) {
    FILE* fic = startLog(fname);
    fprintf(fic, "%s, n = %d, m = %d\n", fname, li->nb_vrtx, li->nb_edges/2);
    VrtxVoisin *currentvoiz;
    for (uint32_t i=0; i < li->nb_vrtx; i++) {
        currentvoiz = li->L[i];
        while (currentvoiz != NULL && currentvoiz->id < i) {
            fprintf(fic, "%d %d\n", i+1, currentvoiz->id+1);
            currentvoiz = currentvoiz->next;
        }
    }
    fclose(fic);
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
    uint32_t unique_vrtx = 0, i = 1, max_attempts = 100;

    while (i <= max_attempts) {
        //float densite = 200*m;
        //densite /= n*(n+1); //     m ÷ (2 parmi n)
        //printf("densité : %.2f%%\n", densite);

        LiAdj* li = listeInit(n);
        unique_vrtx = 0;
        for (uint32_t j=0; j<m; j++) {
            int va = rand()%n, gap = rand()%(n-1) +1;
            int vb = (va + gap)%n;
            if (li->L[va] == NULL) unique_vrtx++;
            if (li->L[vb] == NULL && va != vb) unique_vrtx++;
            listeAdd(li, va, vb);
        }
        if (unique_vrtx == n) return li;
        printf("error %d\n", n - unique_vrtx);
        listeFree(li);
        i++;
    }
    return NULL;
}

LiAdj* erdosRenyi2(uint32_t n, uint32_t m) {
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

uint32_t* listeDegDistrib(LiAdj* li, uint32_t* tab_size) {
    assert(li && tab_size);
    uint32_t *degs = (uint32_t*)malloc(li->nb_vrtx * sizeof(uint32_t));
    assert(degs);
    for (uint32_t i=0; i < li->nb_vrtx; i++) {
        degs[i] = vrtxDeg(li->L[i]);
    }
    uint32_t *occ = tabCountsOcc(degs, li->nb_vrtx, tab_size);
    free(degs);

    return occ;
}

void listeSaveDegDistrib(LiAdj* li, char* nom_plot) {
    assert(li);
    FILE* plot = startLog(nom_plot);

    uint32_t *degs = (uint32_t*)malloc(li->nb_vrtx * sizeof(uint32_t));
    assert(degs);
    uint32_t size_occ = 0;
    for (uint32_t i=0; i < li->nb_vrtx; i++) {
        degs[i] = vrtxDeg(li->L[i]);
    }
    uint32_t *occ = tabCountsOcc(degs, li->nb_vrtx, &size_occ);
    free(degs);
    for (uint32_t i=0; i<size_occ; i++) {
        if (occ[i] == 0 && i!=size_occ-1) continue;
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
        printf("%d :\t", i);//+1
        vrtxVoisinsPrint(li->L[i]);
    }
    printf("\n");
}

void listeRender(LiAdj* li, char* graph_name, int do_open) {
    assert(li && graph_name);
    char dot_path[50];
    char png_path[50];
    snprintf(dot_path, 50, "outputs/%s.dot", graph_name);
    snprintf(png_path, 50, "outputs/%s.png", graph_name);
    FILE *fic = startLog(dot_path);

    fprintf(fic, "graph G {\n");
    for (uint32_t i = 0; i < li->nb_vrtx; i++) {
        VrtxVoisin* voiz = li->L[i];
        while (voiz) {
            if (i < voiz->id) {
                fprintf(fic, "    %d -- %d;\n", i+1, voiz->id);//+1
            }
            voiz = voiz->next;
        }
    }
    fprintf(fic, "}\n");
    fclose(fic);
    //-Kneato, -Kdot, -Kcirco
    if (do_open == 1) {
        if (fork() == 0) {
            execl("/opt/homebrew/bin/dot", "dot", "-Tpng", "-Kcirco", dot_path, "-o", png_path, NULL);
            exit(EXIT_FAILURE);
        } else wait(NULL);
        if (fork() == 0) {
            execl("/bin/rm", "rm", dot_path, NULL);
            exit(EXIT_FAILURE);
        } else wait(NULL);
        if (fork() == 0) {
            execl("/usr/bin/open", "open", png_path, NULL);
            exit(EXIT_FAILURE);
        } else wait(NULL);
    }
}

// VOISINS : LISTE CHAINÉE

VrtxVoisin* vrtxVoisinInit(uint32_t id, VrtxVoisin* next) {
    assert(id >= 0);

    VrtxVoisin* new_vrtx = (VrtxVoisin*)malloc(sizeof(VrtxVoisin));
    assert(new_vrtx);

    new_vrtx->id = id;
    new_vrtx->next = next;
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
    uint32_t i = 0;
    while (vrtx != NULL) {
        vrtx = vrtx->next;
        i++;
    }
    return i;
}

uint8_t vrtxEstVoisin(VrtxVoisin* vrtx, uint32_t voiz) {
    assert(vrtx);
    while (vrtx != NULL) {
        if (voiz == vrtx->id) {
            return 1;  
        }
        vrtx = vrtx->next;
    }
    return 0;  
}


void vrtxVoisinsPrint(VrtxVoisin* vrtx) {
    while (vrtx != NULL) {
        printf("%d ", vrtx->id);//+1
        vrtx = vrtx->next;
    }
    printf("\n");
}


void readFile(char* fname, void (*actionOnLine)(uint32_t va, uint32_t vb, void* ret), void* ret, int do_remove_header) {
    FILE* fic = fopen(fname, "r");
    assert(fic);

    if (do_remove_header == 1) {
        char header[256];  // pas de données sur la première ligne
        fgets(header, 256, fic);
    }

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
    readFile(fname, &_unique_vrtx_liste, li, 1);
    uint32_t nb = li->nb_edges;
    listeFree(li);
    return nb;
}

uint32_t* loadConfigModel(char* fname, uint32_t* size_config_model) {
    *size_config_model = 0;
    readFile(fname, &_find_max, size_config_model, 0);
    (*size_config_model)++;
    uint32_t* tab = (uint32_t*)malloc(sizeof(uint32_t)* (*size_config_model) );
    assert(tab);
    for (uint32_t i=0; i< *size_config_model; i++) tab[i] = 0;
    readFile(fname, &_config_model_tab, tab, 0);
    return tab;
}

void _printline(uint32_t va, uint32_t vb, void* test) {
    printf("%d %d\n", va, vb);
}

void _unique_vrtx_liste(uint32_t va, uint32_t vb, void* li) {
    listeAddSingle((LiAdj*)li, 0, va);
    listeAddSingle((LiAdj*)li, 0, vb);
}

void _edges_liste(uint32_t va, uint32_t vb, void* li) {
    listeAdd(li, va, vb);
}

void _find_max(uint32_t va, uint32_t vb, void* max) {
    va++;vb++;
    if (va > *(uint32_t*)max) *(uint32_t*)max = va;
    (void)vb;
}

void _config_model_tab(uint32_t va, uint32_t vb, void* tab) {
    va++;vb++;
    ((uint32_t*)tab)[va] = vb;
}

FILE* startLog(char* fname) {
    FILE *fic = fopen(fname, "w");
    assert(fic);
    fclose(fic);
    return fopen(fname, "a");
}

void swapTab(uint32_t* tab, uint32_t i, uint32_t k) {
    assert(tab);

    uint32_t tmp = tab[i];
    tab[i] = tab[k];
    tab[k] = tmp;
}

void shuffle(uint32_t* tab, uint32_t size) {
    uint32_t* shuffled = (uint32_t*)malloc(size * sizeof(uint32_t));
    assert(shuffled);
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
    uint32_t i, va, vb;
    for (i=0; i<tab_size/2; i++) {
        //printtab(tab, tab_size);
        vb = rand()%(tab_size-2*i);
        va = vb;
        while (va == vb) {
            va = rand()%(tab_size-2*i);
        }
        //printf("tab[%d] = %d \t tab[%d] = %d \t\n", va, tab[va], vb, tab[vb]);
        if (tab[va] == tab[vb] || _edgeListeDoublon(tab[va], tab[vb], tab, tab_size-2*i, 2*i)==1) return NULL;
        swapTab(tab, vb, tab_size -2*i- 1);
        if (tab_size -2*i- 1 == va) va = vb;
        swapTab(tab, va, tab_size -2*(i+1));
        //printtab(tab, tab_size);
        //printf("\n");
    }
    return tab;
}

uint32_t* iterSortEdgetab(uint32_t* tab, uint32_t tab_size, uint32_t max_iter, uint32_t* nb_tries) {
    assert(tab != NULL && tab_size > 0);

    uint32_t i=0;
    uint32_t *res;
    do {
        shuffle(tab, tab_size);
        res = sortEdgetab(tab, tab_size);
        i++;
        //printf("\n\n");
    }
    while (res==NULL && i<max_iter);
    *nb_tries = i;
    return res;
}

void printtab(uint32_t* tab, uint32_t tab_size) {
    if (tab == NULL) return;

    printf("[");
    for (uint32_t i=0; i<tab_size-1; i++) {
        printf("%d, ", tab[i]);
    }
    printf("%d]\n", tab[tab_size-1]);
}

uint32_t* duptab(uint32_t* tab, uint32_t tab_size) {
    assert(tab && tab_size);

    uint32_t* newtab = (uint32_t*)malloc(sizeof(uint32_t)*tab_size);
    for (uint32_t i=0; i<tab_size-1; i++) {
        newtab[i] = tab[i];
    }
    return newtab;
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

uint32_t* tabEdgesConfig(uint32_t* occ, uint32_t size_occ, uint32_t* edges_size, uint32_t* graph_size) {
    assert(occ && size_occ != 0);

    uint32_t nb_half_edges = 0;
    uint32_t nb_vrtx = 0;
    uint32_t i, j, k;
    for (i=0; i<size_occ; i++) {
        nb_half_edges += occ[i] * i;
        nb_vrtx += occ[i];
    }
    uint32_t* edges = (uint32_t*)malloc(nb_half_edges * sizeof(uint32_t));
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
    shuffle(edges, nb_half_edges);
    *edges_size = nb_half_edges;
    *graph_size = nb_vrtx;
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

void swap(LiAdj* li, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint8_t do_test) {
    if (do_test >= 1) {
        assert(areteExiste(li, a, c) == 0);
        assert(areteExiste(li, b, d) == 0);
        if (do_test == 2) {
            assert(a != b && a != c && a != d && b != c && b != d && c!= d);
            assert(areteExiste(li, a, b) == 1);
            assert(areteExiste(li, c, d) == 1);
        }
    }

    listeRemove(li, a, b);
    listeRemove(li, c, d);

    listeAdd(li, a, c);
    listeAdd(li, b, d);
}


int bernou(double prob) {
    assert(prob <= 1 && prob >= 0);
    double randFl = rand();
    randFl /= RAND_MAX;
    if (randFl <= prob) return 1;
    else return 0;
}


uint32_t compterTriangles(LiAdj* li) {
    assert(li);

    uint32_t nb_triang = 0;
    for (uint32_t a = 0; a < li->nb_vrtx; a++) {
        VrtxVoisin* vA = li->L[a];
        while (vA != NULL) {
            uint32_t b = vA->id;
            // pas faire doublons
            if (b > a) {
                VrtxVoisin* vB = li->L[b];
                while (vB != NULL) {
                    uint32_t c = vB->id;
                    if (c > b && c > a) {
                        if (vrtxEstVoisin(li->L[a], c)) {
                            nb_triang++;
                        }
                    }
                    vB = vB->next;
                }
            }
            vA = vA->next;
        }
    }

    return nb_triang;
}


void actualiserTriangle(LiAdj* li, uint32_t a, uint32_t b, uint32_t c, uint32_t d,uint32_t *nb_triangles){
    assert(li != NULL);
    VrtxVoisin* vA = li->L[a];
    while (vA != NULL) {
        uint32_t temp = vA->id;
        //si temp est un voisin de c, ajouter un triangle (puisque lien entre a et c est nouveau)
        if (vrtxEstVoisin(li->L[c], temp)) {
            (*nb_triangles)++;
            printf("voisin trouvé pour %d et %d\n",a,c);
        }
        //si temp est un voisin de b, supprimer un triangle
        if (vrtxEstVoisin(li->L[b], temp)) {
            (*nb_triangles)--;
            printf("voisin trouvé pour %d et %d \n",a,b);
        }
        vA = vA->next;
    }
    // meme chose avec d
    VrtxVoisin* vD = li->L[d];
    while (vD != NULL) {
        uint32_t temp = vD->id;
        if (vrtxEstVoisin(li->L[c], temp)) {
            (*nb_triangles)--;
            printf("voisin trouvé pour %d et %d\n",d,c);
        }
        if (vrtxEstVoisin(li->L[b], temp)) {
            (*nb_triangles)++;
            printf("voisin trouvé pour %d et %d\n",d,b);
        }

        vD = vD->next;
    }

    return ;
}

