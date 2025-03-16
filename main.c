#include "graphes.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_ITER 100000

void genfromdist(FILE* plot, uint32_t* config_tab, uint32_t size_cfg_mdl, uint32_t samples) {
    printtab(config_tab, size_cfg_mdl);
    uint64_t nb_triesA = 0, nb_triesB = 0;
    for (uint32_t i=0; i<samples; i++) {
        uint32_t half_edges_tab_size, graph_size;
        uint32_t* half_edgesA = tabEdgesConfig(config_tab, size_cfg_mdl, &half_edges_tab_size, &graph_size);
        uint32_t* half_edgesB = duptab(half_edgesA, half_edges_tab_size);
        nb_triesA += (uint64_t)repeatedSortEdgetab(half_edgesA, half_edges_tab_size, MAX_ITER);
        nb_triesB += (uint64_t)iterSortEdgetab(half_edgesB, half_edges_tab_size, MAX_ITER);
        free(half_edgesA);
        free(half_edgesB);
    }
    Pblue();printf("%llu, %llu\n", nb_triesA, nb_triesB);Preset();
    double moyA = (double)nb_triesA/samples;
    double moyB = (double)nb_triesB/samples;
    fprintf(plot, "%.2f %.2f\n", moyA, moyB);
}

int main(void)
{
    srand(time(NULL));
    LiAdj* li = listeInit(20);

/** 
graphes sans aucun triangle
    listeAdd(li, 0, 1);
    listeAdd(li, 2, 3);
    listeAdd(li, 4, 5);
    listeAdd(li, 6, 7);
    listeAdd(li, 8, 9);
    listeAdd(li, 10, 11);
    listeAdd(li, 12, 13);
    listeAdd(li, 14, 15);
    listeAdd(li, 16, 17);
    listeAdd(li, 18, 19);
**/


/**
graphes avec 7 trianles + random

    listeAdd(li, 0, 1);
    listeAdd(li, 1, 2);
    listeAdd(li, 2, 0);

    // 345
    listeAdd(li, 3, 4);
    listeAdd(li, 4, 5);
    listeAdd(li, 5, 3);

    // 678
    listeAdd(li, 6, 7);
    listeAdd(li, 7, 8);
    listeAdd(li, 8, 6);

    // 9 10 11
    listeAdd(li, 9, 10);
    listeAdd(li, 10, 11);
    listeAdd(li, 11, 9);

    // 12 13 14
    listeAdd(li, 12, 13);
    listeAdd(li, 13, 14);
    listeAdd(li, 14, 12);

    // 15 16 17
    listeAdd(li, 15, 16);
    listeAdd(li, 16, 17);
    listeAdd(li, 17, 15);

    // 18 19 0
    listeAdd(li, 18, 19);
    listeAdd(li, 19, 0);
    listeAdd(li, 0, 18);


    listeAdd(li, 0, 4); 
    listeAdd(li, 2, 5);
    listeAdd(li, 3, 8);
    listeAdd(li, 6, 9);
    listeAdd(li, 7, 10);
**/


/**
graphes solely triangulaires
**/
    listeAdd(li, 0, 1);
    listeAdd(li, 1, 2);
    listeAdd(li, 2, 0);

    listeAdd(li, 3, 4);
    listeAdd(li, 4, 5);
    listeAdd(li, 5, 3);

    listeAdd(li, 6, 7);
    listeAdd(li, 7, 8);
    listeAdd(li, 8, 6);

    listeAdd(li, 9, 10);
    listeAdd(li, 10, 11);
    listeAdd(li, 11, 9);

    listeAdd(li, 12, 13);
    listeAdd(li, 13, 14);
    listeAdd(li, 14, 12);

    listeAdd(li, 15, 16);
    listeAdd(li, 16, 17);
    listeAdd(li, 17, 15);

    listeAdd(li, 18, 19);
    listeAdd(li, 19, 0);
    listeAdd(li, 0, 18);


    listePrint(li);
    uint32_t nb_triangles = compterTriangles(li);
    printf("Nombre de triangles dans le graphe : %" PRIu32 "\n", nb_triangles);

    listeFree(li);
 
    return 0;
}
