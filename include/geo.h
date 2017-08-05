#ifndef GEO_H
#define GEO_H

typedef struct point {
    double x;
    double y;
} point;

/* a line in homogenous form:
    a1*y + a2*x - b = 0 */
typedef struct line {
    double a1;
    double a2;
    double b;
} line;

typedef struct radians {
    double theta;
} radians;

extern const double pi;

double apply(double, line);
line from_points(point, point);
line perpendicular(line);
line shift(line, double);
radians angle_down(line);
point rotate_point(radians, point);
line rotate_line(radians, line);
int parallel(line, line);
point intersect(line, line);

#endif
