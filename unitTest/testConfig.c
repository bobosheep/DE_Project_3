#include <stdio.h>
#include <stdlib.h>
#include "../rgrep/rgrep.h"

int main(int argc, char ** argv)
{
    CONFIG * config = malloc(sizeof(CONFIG));
    getConfig(argc, argv, config);
    showConfig(config);

    return 0;
}