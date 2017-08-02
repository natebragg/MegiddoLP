#include "median.h"
#include "test.h"

#include <time.h>
#include <stdlib.h>

int test_find_median()
{
    size_t j = 0, count = 99, greater = 0, equal = 0, less = 0;
    array a = make_array(count, double);
    iter i = make_iter(&a);
    double median = 0;
    srand(time(NULL));
    for(; j < count; ++j) {
        *grow(&a, double) = ((double)rand())/((double)rand());
    }
    median = find_median(&a);
    for(; cur(i, double); next(&i, double)) {
        double v = *cur(i, double);
        if(v < median) {
            less++;
        } else if (v > median) {
            greater++;
        } else {
            equal++;
        }
    }
    
    assert_eq(greater, 49);
    assert_eq(less, 49);
    assert_eq(equal, 1);
    free_array(&a);
    return 0;
}


int main()
{
    int result = test_find_median();
    return result;
}

