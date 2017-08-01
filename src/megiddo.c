#include "megiddo.h"

static int is_pointing_up(const constraint *c)
{
    return c->ord == geq;
}

point optimize(line objective, array constraints)
{
    point result = {0, 0};
    radians theta = angle_down(objective);
    array upwards, downwards, *set;
    iter i = make_iter(&constraints);
    constraint *c = NULL;
    for(c = cur(&i, constraint); c; c = next(&i, constraint)) {
        *c = rotate_constraint(theta, *c);
    }

    downwards = partition(is_pointing_up, &constraints);
    upwards = clone(&constraints);
    set = &downwards;
    while(1) {
        set = set == &downwards ? &upwards : &downwards;
    }

    free_array(&downwards);
    free_array(&upwards);
    return result;
}
