typedef void Clp_Simplex;

const char* Clp_Version();
int Clp_VersionMajor();
int Clp_VersionMinor();
int Clp_VersionRelease();

Clp_Simplex *Clp_newModel();
void Clp_deleteModel(Clp_Simplex *);

int Clp_readMps(Clp_Simplex *, const char *, int, int);
void Clp_addRows(Clp_Simplex *, int, double *, double *, int *, int *, double *);
void Clp_addColumns(Clp_Simplex *, int, double *, double *, double *, int *, int *, double *);

void Clp_setOptimizationDirection(Clp_Simplex *, double);
void Clp_setLogLevel(Clp_Simplex *, int);

int Clp_initialSolve(Clp_Simplex *);

int Clp_getNumRows(Clp_Simplex *);
int Clp_getNumCols(Clp_Simplex *);

int Clp_isAbandoned(Clp_Simplex *);
int Clp_isProvenOptimal(Clp_Simplex *);
int Clp_isProvenPrimalInfeasible(Clp_Simplex *);
int Clp_isProvenDualInfeasible(Clp_Simplex *);
int Clp_isPrimalObjectiveLimitReached(Clp_Simplex *);
int Clp_isDualObjectiveLimitReached(Clp_Simplex *);
int Clp_isIterationLimitReached(Clp_Simplex *);

double *Clp_getRowActivity(Clp_Simplex *);
double *Clp_getColSolution(Clp_Simplex *);
