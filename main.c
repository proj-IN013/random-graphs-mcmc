#include "analyse.h"
#include <stdio.h>
#include <stdlib.h>

void meanBuildTime(FILE* plot, uint32_t* config_tab, uint32_t size_cfg_mdl, uint32_t samples) {
    printtab(config_tab, size_cfg_mdl);
    uint64_t total_tries = 0;
    uint32_t step_tries = 0;
    uint32_t half_edges_tab_size, graph_size;
    uint32_t* half_edges = genHalfEdgesTab(config_tab, size_cfg_mdl, &half_edges_tab_size, &graph_size);

    for (uint32_t i=0; i<samples; i++) {
        shuffle(half_edges, half_edges_tab_size);
        printf("\r%.2f%%", 100.0*(i+1)/samples);fflush(stdout);
        half_edges = iterSortHalfEdgesTab(half_edges, half_edges_tab_size, MAX_ITER, &step_tries);

        if (half_edges != NULL) {
            total_tries += (uint64_t)step_tries;
            fprintf(plot, "%u\n", step_tries);

        } else samples--;
    }
    free(half_edges);
    if (total_tries != samples*MAX_ITER) Pblue();
    else Pred();
    double moy = (double)total_tries/samples;
    printf("\tessais : %f\tsamples : %d\n", moy, samples);Preset();
    fprintf(plot, "%.2f\n", moy);
}

int main(void)
{
    srand(time(NULL));
    Graph *Gr = initGraph(listeLoad("data/arenas-pgp/out.arenas-pgp"));
    //Graph *Gr = initGraph(listeLoad("data/arenas-jazz/out.arenas-jazz"));
    //Graph *Gr = initGraph(listeLoad("data/as-caida20071105/out.as-caida20071105"));
    //Graph *Gr = initGraph(genListFromCfgMdl("data/dd1.txt"));

    initThreadList(Gr, updateTriangles);
    Pgreen();printf("starting\n");Preset();
    //FILE *plot = startLog("outputs/caida.txt");

    //fclose(plot);
    return 0;
}
