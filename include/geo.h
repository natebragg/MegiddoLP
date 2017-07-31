typedef struct point {
    double x;
    double y;
} point;

typedef struct line {
    double a;
    double b;
} line;

double apply(double, line);
line from_points(point, point);
point rotate_point(double, point);
line rotate_line(double, line);
int parallel(line, line);
point intersect(line, line);
