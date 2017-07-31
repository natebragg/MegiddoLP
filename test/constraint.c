#include "constraint.h"
#include "test.h"

int test_rotate_constraint()
{
    const double pi = 3.14159265358979323846;
    {
        constraint c, c_;
        c.ord = leq;
        c.f.a = -1;
        c.f.b = 0;

        c_ = rotate_constraint(pi/2, c);
        assert(c_.ord == leq);
    }
    {
        constraint c, c_;
        c.ord = geq;
        c.f.a = -1;
        c.f.b = 0;

        c_ = rotate_constraint(pi/2, c);
        assert(c_.ord == geq);
    }
    {
        constraint c, c_;
        c.ord = leq;
        c.f.a = 1;
        c.f.b = 0;

        c_ = rotate_constraint(pi/2, c);
        assert(c_.ord == geq);
    }
    {
        constraint c, c_;
        c.ord = geq;
        c.f.a = 1;
        c.f.b = 0;

        c_ = rotate_constraint(pi/2, c);
        assert(c_.ord == leq);
    }


    return 0;
}

int main()
{
    int result = test_rotate_constraint();
    return result;
}
