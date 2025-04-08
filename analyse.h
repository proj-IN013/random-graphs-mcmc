#ifndef GRAPHES_C_ANALYSE_H
#define GRAPHES_C_ANALYSE_H

#include "graphes.h"
#include <pthread.h>

#define NB_THREADS 100
#define SAMPLING_GAP 5000
#define MAX_STEPS 250000

typedef struct _graph Graph;
struct _graph {
    LiAdj *liAdj;
    uint32_t *edge_list;
    uint32_t swap_attempts;
    uint32_t swaps;
    uint32_t triangles;
};

typedef struct _thread Thread;
struct _thread {
    Graph *Gr;
    void (*updateMeasure)(Graph *Gr, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
    FILE* fd;
    pthread_t thread_id;
};

Graph *initGraph(LiAdj *liAdj);
void freeGraph(Graph *Gr);
Graph* dupGraph(Graph* Gr);
void countTriangles(Graph *Gr);
void updateTriangles(Graph *Gr, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
void genEdgeList(Graph *Gr);
uint32_t getRandomEdge(Graph *Gr, uint32_t *vrtxA, uint32_t *vrtxB);
void swapAndUpdate(Graph *Gr, void (*update)(Graph *Gr, uint32_t a, uint32_t b, uint32_t c, uint32_t d));
Thread *initThreadList(Graph *Gr, void (*updateMeasure)(Graph *Gr, uint32_t a, uint32_t b, uint32_t c, uint32_t d));
void *MarkovThread(void *arg);
void recordMeasuresThreads(Thread *threads);

#endif