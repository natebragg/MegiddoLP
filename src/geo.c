#include "geo.h"

#include <math.h>

const double pi = 3.14159265358979323846;

line make_positive(line f)
{
    double sign = f.a1 < 0 ? -1 : 1;
    f.a1 *= sign;
    f.a2 *= sign;
    f.b  *= sign;
    return f;
}

double apply(double x, line f)
{
     return (-1 * f.a2 * x + f.b)/f.a1;
}

line from_points(point p, point q)
{
    line f;
    f.a1 = p.x - q.x;
    f.a2 = q.y - p.y;
    f.b = f.a1 * p.y + f.a2 * p.x;
    return make_positive(f);
}

line perpendicular(line f)
{
    line g;
    g.a1 = f.a2;
    g.a2 = -f.a1;
    g.b = f.b;
    return make_positive(g);
}

line shift(line f, double delta)
{
    f.b += delta;
    return f;
}

radians angle_down(line f)
{
    radians theta;
    /* counterclockwise offset from 0 radians
       (right), the reference for atan, to down,
       the reference for Megiddo's algorithm. */
    const double offset = 3 * pi / 2;
    theta.theta = offset - fmod(pi + atan2(f.a1, -1 * f.a2), pi);
    return theta;
}

point rotate_point(radians theta, point p)
{
    double sin_theta = sin(theta.theta);
    double cos_theta = cos(theta.theta);
    point q;
    q.x = p.x * cos_theta + p.y * (-sin_theta);
    q.y = p.x * sin_theta + p.y *   cos_theta;
    return q;
}

line rotate_line(radians theta, line f)
{
    line orth1 = perpendicular(f);
    line orth2 = shift(orth1, 1);
    point p = rotate_point(theta, intersect(f, orth1));
    point q = rotate_point(theta, intersect(f, orth2));
    return from_points(p, q);
}

int parallel(line f, line g)
{
    return f.a2*g.a1 == g.a2*f.a1;
}

point intersect(line f, line g)
{
    point cross;
    double a = g.a1*f.b  - f.a1*g.b;
    double b = f.a2*g.b  - g.a2*f.b;
    double c = f.a2*g.a1 - g.a2*f.a1;

    cross.x = a/c;
    cross.y = b/c;
    return cross;
}
