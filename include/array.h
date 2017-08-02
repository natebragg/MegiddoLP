#ifndef ARRAY_H
#define ARRAY_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef struct array {
    size_t size;
    size_t length;
    size_t width;
    int *refs;
    void *start;
} array;

typedef struct iter {
    array *a;
    ptrdiff_t idx;
} iter;

#define assert_type(a, type) \
    assert((a)->width == sizeof(type))

#define make_array(size, type) \
    make_array_of_width(size, sizeof(type))

static array make_array_of_width(size_t size, size_t width)
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
    ((type*)index_untyped((assert_type(&a, type), a), idx))

static void *index_untyped(array a, ptrdiff_t idx)
{
    return (char *)a.start + idx * a.width;
}

static size_t index_of(array a, void *value)
{
    return ((char *)value - (char *)a.start) / a.width;
}

static void shrink(array *a)
{
    a->length = a->length == 0 ? 0 : a->length - 1;
}

#define grow(a, type) \
    ((type*)grow_untyped((assert_type(a, type), a)))

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

static int valid(iter i)
{
    return (i.idx >= 0 && i.idx < i.a->length);
}

#define cur(i, type) \
    ((type*)cur_untyped((assert_type((i).a, type), i)))

static void *cur_untyped(iter i)
{
    if(!valid(i)) {
        return NULL;
    }

    return index_untyped(*i.a, i.idx);
}

#define next(i, type) \
    ((type*)next_untyped((assert_type((i)->a, type), i)))

static void *next_untyped(iter *i)
{
    if(!valid(*i)) {
        return NULL;
    }

    i->idx++;
    return cur_untyped(*i);
}

#define partition(pred, a) \
    partition_untyped((int (*)(const void *))pred, a)

static array partition_untyped(int (*pred)(const void *), array *a)
{
    array falses = clone(a);
    iter i = make_iter(a);
    void *elem = NULL;
    void *tmp = malloc(a->width);
    for(elem = cur_untyped(i); elem; elem = next_untyped(&i)) {
        if(pred(elem)) {
            if(elem > falses.start) {
                memcpy(tmp, falses.start, a->width);
                memcpy(falses.start, elem, a->width);
                memcpy(elem, tmp, a->width);
            }
            falses.start = index_untyped(falses, 1);
            falses.length -= 1;
            falses.size -= 1;
        }
    }
    free(tmp);
    a->length -= falses.length;
    a->size = a->length;
    return falses;
}

#define partition1(pred, userdata, a) \
    partition1_untyped((int (*)(const void *, const void *))pred, userdata, a)

static array partition1_untyped(int (*pred)(const void *, const void *), void *userdata, array *a)
{
    array falses = clone(a);
    iter i = make_iter(a);
    void *elem = NULL;
    void *tmp = malloc(a->width);
    for(elem = cur_untyped(i); elem; elem = next_untyped(&i)) {
        if(pred(userdata, elem)) {
            if(elem > falses.start) {
                memcpy(tmp, falses.start, a->width);
                memcpy(falses.start, elem, a->width);
                memcpy(elem, tmp, a->width);
            }
            falses.start = index_untyped(falses, 1);
            falses.length -= 1;
            falses.size -= 1;
        }
    }
    free(tmp);
    a->length -= falses.length;
    a->size = a->length;
    return falses;
}

static array partition_at(size_t idx, array *a)
{
    array front = clone(a);
    size_t len = a->length < idx ? a->length : idx;
    size_t siz = a->size   < idx ? a->size   : idx;
    front.length = len;
    front.size = siz;
    a->start = index_untyped(*a, siz);
    a->length -= len;
    a->size -= siz;
    return front;
}

#define map(f, in, out_type) \
    map_untyped((void (*)(const void *, void *))f, in, sizeof(out_type))

static array map_untyped(void (*f)(const void *, void *), array in, size_t width)
{
    array out = make_array_of_width(in.length, width);
    iter i = make_iter(&in);
    void *v = NULL, *u = malloc(width);
    for(v = cur_untyped(i); v; v = next_untyped(&i)) {
        f(v, u);
        memcpy(grow_untyped(&out), u, width);
    }
    free(u);
    return out;
}

#define map1(f, userdata, in, out_type) \
    map1_untyped((void (*)(const void *, const void *, void *))f, userdata, in, sizeof(out_type))

static array map1_untyped(void (*f)(const void *, const void *, void *), const void *userdata, array in, size_t width)
{
    array out = make_array_of_width(in.length, width);
    iter i = make_iter(&in);
    void *v = NULL, *u = malloc(width);
    for(v = cur_untyped(i); v; v = next_untyped(&i)) {
        f(userdata, v, u);
        memcpy(grow_untyped(&out), u, width);
    }
    free(u);
    return out;
}

#define ap(f, in) \
    ap_untyped((void (*)(const void *))f, in)

static void ap_untyped(void (*f)(const void *), array in)
{
    iter i = make_iter(&in);
    void *v = NULL;
    for(v = cur_untyped(i); v; v = next_untyped(&i)) {
        f(v);
    }
}

#define foldl(f, accum, in) \
    foldl_untyped((void (*)(void *, const void *))f, accum, in)

static void *foldl_untyped(void (*f)(void *, const void *), void *accum, array in)
{
    iter i = make_iter(&in);
    void *v = NULL;
    for(v = cur_untyped(i); v; v = next_untyped(&i)) {
        f(accum, v);
    }
    return accum;
}

static array split(array *a, size_t size)
{
    size_t bs_count = (a->length + size - 1) / size;
    array bs = make_array(bs_count, array);
    array rest = clone(a);
    int i = 0;
    for(; i < bs_count; ++i) {
        *grow(&bs, array) = partition_at(size, &rest);
    }
    free_array(&rest);
    return bs;
}

#endif
