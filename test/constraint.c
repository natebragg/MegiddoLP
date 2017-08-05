#include "constraint.h"
#include "test.h"

int test_rotate_constraint()
{
    const double pi = 3.14159265358979323846;
    {
        constraint c = {leq, {1, 1, 0}}, c_;
        radians theta;
        theta.theta = pi/2;

        c_ = rotate_constraint(theta, c);
        assert_eq(c_.ord, leq);
    }
    {
        constraint c = {geq, {1, 1, 0}}, c_;
        radians theta;
        theta.theta = pi/2;

        c_ = rotate_constraint(theta, c);
        assert_eq(c_.ord, geq);
    }
    {
        constraint c = {leq, {1, -1, 0}}, c_;
        radians theta;
        theta.theta = pi/2;

        c_ = rotate_constraint(theta, c);
        assert_eq(c_.ord, geq);
    }
    {
        constraint c = {geq, {1, -1, 0}}, c_;
        radians theta;
        theta.theta = pi/2;

        c_ = rotate_constraint(theta, c);
        assert_eq(c_.ord, leq);
    }


    return 0;
}

int main()
{
    int result = test_rotate_constraint();
    return result;
}
