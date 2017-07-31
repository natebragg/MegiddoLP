typedef struct point {
    double x;
    double y;
} point;

typedef struct line {
    double a;
    double b;
} line;

typedef struct radians {
    double theta;
} radians;

extern const double pi;

double apply(double, line);
line from_points(point, point);
radians angle_down(line);
point rotate_point(radians, point);
line rotate_line(radians, line);
int parallel(line, line);
point intersect(line, line);
