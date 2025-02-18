#include "graphes.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    //Graphe* test = grLoad("/Users/floux/Documents/cours/L2/projet_13/data/hiv/out.hiv", 0, 1);
    //listePrint(test->liste);
    //printf("\n\n%d %d\n", test->liste->nb_vrtx, test->liste->nb_edges/2);
    //grFree(test);

    //Graphe* erdr = erdosRenyi(26039, 214817);
    //listePrint(erdr->liste);
    //grPrintStats(erdr);
    //printf("\n");
    //grDegDistrib(erdr);
    //grFree(erdr);
    int tabTest[] = {0,1, 2, 1, 2};
    int edges_size;
    int* edges = tabEdgesConfig(tabTest, 5, &edges_size);
    sortEdgetab(edges, 16);
    printtab(edges, 16);
    LiAdj* li = tabEdges2Liste(6,edges, 16);
    Graphe* test = grInit();
    test->liste = li;
    grPrintStats(test);
    listePrint(test->liste);
    grDegDistrib(test);
    grFree(test);

    return 0;
}
