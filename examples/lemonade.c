#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "clp_wrapper.h"

#define num_rows 5

void input_by_columns(Clp_Simplex *model)
{
    {
        /*                           Time Lemons  Sugar  Water Vodka */
        double rowLower[num_rows] = { 0.0,   0.0,   0.0,   0.0,  0.0};
        double rowUpper[num_rows] = {60.0, 200.0, 250.0, 240.0, 50.0};
        Clp_addRows(model, num_rows, rowLower, rowUpper, NULL, NULL, NULL);
    }

    /* Regular */
    {
        double columnLower = 0.0;
        double columnUpper = DBL_MAX;
        double objective = 1.0;
        int columnStarts[2] = {0, num_rows};
        int rows[num_rows] = {0, 1, 2, 3, 4};
        double elements[num_rows] = {0.25, 1.0, 2.0, 2.0, 0.0};
        Clp_addColumns(model, 1, &columnLower, &columnUpper, &objective, columnStarts, rows, elements);
    }

    /* Special */
    {
        double columnLower = 0.0;
        double columnUpper = DBL_MAX;
        double objective = 2.0;
        int columnStarts[2] = {0, num_rows};
        int rows[num_rows] = {0, 1, 2, 3, 4};
        double elements[num_rows] = {0.5, 1.0, 1.25, 0.6, 0.5};
        Clp_addColumns(model, 1, &columnLower, &columnUpper, &objective, columnStarts, rows, elements);
    }
}

#define num_cols 2

void input_by_rows(Clp_Simplex *model)
{
    {
        /*                              Regular  Special */
        double columnLower[num_cols] = {    0.0,     0.0};
        double columnUpper[num_cols] = {DBL_MAX, DBL_MAX};
        double objective[num_cols]   = {    1.0,     2.0};
        Clp_addColumns(model, num_cols, columnLower, columnUpper, objective, NULL, NULL, NULL);
    }

    /* Time */
    {
        double rowLower = 0.0;
        double rowUpper = 60.0;
        int rowStarts[2] = {0, num_cols};
        int columns[num_cols] = {0, 1};
        double elements[num_cols] = {0.25, 0.5};
        Clp_addRows(model, 1, &rowLower, &rowUpper, rowStarts, columns, elements);
    }

    /* Lemons */
    {
        double rowLower = 0.0;
        double rowUpper = 200.0;
        int rowStarts[2] = {0, num_cols};
        int columns[num_cols] = {0, 1};
        double elements[num_cols] = {1.0, 1.0};
        Clp_addRows(model, 1, &rowLower, &rowUpper, rowStarts, columns, elements);
    }

    /* Sugar */
    {
        double rowLower = 0.0;
        double rowUpper = 250.0;
        int rowStarts[2] = {0, num_cols};
        int columns[num_cols] = {0, 1};
        double elements[num_cols] = {2.0, 1.25};
        Clp_addRows(model, 1, &rowLower, &rowUpper, rowStarts, columns, elements);
    }

    /* Water */
    {
        double rowLower = 0.0;
        double rowUpper = 240.0;
        int rowStarts[2] = {0, num_cols};
        int columns[num_cols] = {0, 1};
        double elements[num_cols] = {2.0, 0.6};
        Clp_addRows(model, 1, &rowLower, &rowUpper, rowStarts, columns, elements);
    }

    /* Vodka */
    {
        double rowLower = 0.0;
        double rowUpper = 50.0;
        int rowStarts[2] = {0, num_cols};
        int columns[num_cols] = {0, 1};
        double elements[num_cols] = {0.0, 0.5};
        Clp_addRows(model, 1, &rowLower, &rowUpper, rowStarts, columns, elements);
    }
}

void input_by_file(Clp_Simplex *model)
{
    int status = Clp_readMps(model, "lemonade.mps", 1, 0);
    if (status) {
       Clp_deleteModel(model);
       exit(status);
    }
}

int main()
{
    int status = 0, nr = 0, row = 0, nc = 0, col = 0;
    double *pr = NULL, *pc = NULL;
    Clp_Simplex *model = Clp_newModel();
    Clp_setLogLevel(model, 0);

    input_by_rows(model);

    Clp_setOptimizationDirection(model, -1);

    status = Clp_initialSolve(model);
    if (status) {
        Clp_deleteModel(model);
        return status;
    }

    printf("Solution: opt %d, ppi %d, pdi %d, plr %d, dlr %d, ilr %d, abn %d\n",
        Clp_isProvenOptimal(model),
        Clp_isProvenPrimalInfeasible(model),
        Clp_isProvenDualInfeasible(model),
        Clp_isPrimalObjectiveLimitReached(model),
        Clp_isDualObjectiveLimitReached(model),
        Clp_isIterationLimitReached(model),
        Clp_isAbandoned(model));

    pr = Clp_getRowActivity(model);
    nr = Clp_getNumRows(model);
    for (row = 0; row < nr; ++row) {
        printf("row %d, value %f\n", row, pr[row]);
    }

    pc = Clp_getColSolution(model);
    nc = Clp_getNumCols(model);
    for (col = 0; col < nc; ++col) {
        printf("col %d, solution %f\n", col, pc[col]);
    }

    Clp_deleteModel(model);
    return 0;
}
