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
    listePrintStats(li);
    listePrint(li);
    listeDegDistrib(li);
    listeFree(li);

    // LECTURE GRAPHE
    LiAdj* li2 = listeLoad("/Users/floux/Documents/cours/L2/projet_13/data/hiv/out.hiv");
    listePrint(li2);
    printf("%d %d\n\n", li2->nb_vrtx, li2->nb_edges/2);
    listeFree(li2);

    // ERDOS RENYI
    LiAdj* erdr = erdosRenyi(26039, 214817);
    listePrintStats(erdr);
    printf("\n");
    listeDegDistrib(erdr);
    listeFree(erdr);

    return 0;
}
