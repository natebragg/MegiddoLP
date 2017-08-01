#ifndef ARRAY_H
#define ARRAY_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct array {
    unsigned long size;
    unsigned long length;
    unsigned char width;
    int *refs;
    void *start;
} array;

typedef struct iter {
    array *a;
    unsigned long idx;
} iter;

#define assert_type(a, type) \
    assert((a)->width == sizeof(type))

#define make_array(size, type) \
    make_array_of_width(size, sizeof(type))

static array make_array_of_width(unsigned long size, unsigned long width)
{
    array a;
    a.size = size > 0 ? size : 1;
    a.length = 0;
    a.width = width > 0 ? width : 1;
    a.refs = malloc(sizeof(int));
    *a.refs = 1;
    a.start = malloc(a.width * a.size);
    return a;
}

static array clone(array *a)
{
    array b;
    b.size = a->size;
    b.length = a->length;
    b.width = a->width;
    b.refs = a->refs;
    *b.refs += 1;
    b.start = a->start;
    return b;
}

static void free_array(array *a)
{
    if(--(*a->refs) == 0) {
        free(a->refs);
        free(a->start);
    }
}

#define index(a, idx, type) \
    (type*)index_untyped((assert_type(&a, type), a), idx)

static void *index_untyped(array a, unsigned long idx)
{
    return (char *)a.start + idx * a.width;
}

static void shrink(array *a)
{
    a->length--;
}

#define grow(a, type) \
    (type*)grow_untyped((assert_type(a, type), a))

static void *grow_untyped(array *a)
{
    if(a->length == a->size) {
        array old = *a;
        *a = make_array_of_width(2 * a->size, a->width);
        a->length = old.length;
        memcpy(a->start, old.start, a->width * a->length);
        free_array(&old);
    }
    memset(index_untyped(*a, a->length), 0, a->width);
    return index_untyped(*a, a->length++);
}

static iter make_iter(array *a)
{
    iter i;
    i.a = a;
    i.idx = 0;
    return i;
}

static int valid(iter *i)
{
    return (i->idx >= 0 && i->idx < i->a->length);
}

#define cur(i, type) \
    (type*)cur_untyped((assert_type((i)->a, type), i))

static void *cur_untyped(iter *i)
{
    return index_untyped(*i->a, i->idx);
}

#define next(i, type) \
    (type*)next_untyped((assert_type((i)->a, type), i))

static void *next_untyped(iter *i)
{
    i->idx++;
    if(!valid(i)) {
        i->idx--;
        return NULL;
    }

    return cur_untyped(i);
}

#endif
