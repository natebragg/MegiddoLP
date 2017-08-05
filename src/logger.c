#include "logger.h"

#include <stdio.h>
#include <stdarg.h>
#include <math.h>

static void print_line(line f, const char *eq)
{
    printf("%f*y %s %f*x %s %f", f.a1, f.a2 < 0 ? "-" : "+", fabs(f.a2), eq, f.b);
}

static void print_constraint(const constraint *c)
{
    print_line(c->f, c->ord == leq ? "<=" : ">=");
}

void log_position(const logger *l, const char *fmt, ...)
{
    if(l->level >= position) {
        va_list ap; 
        va_start(ap, fmt);
        vprintf(fmt, ap);
        va_end(ap);
        printf("\n");
    }
}

void log_result(const logger *l, const char *fmt, ...)
{
    if(l->level >= result) {
        va_list ap; 
        va_start(ap, fmt);
        vprintf(fmt, ap);
        va_end(ap);
        printf("\n");
    }
}

void log_line(const logger *l, line f, const char *heading, ...)
{
    if(l->level >= equations) {
        va_list ap; 
        va_start(ap, heading);
        vprintf(heading, ap);
        va_end(ap);
        print_line(f, "=");
        printf("\n");
    }
}

void log_constraint(const logger *l, const constraint *c, const char *heading, ...)
{
    if(l->level >= equations) {
        va_list ap; 
        va_start(ap, heading);
        vprintf(heading, ap);
        va_end(ap);
        print_constraint(c);
        printf("\n");
    }
}

void log_constraint_array(const logger *l, array cs, const char *heading, ...)
{
    if(l->level >= equations) {
        iter i = make_iter(&cs);
        constraint *c = NULL;
        va_list ap; 
        va_start(ap, heading);
        vprintf(heading, ap);
        printf("\n");
        va_end(ap);
        for(c = cur(i, constraint); c; c = next(&i, constraint)) {
            log_constraint(l, c, "\t");
        }
    }
}

void log_pair_array(const logger *l, array ps, const char *heading, ...)
{
    if(l->level >= equations) {
        iter i = make_iter(&ps);
        pair *p = NULL;
        va_list ap; 
        va_start(ap, heading);
        vprintf(heading, ap);
        printf("\n");
        for(p = cur(i, pair); p; p = next(&i, pair)) {
            printf("\t");
            print_constraint(p->c1);
            printf("\t");
            print_constraint(p->c2);
            printf("\n");
        }
    }
}

void log_double_array(const logger *l, array ds, const char *heading, ...)
{
    if(l->level >= position) {
        iter i = make_iter(&ds);
        double *d = NULL;
        va_list ap; 
        va_start(ap, heading);
        vprintf(heading, ap);
        va_end(ap);
        for(d = cur(i, double); d; d = next(&i, double)) {
            printf("%f\t", *d);
        }
        printf("\n");
    }
}

void log_double_ptr_array(const logger *l, array ds, const char *heading, ...)
{
    if(l->level >= position) {
        iter i = make_iter(&ds);
        double **d = NULL;
        va_list ap; 
        va_start(ap, heading);
        vprintf(heading, ap);
        va_end(ap);
        for(d = cur(i, double*); d; d = next(&i, double*)) {
            printf("%f\t", **d);
        }
        printf("\n");
    }
}
