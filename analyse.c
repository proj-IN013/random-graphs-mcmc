#include "analyse.h"

Graph *initGraph(LiAdj *liAdj)
{
    assert(liAdj);

    Graph *Gr = (Graph*)malloc(sizeof(Graph));
    assert(Gr);

    Gr->liAdj = liAdj;
    genEdgeList(Gr);
    Gr->swap_attempts = 0;
    Gr->swaps = 0;
    Gr->triangles = 0;
    countTriangles(Gr);

    return Gr;
}


void freeGraph(Graph *Gr)
{
    assert(Gr);

    freeList(Gr->liAdj);
    free(Gr->edge_list);

    free(Gr);
}


Graph* dupGraph(Graph* Gr)
{
    if (!Gr) return NULL;

    Graph *copy = malloc(sizeof(Graph));
    assert(copy != NULL);

    copy->liAdj = dupList(Gr->liAdj);
    copy->swap_attempts = Gr->swap_attempts;
    copy->swaps = Gr->swaps;
    copy->triangles = Gr->triangles;

    copy->edge_list = (uint32_t*)malloc(Gr->liAdj->nb_edges * sizeof(uint32_t));
    assert(copy->edge_list != NULL);
    memcpy(copy->edge_list, Gr->edge_list, Gr->liAdj->nb_edges * sizeof(uint32_t));

    return copy;
}


void countTriangles(Graph *Gr)
{
    assert(Gr);
    LiAdj *li = Gr->liAdj;
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
                        if (areteExiste(li, a, c)) {
                            nb_triang++;
                        }
                    }
                    vB = vB->next;
                }
            }
            vA = vA->next;
        }
    }
    Gr->triangles = nb_triang;
}


void updateTriangles(Graph *Gr, uint32_t a, uint32_t b, uint32_t c, uint32_t d)
{
    assert(Gr);
    LiAdj *li = Gr->liAdj;
    assert(li);

    assert(li->L[a] && li->L[b] && li->L[c] && li->L[d]);
    VrtxVoisin* temp = li->L[a];
    while (temp != NULL) {
        if (temp->id != c && temp->id != d && areteExiste(li, b, temp->id) == 1) {
            Gr->triangles--;
        }
        if (areteExiste(li, c, temp->id) == 1) {
            Gr->triangles++;
        }
        temp = temp->next;
    }
    temp = li->L[d];
    while (temp != NULL) {
        if (temp->id != a && temp->id != b && areteExiste(li, c, temp->id) == 1) {
            Gr->triangles--;
        }
        if (areteExiste(li, b, temp->id) == 1) {
            Gr->triangles++;
        }
        temp = temp->next;
    }
}


void genEdgeList(Graph *Gr)
{
    assert(Gr);
    LiAdj *li = Gr->liAdj;
    assert(li);

    uint32_t* edge_tab = (uint32_t*)calloc(li->nb_edges, sizeof(uint32_t));
    assert(edge_tab);
    uint32_t i, k=0;
    VrtxVoisin* voiz;
    for (i=0; i < li->nb_vrtx; i++) {
        voiz = li->L[i];
        while (voiz != NULL && voiz->id <= i) {
            edge_tab[k] = i;
            edge_tab[k+1] = voiz->id;
            k+=2;
            voiz = voiz->next;
        }
    }
    Gr->edge_list = edge_tab;
}


uint32_t getRandomEdge(Graph *Gr, uint32_t *vrtxA, uint32_t *vrtxB)
{
    assert(Gr);
    assert(Gr->liAdj);
    assert(Gr->edge_list);

    uint32_t edge = random()%(Gr->liAdj->nb_edges/2) *2;
    *vrtxA = Gr->edge_list[edge];
    *vrtxB = Gr->edge_list[edge+1];
    return edge;
}


void swapAndUpdate(Graph *Gr, void (*update)(Graph *Gr, uint32_t a, uint32_t b, uint32_t c, uint32_t d))
{
    assert(Gr);
    assert(Gr->liAdj);
    assert(Gr->edge_list);

    uint32_t a,b,c,d;
    uint32_t edgeAB, edgeCD;
    edgeAB = getRandomEdge(Gr, &a, &b);
    edgeCD = getRandomEdge(Gr, &c, &d);

    if (swap(Gr->liAdj, a, b, c, d, 2) == 0) {
        update(Gr, a, b, c, d);
        swapTab(Gr->edge_list, edgeAB + 1, edgeCD);
        Gr->swaps++;
    } else if (swap(Gr->liAdj, a, b, d, c, 2) == 0) {
        update(Gr, a, b, d, c);
        swapTab(Gr->edge_list, edgeAB + 1, edgeCD + 1);
        Gr->swaps++;
    }
    //Pred();printf("SWAP");Preset();printf(" : %d %d %d\n", Gr->triangles, Gr->swap_attempts, Gr->swaps);
    Gr->swap_attempts++;
}


int kswapAndUpdate(Graph *Gr) {
    uint32_t k = 2;//tirage_zipf(Gr->liAdj->nb_edges/2 -1, .01) + 1;
    //printf("k = %d\n", k);

    uint32_t *liste_vrtx = malloc(2 * k * sizeof(uint32_t));
    uint32_t *indices = tirage_entiers_distincts(k, Gr->liAdj->nb_edges/2);
    assert(liste_vrtx != NULL && indices != NULL);
    for (uint32_t i = 0; i < k; i++) {
        //if (2*indices[i]+1 >= 5848) {printf("WWAAAAAZAAA\n");}
        //printf("%d %d\n", 2*indices[i]+1, Gr->liAdj->nb_edges);
        //printf("%d\n", 2*indices[i]+1);
        liste_vrtx[i] = Gr->edge_list[2*indices[i]];
        liste_vrtx[k + i] = Gr->edge_list[2*indices[i]+1];
    }

    uint32_t *perm = malloc(k * sizeof(uint32_t));
    assert(perm);
    //printtab(liste_vrtx, 2*k);
    genPerm(perm, liste_vrtx, k);
    //printtab(perm, k);

    int res = kswap(Gr->liAdj, liste_vrtx, perm, k);
    //int delta = 0;
    printf("%d\n", res);
    exit(EXIT_SUCCESS);

    if (res == 0) {
        //delta = Gr->triangles;
        //int preswap = Gr->triangles;
        //countTriangles(Gr);
        //int postswap = Gr->triangles;
        //delta = postswap - 44;

        if (1<= 2) {
            for (uint32_t i = 0; i<k; i++) {
                Gr->edge_list[2*indices[i]+1] = perm[i];
            }
            //printf("\tk : %d\ttriangles : %d\tdelta : %d\ttotal : %d\n", k, Gr->triangles, delta, Gr->triangles+delta);

            //printf("\t\t\t\ttotal : %d\n", Gr->triangles);
        } else {
            rollback_kswap(Gr->liAdj, liste_vrtx, perm, k);
            //Gr->triangles = preswap;
        }
    }

    free(perm);
    free(liste_vrtx);
    free(indices);
    return res;//(res == 0 && 1 <= 2) ? 0 : -1;
}


Thread *initThreadList(Graph *Gr, void (*updateMeasure)(Graph *Gr, uint32_t a, uint32_t b, uint32_t c, uint32_t d))
{
    Thread *threads = (Thread*)malloc(NB_THREADS * sizeof(Thread));
    for (int i=0; i<NB_THREADS; i++) {
        threads[i].Gr = dupGraph(Gr);
        threads[i].updateMeasure = updateMeasure;
        threads[i].thread_id = 0;
        char str[64];
        snprintf(str, 64, "outputs/threads/%d.txt", i);
        Ppurple();printf("%s\n", str);Preset();
        threads[i].fd = startLog(str);
        //fprintf(threads[i].fd, "%d\n", threads[i].Gr->triangles);
        fprintf(threads[i].fd, "%.5f\n", computeAssortativity(threads[i].Gr->liAdj));


        pthread_create(&(threads[i].thread_id), NULL, MarkovThread, &threads[i]);
    }

    for (int i=0; i<NB_THREADS; i++) {
        pthread_join(threads[i].thread_id, NULL);
    }
    return threads;
}


void *MarkovThread(void *arg)
{
    Thread *thread = (Thread*)arg;
    uint32_t i=0, j=0;

    while (i<MAX_STEPS) {

        for (j=0; j<SAMPLING_GAP; j++) {
            //swapAndUpdate(thread->Gr, thread->updateMeasure);
            kswapAndUpdate(thread->Gr);
            //printf("Thread [%p] Swap i=%d j=%d\n", (void *)thread, i, j);
        }
        i+=j;
        //printf("Thread [%p] fin des swaps\n", (void *)thread);

        fprintf(thread->fd, "%.5f\n", computeAssortativity(thread->Gr->liAdj));
        //fprintf(thread->fd, "%d\n", thread->Gr->triangles);
    }
    printf("Thread [%p] fin du processus\n", (void *)thread);

    fclose(thread->fd);
    freeGraph(thread->Gr);
    pthread_exit(NULL);
    return NULL;
}

void genPerm(uint32_t *perm, uint32_t *orig, uint32_t k) {
    // Initialiser perm = orig
    for (uint32_t i = 0; i < k; i++) perm[i] = orig[i];

    // Fisher-Yates + correction pour éviter les points fixes
    for (uint32_t i = k - 1; i > 0; i--) {
        uint32_t j = rand() % (i + 1);
        // S'assurer qu'on ne crée pas un point fixe
        if (perm[j] == orig[i]) {
            // Si perm[j] == orig[i], échanger avec quelqu’un d’autre
            if (j != 0) j--;
            else j++;
        }
        uint32_t tmp = perm[i];
        perm[i] = perm[j];
        perm[j] = tmp;
    }

    // Si dernier point fixe, échanger avec précédent
    if (perm[0] == orig[0]) {
        uint32_t tmp = perm[0];
        perm[0] = perm[1];
        perm[1] = tmp;
    }
}


double computeAssortativity(LiAdj *li)
{
    const double M = (double)(li->nb_edges / 2);      /* #arêtes */
    if (M == 0.0) return 0.0;

    double sum_jk = 0.0, sum_jpk = 0.0, sum_j2pk2 = 0.0;

    for (uint32_t u = 0; u < li->nb_vrtx; ++u) {
        uint32_t du = vrtxDeg(li->L[u]);
        for (VrtxVoisin *v = li->L[u]; v; v = v->next) {
            uint32_t w = v->id;
            if (w > u) {
                uint32_t dw = vrtxDeg(li->L[w]);
                sum_jk += (double)du * dw;
                sum_jpk += (double)(du + dw);
                sum_j2pk2 += (double)(du*du + dw*dw);
            }
        }
    }
    double a = sum_jk / M;
    double b = sum_jpk / (2.0*M);
    double c = sum_j2pk2 / (2.0*M);
    double num = a - b*b;
    double den = c - b*b;

    /*  variance nulle  →  graphe parfaitement assortatif  */
    if (fabs(den) < 1e-12)          /* tous les degrés identiques            */
        return 1.0;                 /* on renvoie r = 1                      */

    return num / den;
}