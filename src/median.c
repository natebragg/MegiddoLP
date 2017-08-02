#include "median.h"

static int cmpx(const void *lhs, const void *rhs)
{
    return *(double*)lhs < *(double*)rhs ? -1 : 1;
}

static void find_median_(array *a, double *median)
{
    *median = find_median(a);
}

static double find_median_candidate(array *a)
{
    double result = 0;
    array fivers = split(a, 5);
    array medians = map(find_median_, fivers, double);
    if(medians.length == 1) {
        result = *index(medians, 0, double);
    } else {
        result = find_median_candidate(&medians);
    }
    ap(free_array, fivers);
    free_array(&fivers);
    free_array(&medians);
    return result;
}

static double candidate = 0;

static int less_than_candidate(const double *v)
{
    return *v < candidate;
}

double find_rank(size_t r, array *a)
{
    double result = 0;
    assert(a->length > r);
    if(a->length <= 5) {
        qsort(a->start, a->length, a->width, cmpx);
        result = *index(*a, r, double);
    } else {
        array less = clone(a), geqs;
        size_t pos = 0;
        candidate = find_median_candidate(a);
        geqs = partition(less_than_candidate, &less);
        pos = less.length;
        if(pos == r) {
            result = candidate;
        } else if (pos > r) {
            result = find_rank(r, &less);
        } else if (pos < r) {
            result = find_rank(r - pos, &geqs);
        }
        free_array(&less);
        free_array(&geqs);
    }
    return result;
}

double find_median(array *a)
{
    return find_rank(a->length/2, a);
}
