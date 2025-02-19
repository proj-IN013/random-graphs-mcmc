#include "graphes.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void)
{
    // GEN <- MODELE CONFIG
    uint32_t tabTest[] = {0,1, 2, 1, 2};
    uint32_t edges_size;
    uint32_t* edges = tabEdgesConfig(tabTest, 5, &edges_size);
    sortEdgetab(edges, edges_size);
    printtab(edges, edges_size);
    LiAdj* li = tabEdges2Liste(6,edges, edges_size);
    Graphe* test2 = grInit();
    test2->liste = li;
    grPrintStats(test2);
    listePrint(test2->liste);
    grDegDistrib(test2);
    grFree(test2);

    // LECTURE GRAPHE
    Graphe* test = grLoad("/Users/floux/Documents/cours/L2/projet_13/data/hiv/out.hiv");
    listePrint(test->liste);
    printf("%d %d\n\n", test->liste->nb_vrtx, test->liste->nb_edges/2);
    grFree(test);

    // ERDOS RENYI
    Graphe* erdr = erdosRenyi(26039, 214817);
    grPrintStats(erdr);
    printf("\n");
    grDegDistrib(erdr);
    grFree(erdr);

    return 0;
}
