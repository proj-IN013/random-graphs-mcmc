#include "graphes.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
    free(half_edges);
    free(config_mdl);
    printf("\tnb essais %d\n", nbIter);Preset();
}

int main(void)
{
    srand(time(NULL));

    etoiles();

    LiAdj* li = listeInit(20);
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
