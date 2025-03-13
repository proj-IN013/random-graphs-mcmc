#include "graphes.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void)
{
    uint32_t* tabTest = loadConfigModel("../data/dd1.txt", 40);
    printtab(tabTest, 41);
    uint32_t edges_size, graph_size;
    uint32_t* edges = tabEdgesConfig(tabTest, 41, &edges_size, &graph_size);
    int a = iterSortEdgetab(edges, edges_size, 15);
    printf("%d essais, taille : %d\n", a, edges_size);
    LiAdj* li = tabEdges2Liste(graph_size,edges, edges_size);
    listePrintStats(li);
    listePrint(li);
    listeDegDistrib(li);
    listeFree(li);
    return 0;
}
