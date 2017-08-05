#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "geo.h"

typedef struct constraint {
    enum {leq, geq} ord;
    line f;
} constraint;

typedef struct pair {
    constraint *c1;
    constraint *c2;
} pair;

constraint rotate_constraint(radians, constraint);

#endif
