# Random Graphs MCMC
**Génération de graphes aléatoires** à l’aide de méthodes Monte-Carlo par chaînes de Markov, dans le cadre d'un projet universitaire encadré par Lionel Tabourier (LIP6 - Sorbonne Université) : [télécharger le rapport](https://github.com/proj-IN013/random-graphs-mcmc/raw/main/Graphes_Aléatoires_MCMC-20%3A06.pdf)


## Mise en route

```bash
brew install graphviz
make
```

| Path | Rôle / commande rapide |
|------|------------------------|
| `main.c` | point d’entrée ; parse les options puis lance la chaîne de Markov. |
| `analyse.c` / `analyse.h` | Cœur multithread du sampler : crée la structure Graph, génère edge_list, lance un pool de NB_THREADS (MarkovThread) qui exécute kswapAndUpdate. Gère le comptage incrémental de triangles (updateTriangles) et l’enregistrement périodique des mesures (recordMeasuresThreads). Paramètres clés : SAMPLING_GAP, MAX_STEPS. |
| `graphes.c` / `graphes.h` | Bibliothèque cœur : structure de graphe (liste d’adjacence), création/duplication, ajout/suppression d’arêtes, I/O (load/save, rendu Graphviz), générateurs aléatoires (Erdős–Rényi, configuration‑model), primitives d’échange (swap/kswap) et utilitaires (degrés, demi‑arêtes, shuffle, Zipf). |
| `Pcolors.c` / `Pcolors.h` | helpers ANSI pour logs colorés. |
| `data/`                   | graphes d’exemple (edge list) et modèles de config. |
| `plot.py`                | tracer les courbes et histogrammes du rapport |
| `save_to_chains.py`       | convertir des logs bruts en chaînes compactes (HDF5). |
| `stationarity.py`         | tests de Kolmogorov-Smirnov sur les chaînes. |
