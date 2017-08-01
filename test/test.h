#ifndef TEST_H
#define TEST_H

#include <stdio.h>

#define assert_pred(pred) \
    do {if (!(pred)) { \
        printf("%s:%d: failed %s\n", __FILE__, __LINE__, #pred); \
         return 1; \
    }} while(0)

#define assert_within(a, b, epsilon) \
    assert_pred(fabs(a - b) < epsilon)

#define assert_eq(a, b) \
    assert_pred(a == b)

#endif
