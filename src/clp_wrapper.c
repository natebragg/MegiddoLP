#include "clp_wrapper.h"
#include "array.h"

#include <stdlib.h>

const char* Clp_Version()
{
    return "0";
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

typedef struct Clp_Wrapper {
    enum {
        None = 0,
        Final = 1,
        Factorizations = 2,
        PlusABitMore = 3,
        Verbose = 4
    } log_level;
    enum {
        Maximize = -1,
        Ignore = 0,
        Minimize = 1
    } direction;
    enum {
        Unknown           = -1,
        Optimal           = 0,
        PrimalInfeasible  = 1,
        DualInfeasible    = 2,
        Stopped           = 3,
        Errors            = 4,
        UserStopped       = 5
    } status;
    array rows;
} Clp_Wrapper;

Clp_Simplex *Clp_newModel()
{
    Clp_Simplex *model = malloc(sizeof(Clp_Wrapper));
    ((Clp_Wrapper*)model)->status = Unknown;
    Clp_setLogLevel(model, Verbose);
    Clp_setOptimizationDirection(model, Maximize);
    ((Clp_Wrapper*)model)->rows = make_array(4, double*);
    return model;
}

void Clp_deleteModel(Clp_Simplex *model)
{
    iter i = make_iter(&((Clp_Wrapper*)model)->rows);
    double **row = NULL;
    for(row = cur(&i, double*); row; row = next(&i, double*)) {
        free(*row);
    }
    free_array(((Clp_Wrapper*)model)->rows);
    free(model);
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
    ((Clp_Wrapper*)model)->direction = value;
}

void Clp_setLogLevel(Clp_Simplex *model, int value)
{
    ((Clp_Wrapper*)model)->log_level = value;
}

int Clp_initialSolve(Clp_Simplex *model)
{
    return ((Clp_Wrapper*)model)->status;
}


int Clp_getNumRows(Clp_Simplex *model)
{
    return ((Clp_Wrapper*)model)->rows.length;
}

int Clp_getNumCols(Clp_Simplex *model)
{
    return 0;
}


int Clp_isAbandoned(Clp_Simplex *model)
{
    return ((Clp_Wrapper*)model)->status == Errors;
}

int Clp_isProvenOptimal(Clp_Simplex *model)
{
    return ((Clp_Wrapper*)model)->status == Optimal;
}

int Clp_isProvenPrimalInfeasible(Clp_Simplex *model)
{
    return ((Clp_Wrapper*)model)->status == PrimalInfeasible;
}

int Clp_isProvenDualInfeasible(Clp_Simplex *model)
{
    return ((Clp_Wrapper*)model)->status == DualInfeasible;
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
    return ((Clp_Wrapper*)model)->status == Stopped;
}


double *Clp_getRowActivity(Clp_Simplex *model)
{
    return 0;
}

double *Clp_getColSolution(Clp_Simplex *model)
{
    return 0;
}
