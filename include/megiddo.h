#ifndef MEGIDDO_H
#define MEGIDDO_H

#include "constraint.h"
#include "array.h"
#include "logger.h"

typedef struct solution {
    enum {unknown, feasible, infeasible} feasibility;
    point optimum;
} solution;

solution optimize(const logger *, point objective, array constraints);

#endif
