#include "graphes.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Pcolors.h"

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

    FILE *plot = startLog("outputs/funcslogs.txt");
    const uint32_t sizemax = 14;
    uint32_t config_tab[sizemax+1] = {0};
    for (uint32_t i=1; i<=ceil(sizemax/2.0); i++) {
        config_tab[i] = sizemax-i+1;
        config_tab[sizemax-i+1] = i;
        genfromdist(plot, config_tab, sizemax+1, 20);
    }
    fclose(plot);
    printf("\n");
    exit(EXIT_SUCCESS);
    uint32_t size_cfg_mdl;
    uint32_t* tabTest = loadConfigModel("data/dd1.txt", &size_cfg_mdl);
    printtab(tabTest, size_cfg_mdl);
    uint32_t edges_size, graph_size;
    uint32_t* edges = tabEdgesConfig(tabTest, size_cfg_mdl, &edges_size, &graph_size);
    int tries = iterSortEdgetab(edges, edges_size, 15);
    printf("%d essais, taille : %d\n", tries, edges_size);
    LiAdj* li = tabEdges2Liste(graph_size,edges, edges_size);
    //listePrintStats(li);
    //listePrint(li);
    //listeSaveDegDistrib(li, "test.txt");
    listeFree(li);

    Pred();printf("CONFIG MODEL ERDOS RENYI\n\n");Preset();
    return 0;
}
