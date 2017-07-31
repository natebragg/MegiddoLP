#include "geo.h"

typedef struct standard_constraint {
    double a1;
    double a2;
    /* all standard constraints are leq */
    double b;
} standard_constraint;

typedef struct constraint {
    enum {leq, geq} ord;
    line f;
} constraint;

constraint from_standard(standard_constraint);
constraint rotate_constraint(double, constraint);
