#include <stdio.h>

#define assert(pred) \
    do {if (!(pred)) { \
        printf("%s:%d: failed %s\n", __FILE__, __LINE__, #pred); \
         return 1; \
    }} while(0)
