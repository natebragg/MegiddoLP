#include "array.h"
#include "test.h"

#include <math.h>

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

int test_index_of()
{
    array a = make_array(1, int);
    int expected = 1000;
    int v = 0, *u;
    for(; v < expected + 1; ++v) {
        u = grow(&a, int);
    }
    assert_eq(index_of(a, u), expected);
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
    for(v = 0, u = cur(i, int); u; ++v, ++v, u = next(&i, int)) {
        /* postcondition: trues are stable, falses are unstable */
        assert_eq(*u, v);
    }
    i = make_iter(&odd);
    for(u = cur(i, int); u; u = next(&i, int)) {
        assert_eq(*u % 2, 1);
    }
    free_array(&odd);
    free_array(&even);
    return 0;
}

void root(int *i, double *d)
{
    *d = sqrt(*i);
}

int test_map()
{
    array a = make_array(1, int);
    array b;
    int v = 0;
    for(; v < 101; ++v) {
        *grow(&a, int) = v;
    }
    b = map(root, a, double);
    assert_within(*index(b, 64, double), 8, 0.0001);
    assert_within(*index(b, 100, double), 10, 0.0001);
    free_array(&b);
    free_array(&a);
    return 0;
}

static int accum_total = 0;

void accum(const int *v)
{
    accum_total += *v;
}

int test_ap()
{
    array a = make_array(1, int);
    int v = 0;
    const int n = 100;
    for(; v < n; ++v) {
        *grow(&a, int) = v;
    }
    ap(accum, a);
    free_array(&a);
    assert_eq(accum_total, n*(n-1)/2);
    return 0;
}

void accum_fold(int *acc, const int *v)
{
    *acc += *v;
}

int test_foldl()
{
    array a = make_array(1, int);
    int v = 0, total = 0;
    const int n = 100;
    for(; v < n; ++v) {
        *grow(&a, int) = v;
    }
    foldl(accum_fold, &total, a, int);
    free_array(&a);
    assert_eq(total, n*(n-1)/2);
    return 0;
}

int test_split()
{
    array a = make_array(1, int);
    array bs;
    int v = 0, expected = 0, actual = -1;
    for(; v < 100; ++v) {
        *grow(&a, int) = v;
    }
    expected = *index(a, 99, int);
    bs = split(&a, 3);

    actual = *index(*index(bs, 33, array), 0, int);
    assert_eq(actual, expected);
    assert_eq(bs.length, 34);
    ap(free_array, bs);
    free_array(&bs);
    free_array(&a);
    return 0;
}


int main()
{
    int result = test_clone();
    result += test_index();
    result += test_index_of();
    result += test_shrink();
    result += test_grow();
    result += test_iter();
    result += test_partition();
    result += test_map();
    result += test_ap();
    result += test_foldl();
    result += test_split();
    return result;
}
