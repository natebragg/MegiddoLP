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

static void equal_to(const double *lhs, array *accum, const double *rhs)
{
    if(*lhs == *rhs) {
        *grow(accum, const double*) = rhs;
    }
}

static void slope_max_at(const array *upwards, double **acc, const size_t *idx)
{
    double *slope = &index(*upwards, *idx, constraint)->f.a;
    *acc = (*acc == NULL || **acc < *slope) ? slope : *acc;
}

static void slope_min_at(const array *upwards, double **acc, const size_t *idx)
{
    double *slope = &index(*upwards, *idx, constraint)->f.a;
    *acc = (*acc == NULL || **acc > *slope) ? slope : *acc;
}

static void index_of_(const array *a, const double **value, size_t *out)
{
    *out = index_of(*a, *value);
}

solution optimize(line objective, array constraints)
{
    solution result = {unknown, {0, 0}};
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
    while(result.feasibility == unknown) {
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
            result.feasibility = infeasible;
        } else {
            enum {left_of_median, right_of_median} direction;
            point where = intersect(max_convex_c, min_concave_c);
            double max_convex_y = *index(upys, max_convex, double);
            double min_concave_y = *index(dnys, min_concave, double);
            if(max_convex_y > min_concave_y) {
                /* the optimum lies in the direction of their intersection */
                direction = median < where.x ? right_of_median : left_of_median;
            } else {
                /* This x-coordinate is *a* feasible solution (not necessarily the optimum) */
                array cross_values = make_array(1, double*);
                array cross_indexes = map1(index_of_, &upys, *foldl1(equal_to, &max_convex_y, &cross_values, upys, array), size_t);
                double *s_tmp = NULL, *S_tmp = NULL;
                double s_g = **foldl1(slope_min_at, &upwards, &s_tmp, cross_indexes, double*);
                double S_g = **foldl1(slope_max_at, &upwards, &S_tmp, cross_indexes, double*);
                if(s_g <= 0 && 0 <= S_g) {
                    result.feasibility = feasible;
                    result.optimum.x = median;
                    result.optimum.y = max_convex_y;
                } else if(max_convex_y < min_concave_y) {
                    /* and the optimum lies in the opposite direction of their intersection. */
                    direction = where.x < median ? right_of_median : left_of_median;
                } else {
                    /* the optimum lies to the side where min_concave_c > max_convex_c */
                    double right_concave_y = apply(median + 1, min_concave_c);
                    double right_convex_y = apply(median + 1, max_convex_c);
                    direction = right_convex_y < right_concave_y ? right_of_median : left_of_median;
                }
                free_array(&cross_indexes);
                free_array(&cross_values);
            }

            if(result.feasibility == unknown) {
                if(direction == left_of_median) {
                } else {
                }
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
