#include "geo.h"

#include <math.h>

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

line rotate_line(double theta, line f)
{
    double sin_theta = sin(theta);
    double cos_theta = cos(theta);
    point p1, q1, p2, q2;
    p1.x = 0;
    p1.y = apply(0, f);
    q1.x = 1;
    q1.y = apply(1, f);

    p2.x = p1.x * cos_theta + p1.y * (-sin_theta);
    p2.y = p1.x * sin_theta + p1.y *   cos_theta;
    q2.x = q1.x * cos_theta + q1.y * (-sin_theta);
    q2.y = q1.x * sin_theta + q1.y *   cos_theta;

    return from_points(p2, q2);
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
