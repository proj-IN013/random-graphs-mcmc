#include "graphes.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_ITER 200000

void meanBuildTime(FILE* plot, uint32_t* config_tab, uint32_t size_cfg_mdl, uint32_t samples) {
    printtab(config_tab, size_cfg_mdl);
    uint64_t total_tries = 0;
    uint32_t step_tries = 0;
    for (uint32_t i=0; i<samples; i++) {
        uint32_t half_edges_tab_size, graph_size;
        uint32_t* half_edges = tabEdgesConfig(config_tab, size_cfg_mdl, &half_edges_tab_size, &graph_size);
        half_edges = iterSortEdgetab(half_edges, half_edges_tab_size, MAX_ITER, &step_tries);
        if (half_edges != NULL) {
            total_tries += (uint64_t)step_tries;
        } else samples--;
        free(half_edges);
    }
    if (total_tries != samples*MAX_ITER) Pblue();
    else Pred();
    double moy = (double)total_tries/samples;
    printf("\t%f\tsamples : %d\n", moy, samples);Preset();
    fprintf(plot, "%.2f\n", moy);
}

void etoiles() {
    printf("Construction d'étoiles...\n");
    FILE *plot = startLog("outputs/funcslogs.txt");
    const uint32_t sizemax = 10;
    uint32_t config_tab[sizemax+1] = {0};
    for (uint32_t i=1; i<=ceil(sizemax/2.0); i++) {
        config_tab[i] = sizemax-i+1;
        config_tab[sizemax-i+1] = i;
        meanBuildTime(plot, config_tab, sizemax+1, 100);
    }
    fclose(plot);
}

void buildDD1() {
    printf("Construction de dd1...\n");
    uint32_t size_config_mdl, half_edges_tab_size, graph_size;
    uint32_t* config_mdl = loadConfigModel("data/dd1.txt", &size_config_mdl);
    uint32_t* half_edges = tabEdgesConfig(config_mdl, size_config_mdl, &half_edges_tab_size, &graph_size);
    uint32_t nbIter;
    half_edges = iterSortEdgetab(half_edges, half_edges_tab_size, MAX_ITER, &nbIter);
    if (half_edges == NULL) Pred();
    else Pgreen();
    LiAdj *quoicoubeh = tabEdges2Liste(graph_size, half_edges, half_edges_tab_size);
    free(half_edges);
    free(config_mdl);
    printf("\tnb essais %d\n", nbIter);Preset();
    printf("vrtx = %d, edges = %d\n", quoicoubeh->nb_vrtx, quoicoubeh->nb_edges);

}

void buildErdr(uint32_t n, uint32_t m) {
    printf("Construction de erdr...\n");
    LiAdj* erdr = erdosRenyi(n, m);
    uint32_t size_config_mdl, half_edges_tab_size, graph_size;
    uint32_t* config_mdl = listeDegDistrib(erdr, &size_config_mdl);
    uint32_t* half_edges = tabEdgesConfig(config_mdl, size_config_mdl, &half_edges_tab_size, &graph_size);
    uint32_t nbIter;
    half_edges = iterSortEdgetab(half_edges, half_edges_tab_size, MAX_ITER, &nbIter);
    if (half_edges == NULL) Pred();
    else Pgreen();
    //listeRender(tabEdges2Liste(graph_size, half_edges, half_edges_tab_size), "test73", 1);
    free(half_edges);
    free(config_mdl);
    printf("\tnb essais %d\n", nbIter);Preset();
}

int main(void)
{
    srand(time(NULL));

    //etoiles();
    //buildDD1();

//gLiAdj* erdr = erdosRenyi1(991, 1944);
    //LiAdj* erdr = erdosRenyi2(991, 1944);
    //listeSaveDegDistrib(erdr, "outputs/erdrDD1.txt");
    //listeSave(erdr, "outputs/SAVEDERDR.txt");
    //LiAdj* li = listeLoad("data/test1.txt");
    //listePrint(li);
    //uint32_t nb_triangles = compterTriangles(li);
    //printf("Nombre de triangles dans le graphe : %" PRIu32 "\n", nb_triangles);
    //listeRender(li, "123456", 1);
    //listeFree(li);
    

   /**   TEST POUR ACTUALISER TRIANGLE
    LiAdj* li = listeInit(8);  // Un graphe avec 8 sommets (0 à 7)

 
    listeAdd(li, 0, 1);
    listeAdd(li, 1, 2);
    listeAdd(li, 2, 0); // Triangle 0-1-2



    listeAdd(li, 3, 4);
    listeAdd(li, 4, 5);
    listeAdd(li, 5, 3); // Triangle 3-4-5

    listeAdd(li, 6, 7);
    listeAdd(li, 7, 0);
    listeAdd(li, 0, 6); // Triangle 6-7-0

  
    printf("Graphes avec triangles (avant l'échange) :\n");
    listePrint(li);

    uint32_t nb_triangles = compterTriangles(li);
    printf("Nombre initial de triangles : %" PRIu32 "\n", nb_triangles);

    swap(li, 0, 1, 4, 5, 1);

    printf("Graphes avec triangles (apres l'échange) :\n");
    listePrint(li);
    actualiserTriangle(li,0,1,4,5,&nb_triangles);
    printf("Nombre de triangles après swap : %" PRIu32 "\n", nb_triangles);


    listeFree(li);**/
    return 0;
}
