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

    if (swap(Gr->liAdj, a, b, c, d, 2) == 1) {
        update(Gr, a, b, c, d);
        swapTab(Gr->edge_list, edgeAB + 1, edgeCD);
        Gr->swaps++;
    }
    //Pred();printf("SWAP");Preset();printf(" : %d %d %d\n", Gr->triangles, Gr->swap_attempts, Gr->swaps);
    Gr->swap_attempts++;
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
        fprintf(threads[i].fd, "%d\n", threads[i].Gr->triangles);


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
            swapAndUpdate(thread->Gr, thread->updateMeasure);
            //printf("Thread [%p] Swap i=%d j=%d\n", (void *)thread, i, j);
        }
        i+=j;
        //printf("Thread [%p] fin des swaps\n", (void *)thread);

        fprintf(thread->fd, "%d\n", thread->Gr->triangles);
    }

    fclose(thread->fd);
    freeGraph(thread->Gr);
    pthread_exit(NULL);
    return NULL;
}