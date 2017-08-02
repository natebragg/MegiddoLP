#include "megiddo.h"
#include "median.h"

#include <stdlib.h>

typedef struct pair {
    constraint *c1;
    constraint *c2;
} pair;

static int is_pointing_up(const constraint *c)
{
    return c->ord == geq;
}

static int is_not_parallel(const pair *p)
{
    return !parallel(p->c1->f, p->c2->f);
}

static void pair_outer(const pair *p, constraint **c)
{
    assert(p->c1->ord == p->c2->ord);
    *c = (p->c1->ord == leq) ?
            (p->c1->f.b > p->c2->f.b ? p->c1 : p->c2) :
            (p->c1->f.b < p->c2->f.b ? p->c1 : p->c2);
}

static void pair_intersect_x(const pair *p, double *x)
{
    *x = intersect(p->c1->f, p->c2->f).x;
}

static array make_pairs(array a)
{
    array pairs = make_array(a.length, pair);
    iter i = make_iter(&a);
    constraint *c1 = NULL, *c2 = NULL;
    for(c1 = cur(i, constraint), c2 = next(&i, constraint); c1 && c2;
            c1 = next(&i, constraint), c2 = next(&i, constraint)) {
        pair *p = grow(&pairs, pair);
        p->c1 = c1;
        p->c2 = c2;
    }
    return pairs;
}

static void apply_median(const double *median, const constraint *c, double *y)
{
    *y = apply(*median, c->f);
}

static void idx_max(const double **acc, const double *v)
{
    *acc = (*acc == NULL || **acc < *v) ? v : *acc;
}

static void idx_min(const double **acc, const double *v)
{
    *acc = (*acc == NULL || **acc > *v) ? v : *acc;
}

point optimize(line objective, array constraints)
{
    point result = {0, 0};
    radians theta = angle_down(objective);
    array upwards, downwards, *set;
    iter i = make_iter(&constraints);
    constraint *c = NULL;
    for(c = cur(i, constraint); c; c = next(&i, constraint)) {
        *c = rotate_constraint(theta, *c);
    }

    downwards = partition(is_pointing_up, &constraints);
    upwards = clone(&constraints);
    set = &downwards;
    while(1) {
        array pairs = make_pairs(*set);
        array parallels = partition(is_not_parallel, &pairs);
        array outers = map(pair_outer, parallels, constraint*);
        array xs = map(pair_intersect_x, pairs, double);
        double median = median = find_median(&xs);
        array upys = map1(apply_median, &median, upwards, double);
        array dnys = map1(apply_median, &median, downwards, double);
        double *max_tmp = NULL, *min_tmp = NULL;
        size_t max_convex = index_of(upys, *foldl(idx_max, &max_tmp, upys, double*));
        size_t min_concave = index_of(dnys, *foldl(idx_min, &min_tmp, dnys, double*));
        line max_convex_c = index(upwards, max_convex, constraint)->f;
        line min_concave_c = index(downwards, min_concave, constraint)->f;
        if(parallel(max_convex_c, min_concave_c)) {
        } else {
            double max_convex_y = *index(upys, max_convex, double);
            double min_concave_y = *index(dnys, min_concave, double);
            if(max_convex_y > min_concave_y) {
                /* the optimum lies in the direction of their intersection */
            } else if(max_convex_y < min_concave_y) {
                /* This x-coordinate is *a* feasible solution (not necessarily the optimum) */
                /* and the optimum lies in the opposite direction of their intersection. */
            } else {
                /* the optimum lies to the side where min_concave_c > max_convex_c */
            }
        }
        free_array(&dnys);
        free_array(&upys);
        free_array(&xs);
        free_array(&outers);
        free_array(&parallels);
        free_array(&pairs);
        set = set == &downwards ? &upwards : &downwards;
    }

    free_array(&downwards);
    free_array(&upwards);
    return result;
}
