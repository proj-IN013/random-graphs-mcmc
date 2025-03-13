#include "graphes.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Pcolors.h"

int main(void)
{
    uint32_t size_cfg_mdl;
    uint32_t* tabTest = loadConfigModel("../data/dd1.txt", &size_cfg_mdl);
    printtab(tabTest, size_cfg_mdl);
    uint32_t edges_size, graph_size;
    uint32_t* edges = tabEdgesConfig(tabTest, size_cfg_mdl, &edges_size, &graph_size);
    int tries = iterSortEdgetab(edges, edges_size, 15);
    printf("%d essais, taille : %d\n", tries, edges_size);
    LiAdj* li = tabEdges2Liste(graph_size,edges, edges_size);
    //listePrintStats(li);
    //listePrint(li);
    //listeDegDistrib(li, "test.txt");
    listeFree(li);

    Pred();printf("CONFIG MODEL ERDOS RENYI\n\n");Preset();
    LiAdj* config_gr = erdosRenyi(100, 4000);
    listeDegDistrib(config_gr, "erdr60.txt");

    uint32_t size_cfg_mdl2;
    uint32_t* config_tab = loadConfigModel("erdr60.txt", &size_cfg_mdl2);
    printtab(config_tab, size_cfg_mdl2);
    uint32_t edges_size2, graph_size2;
    uint32_t* edges2 = tabEdgesConfig(config_tab, size_cfg_mdl2, &edges_size2, &graph_size2);
    int tries2 = iterSortEdgetab(edges2, edges_size2, 100);
    printf("%d essais, taille : %d\n", tries2, edges_size2);
    LiAdj* li2 = tabEdges2Liste(graph_size,edges2, edges_size2);
    listePrintStats(li2);
    return 0;
}
