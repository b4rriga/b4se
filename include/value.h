// SPDX-License-Identifier: LGPL-3.0-or-later
// This file is part of b4se.

#ifndef B4_VALUE_H
#define B4_VALUE_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    VALUE_OK,
    VALUE_NOT_FOUND,
    VALUE_BAD_CONVERSION
} B4ValueStatus;

typedef struct values B4Values;

bool b4_value_is_bool(const char *s);
bool b4_value_is_int(const char *s);
bool b4_value_is_float(const char *s);

B4Values *b4_value_new(void);
void b4_value_store(B4Values *vals, const char *key, const char *val);
void b4_value_dump(B4Values *vals);
void b4_value_free(B4Values *vals);

const char *b4_value_error(B4ValueStatus err);
const char *b4_value_boolstr(bool expr);

char *b4_value_get_str(B4Values *vals, const char *key, B4ValueStatus *err);
bool b4_value_get_bool(B4Values *vals, const char *key, B4ValueStatus *err);
int64_t b4_value_get_int(B4Values *vals, const char *key, B4ValueStatus *err);
double b4_value_get_float(B4Values *vals, const char *key, B4ValueStatus *err);

// TODO: graceful set functions (append if already set in file)
// TODO: forceful set functions (overwrite and prune duplicate instances)

#endif // B4_VALUE_H
