#include "array.h"
#include "test.h"

int test_clone()
{
    int expected = 1024;
    array a = make_array(1, int);
    array b = clone(&a);
    *grow(&b, int) = expected;
    free_array(&b);

    assert_eq(*index(a, 0, int), expected);
    free_array(&a);
    return 0;
}

int test_index()
{
    array a = make_array(1, int);
    assert_neq(index(a, 0, int), NULL);
    free_array(&a);
    return 0;
}

int test_shrink()
{
    array a = make_array(1, int);
    assert_eq(a.length, 0);
    shrink(&a);
    assert_eq(a.length, 0);
    free_array(&a);
    return 0;
}

int test_grow()
{
    int expected = 1024;
    array a = make_array(1, int);
    *grow(&a, int) = 128;
    *grow(&a, int) = 256;
    *grow(&a, int) = 512;
    *grow(&a, int) = expected;
    assert_eq(a.length, 4);
    assert_eq(*index(a, 3, int), expected);
    free_array(&a);
    return 0;
}

int test_iter()
{
    array a = make_array(1, int);
    iter i = make_iter(&a);
    int v = 0, *u;
    for(; v < 1000; ++v) {
        *grow(&a, int) = v;
    }
    for(v = 0, u = cur(i, int); u; ++v, u = next(&i, int)) {
        assert_eq(*u, v);
    }
    free_array(&a);
    return 0;
}

int is_even(int *i)
{
    return (*i) % 2 == 0;
}

int test_partition()
{
    array odd, even = make_array(1, int);
    iter i;
    int v = 0, *u;
    for(; v < 10000; ++v) {
        *grow(&even, int) = v;
    }
    odd = partition(is_even, &even);
    assert_eq(odd.length, even.length);
    i = make_iter(&even);
    for(u = cur(i, int); u; u = next(&i, int)) {
        assert_eq(*u % 2, 0);
    }
    i = make_iter(&odd);
    for(u = cur(i, int); u; u = next(&i, int)) {
        assert_eq(*u % 2, 1);
    }
    free_array(&odd);
    free_array(&even);
    return 0;
}


int main()
{
    int result = test_clone();
    result += test_index();
    result += test_shrink();
    result += test_grow();
    result += test_iter();
    result += test_partition();
    return result;
}
