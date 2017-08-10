#include "clp_wrapper.h"
#include "megiddo.h"
#include "constraint.h"
#include "array.h"
#include "logger.h"

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
    array rows_upper;
    array rows_lower;
    array cols_upper;
    array cols_lower;
    array row_activity;
    array col_solution;
    point objective;
} Clp_Wrapper;

Clp_Simplex *Clp_newModel()
{
    Clp_Wrapper *model = malloc(sizeof(Clp_Wrapper));
    model->status = Unknown;
    Clp_setLogLevel(model, Verbose);
    Clp_setOptimizationDirection(model, Maximize);
    model->rows_upper = make_array(4, constraint);
    model->rows_lower = make_array(4, constraint);
    model->cols_upper = make_array(2, constraint);
    model->cols_lower = make_array(2, constraint);
    model->row_activity = make_array(4, double);
    model->col_solution = make_array(2, double);
    model->objective.y = 0;
    model->objective.x = 0;
    return model;
}

void Clp_deleteModel(Clp_Simplex *model)
{
    Clp_Wrapper *m = model;
    free_array(&m->rows_upper);
    free_array(&m->rows_lower);
    free_array(&m->cols_upper);
    free_array(&m->cols_lower);
    free_array(&m->row_activity);
    free_array(&m->col_solution);
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
    Clp_Wrapper *m = model;
    int i = 0;
    for(; i < number; ++i) {
        double as[2] = {0, 0};
        if(row_starts != NULL && columns != NULL && elements != NULL) {
            int j = 0;
            for(j = row_starts[i]; j < row_starts[i+1]; ++j) {
                if(columns[j] >= 0 && columns[j] <= 1) {
                    as[columns[j]] = elements[j];
                }
            }
        }
        {
            constraint *row = grow(&m->rows_upper, constraint);
            row->f.a1 = as[0];
            row->f.a2 = as[1];
            row->ord  = leq;
            row->f.b  = row_upper[i];
        }
        {
            constraint *row = grow(&m->rows_lower, constraint);
            row->f.a1 = -as[0];
            row->f.a2 = -as[1];
            row->ord  = leq;
            row->f.b  = -row_lower[i];
        }
    }
}

void Clp_addColumns(Clp_Simplex *model, int number,
                    double *column_lower, double *column_upper,
                    double *objective, int *column_starts,
                    int *rows, double *elements)
{
    Clp_Wrapper *m = model;
    int i = 0;
    unsigned long col_len = m->cols_upper.length;
    assert(number + col_len <= 2); /* Can only handle 2D LP. */
    for(; i < number; ++i) {
        if(column_starts != NULL && rows != NULL && elements != NULL) {
            int j = 0;
            for(j = column_starts[i]; j < column_starts[i+1]; ++j) {
                if(rows[j] >= 0 && rows[j] <= m->rows_upper.length) {
                    constraint *row = index(m->rows_upper, rows[j], constraint);
                    *(col_len + i == 0 ? &row->f.a1 : &row->f.a2) = elements[j];
                    row = index(m->rows_lower, rows[j], constraint);
                    *(col_len + i == 0 ? &row->f.a1 : &row->f.a2) = -elements[j];
                }
            }
        }
        *(col_len + i == 0 ? &m->objective.y : &m->objective.x) = objective[i];
        {
            constraint *col = grow(&m->cols_upper, constraint);
            *(col_len + i == 0 ? &col->f.a1 : &col->f.a2) = 1.0;
            col->ord  = leq;
            col->f.b  = column_upper[i];
        }
        {
            constraint *col = grow(&m->cols_lower, constraint);
            *(col_len + i == 0 ? &col->f.a1 : &col->f.a2) = -1.0;
            col->ord  = leq;
            col->f.b  = column_lower[i];
        }
    }
}

void Clp_setOptimizationDirection(Clp_Simplex *model, double value)
{
    ((Clp_Wrapper*)model)->direction = value;
}

void Clp_setLogLevel(Clp_Simplex *model, int value)
{
    ((Clp_Wrapper*)model)->log_level = value;
}

static logger make_logger(Clp_Wrapper *m)
{
    logger l;
    switch(m->log_level) {
    case Final:
    case Factorizations: l.level = result; break;
    case PlusABitMore:   l.level = position; break;
    case Verbose:        l.level = equations; break;
    case None:
    default:             l.level = off; break;
    }
    return l;
}

int Clp_initialSolve(Clp_Simplex *model)
{
    Clp_Wrapper *m = model;
    logger l = make_logger(m);
    solution optimum;
    array cs = make_array(m->rows_upper.length + m->rows_lower.length +
                          m->cols_upper.length + m->cols_lower.length, constraint);
    array* scs[4];
    int j = 0;
    scs[0] = &m->rows_upper;
    scs[1] = &m->rows_lower;
    scs[2] = &m->cols_upper;
    scs[3] = &m->cols_lower;
    for(; j < 4; ++j) {
        iter i = make_iter(scs[j]);
        constraint *c = NULL;
        log_constraint_array(&l, *scs[j], "rows[%d]:", j);
        for(c = cur(i, constraint); c; c = next(&i, constraint)) {
            *grow(&cs, constraint) = *c;
        }
    }

    log_position(&l, "objective: {%f, %f}", m->objective.x, m->objective.y);
    optimum = optimize(&l, m->objective, cs);
    free_array(&cs);

    while(m->col_solution.length < m->cols_upper.length) {
        grow(&m->col_solution, double);
    }
    if(m->cols_upper.length > 0) {
        *index(m->col_solution, 0, double) = optimum.optimum.y;
    }
    if(m->cols_upper.length > 1) {
        *index(m->col_solution, 1, double) = optimum.optimum.x;
    }
    while(m->row_activity.length < m->rows_upper.length) {
        grow(&m->row_activity, double);
    }
    for(j = 0; j < m->rows_upper.length; ++j) {
        constraint *c = index(m->rows_upper, j, constraint);
        *index(m->row_activity, j, double) = c->f.a1 * optimum.optimum.y + c->f.a2 * optimum.optimum.x;
    }
    switch(optimum.feasibility) {
    case unknown:    m->status = Unknown; break;
    case feasible:   m->status = Optimal; break;
    case infeasible: m->status = PrimalInfeasible; break;
    default:          m->status = Errors; break;
    }

    return m->status;
}


int Clp_getNumRows(Clp_Simplex *model)
{
    return ((Clp_Wrapper*)model)->rows_upper.length;
}

int Clp_getNumCols(Clp_Simplex *model)
{
    return ((Clp_Wrapper*)model)->cols_upper.length;
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
    return ((Clp_Wrapper*)model)->row_activity.start;
}

double *Clp_getColSolution(Clp_Simplex *model)
{
    return ((Clp_Wrapper*)model)->col_solution.start;
}
