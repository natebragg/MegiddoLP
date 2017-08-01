#include "geo.h"
#include "test.h"

#include <math.h>

const double epsilon = 0.00001;

int test_rotate_line()
{
    {
        line f, g, expected;
        radians theta;
        theta.theta = pi/2;
        f.a = -1;
        f.b = 0;
        expected.a = 1;
        expected.b = 0;

        g = rotate_line(theta, f);
        assert_within(g.a, expected.a, epsilon);
        assert_within(g.b, expected.b, epsilon);
    }
    {
        line f, g, expected;
        radians theta;
        theta.theta = pi;
        f.a = 0;
        f.b = 0;
        expected.a = 0;
        expected.b = 0;

        g = rotate_line(theta, f);
        assert_within(g.a, expected.a, epsilon);
        assert_within(g.b, expected.b, epsilon);
    }
    {
        line f, g, expected;
        radians theta;
        theta.theta = 5 * pi / 8;
        f.a = 0.5;
        f.b = 2;
        expected.a = -0.867295;
        expected.b = -2.367920;

        g = rotate_line(theta, f);
        assert_within(g.a, expected.a, epsilon);
        assert_within(g.b, expected.b, epsilon);
    }

    return 0;
}

int test_rotate_point()
{
    {
        point p, q, expected;
        radians theta;
        theta.theta = pi/2;
        p.x = -1;
        p.y = 0;
        expected.x = 0;
        expected.y = -1;

        q = rotate_point(theta, p);
        assert_within(q.x, expected.x, epsilon);
        assert_within(q.y, expected.y, epsilon);
    }
    {
        point p, q, expected;
        radians theta;
        theta.theta = -pi/4;
        p.x = -1;
        p.y = 1;
        expected.x = 0;
        expected.y = 1.414214;

        q = rotate_point(theta, p);
        assert_within(q.x, expected.x, epsilon);
        assert_within(q.y, expected.y, epsilon);
    }

    return 0;
}

int test_intersect()
{
    {
        line f, g;
        point p, expected;
        f.a = 2;
        f.b = 1;
        g.a = 1;
        g.b = 0;
        expected.x = -1;
        expected.y = -1;

        p = intersect(f, g);
        assert_within(p.x, expected.x, epsilon);
        assert_within(p.y, expected.y, epsilon);
    }

    {
        line f, g;
        point p, expected;
        f.a = 100;
        f.b = -100;
        g.a = -0.01;
        g.b = 0.01;
        expected.x = 1;
        expected.y = 0;

        p = intersect(f, g);
        assert_within(p.x, expected.x, epsilon);
        assert_within(p.y, expected.y, epsilon);
    }

    return 0;
}

int test_angle_down()
{
    {
        line f;
        radians theta, expected;
        f.a = 1;
        f.b = 0;
        expected.theta = 5 * pi / 4;

        theta = angle_down(f);
        assert_within(theta.theta, expected.theta, epsilon);
    }
    {
        line f;
        radians theta, expected;
        f.a = -1;
        f.b = 0;
        expected.theta = 3 * pi / 4;

        theta = angle_down(f);
        assert_within(theta.theta, expected.theta, epsilon);
    }

    return 0;
}

int main()
{
    int result = test_rotate_line();
    result += test_rotate_point();
    result += test_intersect();
    result += test_angle_down();
    return result;
}

