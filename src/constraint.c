#include "constraint.h"

constraint from_standard(standard_constraint s)
{
    constraint c;
    c.ord = leq;
    c.f.a1 = s.a1;
    c.f.a2 = s.a2;
    c.f.b = s.b;
    return c;
}

constraint rotate_constraint(radians theta, constraint c)
{
    line rotation = rotate_line(theta, c.f);
    line offset = rotate_line(theta, shift(c.f, 0.5, c.ord == leq ? -1 : 1));
    line orth = perpendicular(rotation);
    point p_r = intersect(rotation, orth);
    point p_o = intersect(offset, orth);
    c.f = rotation;
    c.ord = p_o.y < p_r.y ? leq :
            p_o.y > p_r.y ? geq :
            p_o.x < p_r.x ? leq : geq;
    return c;
}
