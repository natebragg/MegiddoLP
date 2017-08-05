#include "geo.h"
#include "test.h"

#include <math.h>

const double epsilon = 0.00001;

int test_rotate_line()
{
    {
        line f = {1, 1, 0}, g;
        line expected = {1, -1, 0};
        radians theta;
        theta.theta = pi/2;

        g = rotate_line(theta, f);
        assert_within(g.a2 / g.a1, expected.a2 / expected.a1, epsilon);
        assert_within(g.b / g.a1, expected.b / expected.a1, epsilon);
    }
    {
        line f = {1, 0, 0}, g;
        line expected = {1, 0, 0};
        radians theta;
        theta.theta = pi;

        g = rotate_line(theta, f);
        assert_within(g.a2 / g.a1, expected.a2 / expected.a1, epsilon);
        assert_within(g.b / g.a1, expected.b / expected.a1, epsilon);
    }
    {
        line f = {1, -0.5, 2}, g;
        line expected = {1, 0.867295, -2.367920};
        radians theta;
        theta.theta = 5 * pi / 8;

        g = rotate_line(theta, f);
        assert_within(g.a2 / g.a1, expected.a2 / expected.a1, epsilon);
        assert_within(g.b / g.a1, expected.b / expected.a1, epsilon);
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
        line f = {1, -2, 1};
        line g = {1, -1, 0};
        point p, expected;
        expected.x = -1;
        expected.y = -1;

        p = intersect(f, g);
        assert_within(p.x, expected.x, epsilon);
        assert_within(p.y, expected.y, epsilon);
    }

    {
        line f = {1, -100, -100};
        line g = {1, 0.01, 0.01};
        point p, expected;
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
        line f = {1, -1, 0};
        radians theta, expected;
        expected.theta = 5 * pi / 4;

        theta = angle_down(f);
        assert_within(theta.theta, expected.theta, epsilon);
    }
    {
        line f = {2, -1, 0};
        radians theta, expected;
        expected.theta = 1.352416 * pi;

        theta = angle_down(f);
        assert_within(theta.theta, expected.theta, epsilon);
    }
    {
        line f = {1, 1, 0};
        radians theta, expected;
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

