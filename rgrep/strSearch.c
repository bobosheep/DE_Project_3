#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "rgrep.h"
#include "../strmat/strmat.h"

#define BM 0
#define SHIFTOR 0
#define SHIFTAND 0

char * strSearch(const sConfig * s_config)
{ 
#if BM
    return boyerMoore(s_config->target, s_config->pattern);

#elif SHIFTOR == 1

#elif SHIFTAND == 1

#else
    return strstr(s_config->target, s_config->pattern);

#endif
}
