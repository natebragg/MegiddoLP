#include "megiddo.h"
#include "median.h"
#include "logger.h"

#include <stdlib.h>

static int is_pointing_up(const constraint *c)
{
    return c->ord == geq;
}

static int is_not_parallel(const pair *p)
{
    return !parallel(p->c1->f, p->c2->f);
}

static void pair_inner(const pair *p, constraint **c)
{
    assert(p->c1->ord == p->c2->ord);
    *c = (p->c1->ord == leq) ?
            (p->c1->f.b < p->c2->f.b ? p->c1 : p->c2) :
            (p->c1->f.b > p->c2->f.b ? p->c1 : p->c2);
}

static void pair_intersect_x(const pair *p, double *x)
{
    *x = intersect(p->c1->f, p->c2->f).x;
}

static array make_pairs(array a, array *leftover)
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
    if(c1) {
        *grow(leftover, constraint*) = c1;
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

static void has_point(const point *p, array *accum, const constraint *c)
{
    double b_if_has_point = c->f.a1 * p->y + c->f.a2 * p->x;
    if(b_if_has_point == c->f.b) {
        *grow(accum, constraint) = *c;
    }
}

static void slope_max_at(double *acc, const constraint *c)
{
    double slope = -1 * c->f.a2 / c->f.a1;
    *acc = *acc < slope ? slope : *acc;
}

static void slope_min_at(double *acc, const constraint *c)
{
    double slope = -1 * c->f.a2 / c->f.a1;
    *acc = *acc > slope ? slope : *acc;
}

typedef enum direction {
    left_of_median,
    right_of_median
} direction;

typedef struct quadrant_filter {
    double median;
    direction opt_dir;
    enum {above, below} outer_is;
} quadrant_filter;

static int in_dir(double a, direction dir, double median)
{
    return (dir == left_of_median) ? a < median : a > median;
}

static void discard_outer(const quadrant_filter *qf, array *acc, const pair *p)
{
    if(in_dir(intersect(p->c1->f, p->c2->f).x, qf->opt_dir, qf->median)) {
        *grow(acc, constraint) = *p->c1;
        *grow(acc, constraint) = *p->c2;
    } else {
        double delta = (qf->opt_dir == left_of_median) ? -1 : 1;
        double opt_side_y1 = apply(qf->median + delta, p->c1->f);
        double opt_side_y2 = apply(qf->median + delta, p->c2->f);
        *grow(acc, constraint) = (qf->outer_is == below ?
                                    (opt_side_y1 > opt_side_y2) :
                                    (opt_side_y1 < opt_side_y2)) ? *p->c1 : *p->c2;
    }
}

static void append(array *acc, const constraint **c)
{
    *grow(acc, constraint) = **c;
}

solution optimize(const logger *l, point objective, array constraints)
{
    solution result = {unknown, {0, 0}};
    radians theta = angle_down(objective);
    array upwards, downwards, *set;
    iter i = make_iter(&constraints);
    constraint *c = NULL;
    log_constraint_array(l, constraints, "unrotated:");
    for(c = cur(i, constraint); c; c = next(&i, constraint)) {
        *c = rotate_constraint(theta, *c);
    }
    log_constraint_array(l, constraints, "rotated by %f:", theta.theta);
    downwards = partition(is_pointing_up, &constraints);
    upwards = clone(&constraints);
    set = &downwards;
    while(result.feasibility == unknown) {
        array leftover = make_array(1, constraint*);
        array pairs = make_pairs(*set, &leftover);
        array parallels = partition(is_not_parallel, &pairs);
        if(pairs.length > 0) {
            array xs = map(pair_intersect_x, pairs, double);
            double median = median = find_median(&xs);
            array upys = map1(apply_median, &median, upwards, double);
            array dnys = map1(apply_median, &median, downwards, double);
            double *max_tmp = NULL, *min_tmp = NULL;
            size_t max_convex = index_of(upys, *foldl(idx_max, &max_tmp, upys, double*));
            size_t min_concave = index_of(dnys, *foldl(idx_min, &min_tmp, dnys, double*));
            line max_convex_c = index(upwards, max_convex, constraint)->f;
            line min_concave_c = index(downwards, min_concave, constraint)->f;
            double max_convex_y = *index(upys, max_convex, double);
            double min_concave_y = *index(dnys, min_concave, double);
            log_pair_array(l, pairs, "set is %s:", set == &downwards ? "downwards" : "upwards");
            log_constraint_array(l, upwards, "upwards[%d]:", upwards.length);
            log_constraint_array(l, downwards, "downwards[%d]:", downwards.length);
            log_double_array(l, xs, "xs:\t");
            log_position(l, "median:\t%f", median);
            log_double_array(l, upys, "upys:\t");
            log_double_array(l, dnys, "dnys:\t");
            if(max_convex_y > min_concave_y && parallel(max_convex_c, min_concave_c)) {
                log_line(l, max_convex_c,  "infeasible: [%d] ", max_convex);
                log_line(l, min_concave_c, "            [%d] ", min_concave);
                result.feasibility = infeasible;
            } else {
                direction opt_dir;
                point where = intersect(max_convex_c, min_concave_c);
                point max_convex_at_median;
                max_convex_at_median.x = median;
                max_convex_at_median.y = max_convex_y;
                if(max_convex_y > min_concave_y) {
                    /* the optimum lies in the direction of their intersection */
                    log_position(l, "not in feasible region");
                    opt_dir = median < where.x ? right_of_median : left_of_median;
                } else {
                    /* This x-coordinate is *a* feasible solution (not necessarily the optimum) */
                    array cross_values = make_array(1, constraint);
                    double s_tmp = 1.0/0.0, S_tmp = -1.0/0.0;
                    double s_g = 0.0, S_g = 0.0;
                    foldl1(has_point, &max_convex_at_median, &cross_values, upwards, array);
                    s_g = *foldl(slope_min_at, &s_tmp, cross_values, double);
                    S_g = *foldl(slope_max_at, &S_tmp, cross_values, double);
                    log_constraint_array(l, cross_values, "cross_values:\t");
                    log_position(l, "IN feasible region; s_g=%f, S_g=%f", s_g, S_g);
                    if(s_g <= 0 && 0 <= S_g) {
                        log_position(l, "optimum reached");
                        result.feasibility = feasible;
                        result.optimum = max_convex_at_median;
                    } else if(max_convex_y < min_concave_y) {
                        /* and the optimum lies in the opposite direction of their intersection. */
                        log_position(l, "feasible but not optimal");
                        opt_dir = where.x < median ? right_of_median : left_of_median;
                    } else {
                        /* the optimum lies to the side where min_concave_c > max_convex_c */
                        double right_concave_y = apply(median + 1, min_concave_c);
                        double right_convex_y = apply(median + 1, max_convex_c);
                        log_position(l, "feasible but in a corner");
                        opt_dir = right_convex_y < right_concave_y ? right_of_median : left_of_median;
                    }
                    free_array(&cross_values);
                }

                if(result.feasibility == unknown) {
                    array inners = map(pair_inner, parallels, constraint*);
                    array culled = make_array(set->length, constraint);
                    quadrant_filter f;
                    f.median = median;
                    f.opt_dir = opt_dir;
                    f.outer_is = set == &downwards ? above : below;
                    foldl1(discard_outer, &f, &culled, pairs, array);
                    foldl(append, &culled, leftover, array);
                    foldl(append, &culled, inners, array);
                    free_array(&inners);
                    free_array(set);
                    *set = culled;
                }
            }
            free_array(&dnys);
            free_array(&upys);
            free_array(&xs);
        } else {
            array inners = map(pair_inner, parallels, constraint*);
            free_array(set);
            *set = make_array(inners.length, constraint);
            foldl(append, set, inners, array);
            free_array(&inners);
        }
        free_array(&parallels);
        free_array(&pairs);
        free_array(&leftover);
        if(result.feasibility == unknown) {
            array *alt = set == &downwards ? &upwards : &downwards;
            set = (alt->length > 1) ? alt : set;
            if (upwards.length == 1 && downwards.length == 1) {
                constraint u = *index(upwards, 0, constraint);
                constraint d = *index(downwards, 0, constraint);
                result.feasibility = feasible;
                result.optimum = intersect(u.f, d.f);
            }
        }
    }

    free_array(&downwards);
    free_array(&upwards);
    theta.theta *= -1;
    result.optimum = rotate_point(theta, result.optimum);
    log_result(l, "%f %% %f", result.optimum.x, result.optimum.y);
    return result;
}
