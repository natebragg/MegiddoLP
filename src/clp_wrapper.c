#include "clp_wrapper.h"

const char* Clp_Version()
{
    return 0;
}

int Clp_VersionMajor()
{
    return 0;
}

int Clp_VersionMinor()
{
    return 0;
}

int Clp_VersionRelease()
{
    return 0;
}


Clp_Simplex *Clp_newModel()
{
    return 0;
}

void Clp_deleteModel(Clp_Simplex *model)
{
}


int Clp_readMps(Clp_Simplex *model, const char *fn, int keep_names, int ignore_errors)
{
    return 0;
}

void Clp_addRows(Clp_Simplex *model, int number,
                 double *row_lower, double *row_upper, int *row_starts,
                 int *columns, double *elements)
{
}

void Clp_addColumns(Clp_Simplex *model, int number,
                    double *column_lower, double *column_upper,
                    double *objective, int *column_starts,
                    int *rows, double *elements)
{
}


void Clp_setOptimizationDirection(Clp_Simplex *model, double value)
{
}

void Clp_setLogLevel(Clp_Simplex *model, int value)
{
}


int Clp_initialSolve(Clp_Simplex *model)
{
    return 0;
}


int Clp_getNumRows(Clp_Simplex *model)
{
    return 0;
}

int Clp_getNumCols(Clp_Simplex *model)
{
    return 0;
}


int Clp_isAbandoned(Clp_Simplex *model)
{
    return 0;
}

int Clp_isProvenOptimal(Clp_Simplex *model)
{
    return 0;
}

int Clp_isProvenPrimalInfeasible(Clp_Simplex *model)
{
    return 0;
}

int Clp_isProvenDualInfeasible(Clp_Simplex *model)
{
    return 0;
}

int Clp_isPrimalObjectiveLimitReached(Clp_Simplex *model)
{
    return 0;
}

int Clp_isDualObjectiveLimitReached(Clp_Simplex *model)
{
    return 0;
}

int Clp_isIterationLimitReached(Clp_Simplex *model)
{
    return 0;
}


double *Clp_getRowActivity(Clp_Simplex *model)
{
    return 0;
}

double *Clp_getColSolution(Clp_Simplex *model)
{
    return 0;
}
