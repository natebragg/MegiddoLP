#include "constraint.h"

static int sign(double v)
{
    return (v >= 0) * 2 - 1;
}

constraint rotate_constraint(radians theta, constraint c)
{
    line shifted = shift(c.f, 1.5 - sign(c.f.b), c.ord == leq ? -1 : 1);
    line rotation = rotate_line(theta, c.f);
    line offset = rotate_line(theta, shifted);
    line orth = perpendicular(rotation);
    point p_r = intersect(rotation, orth);
    point p_o = intersect(offset, orth);
    c.f = rotation;
    c.ord = p_o.y < p_r.y ? leq :
            p_o.y > p_r.y ? geq :
            p_o.x < p_r.x ? leq : geq;
    return c;
}
