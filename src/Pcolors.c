#include <stdio.h>
#include "Pcolors.h"

void Pblack(void) {
    printf("\033[1;30m");
}

void Pred(void) {
    printf("\033[1;31m");
}

void Pgreen(void) {
    printf("\033[1;32m");
}

void Pyellow(void) {
    printf("\033[1;33m");
}

void Pblue(void) {
    printf("\033[1;34m");
}

void Ppurple(void) {
    printf("\033[1;35m");
}

void Pcyan(void) {
    printf("\033[1;36m");
}

void Pwhite(void) {
    printf("\033[1;37m");
}

void Preset(void) {
    printf("\033[0m");
}
