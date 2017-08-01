#include "megiddo.h"

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
        array xs = map(pair_intersect_x, pairs, double);
        free_array(&xs);
        free_array(&parallels);
        free_array(&pairs);
        set = set == &downwards ? &upwards : &downwards;
    }

    free_array(&downwards);
    free_array(&upwards);
    return result;
}
