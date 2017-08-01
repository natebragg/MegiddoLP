#ifndef MEGIDDO_H
#define MEGIDDO_H

#include "constraint.h"
#include "array.h"

double find_median(array *);
point optimize(line objective, array constraints);

#endif
