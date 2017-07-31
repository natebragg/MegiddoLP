#include "geo.h"

#include <math.h>

const double pi = 3.14159265358979323846;

double apply(double x, line f)
{
    return f.a*x+f.b;
}

line from_points(point p, point q)
{
    line f;
    f.a = (p.y - q.y) / (p.x - q.x);
    f.b = p.y - f.a * p.x;
    return f;
}

radians angle_down(line f)
{
    radians theta;
    /* counterclockwise offset from 0 radians
       (right), the reference for atan, to down,
       the reference for Megiddo's algorithm. */
    const double offset = 3 * pi / 2;
    theta.theta = offset - fmod(pi + atan(f.a), pi);
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
    point p, q;
    p.x = 0;
    p.y = apply(0, f);
    q.x = 1;
    q.y = apply(1, f);

    return from_points(rotate_point(theta, p), rotate_point(theta, q));
}

int parallel(line f, line g)
{
    return f.a == g.a;
}

point intersect(line f, line g)
{
    point cross;
    cross.x = (g.b - f.b) / (f.a - g.a);
    cross.y = apply(cross.x, f);
    return cross;
}
