#ifndef MEGIDDO_H
#define MEGIDDO_H

#include "constraint.h"
#include "array.h"

typedef struct solution {
    enum {unknown, feasible, infeasible} feasibility;
    point optimum;
} solution;

solution optimize(line objective, array constraints);

#endif
