#include "constraint.h"

constraint from_standard(standard_constraint s)
{
    constraint c;
    c.ord = s.a1 >= 0 ? leq : geq;
    c.f.a = -s.a2 / s.a1;
    c.f.b = s.b / s.a1;
    return c;
}

constraint rotate_constraint(double theta, constraint c)
{
    constraint c_rot;
    point p, q;
    p.x = 0;
    p.y = apply(p.x, c.f) + (c.ord == leq ? -1 : 1);
    q = rotate_point(theta, p);
    c_rot.f = rotate_line(theta, c.f);
    c_rot.ord = apply(q.x, c_rot.f) > q.y ? leq : geq; 
    return c_rot;
}
