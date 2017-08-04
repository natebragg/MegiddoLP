#ifndef LOGGER_H
#define LOGGER_H

#include "constraint.h"
#include "array.h"

typedef struct logger {
    enum {off, result, position, equations} level;
} logger;

void log_position(const logger *, const char *, ...);
void log_result(const logger *, const char *, ...);
void log_line(const logger *, line, const char *, ...);
void log_constraint(const logger *, const constraint *, const char *, ...);
void log_standard_constraint_array(const logger *, array, const char *, ...);
void log_constraint_array(const logger *, array, const char *, ...);
void log_pair_array(const logger *, array, const char *, ...);
void log_double_array(const logger *, array, const char *, ...);
void log_double_ptr_array(const logger *, array, const char *, ...);

#endif
