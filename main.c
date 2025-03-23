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
            fprintf(plot, "%u\n", step_tries);

        } else samples--;
        free(half_edges);
    }
    if (total_tries != samples*MAX_ITER) Pblue();
    else Pred();
    double moy = (double)total_tries/samples;
    printf("\t%f\tsamples : %d\n", moy, samples);Preset();
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

uint32_t buildfromdist(char* fname) {
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
    return nbIter;
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
    uint32_t size_cfg_mdl1, size_cfg_mdl2;
    uint32_t* erdr = loadConfigModel("data/testerdr/erdrDD1.txt", &size_cfg_mdl1);
    uint32_t* dd1  = loadConfigModel("data/dd1.txt", &size_cfg_mdl2);

    FILE *plot1 = startLog("outputs/res1.txt");
    meanBuildTime(plot1, erdr, size_cfg_mdl1, 1000);
    FILE *plot2 = startLog("outputs/res2.txt");
    meanBuildTime(plot2, dd1 , size_cfg_mdl2, 1000);
    fclose(plot1);
    fclose(plot2);
    return 0;
}
