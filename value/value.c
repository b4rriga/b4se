// SPDX-License-Identifier: LGPL-3.0-or-later
// This file is part of b4se.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "priv.h"

bool b4_value_is_bool(const char *s)
{
    if (stricmp(s, "true")  ||
        stricmp(s, "false") ||
        stricmp(s, "on")    ||
        stricmp(s, "off")   ||
        stricmp(s, "yes")   ||
        stricmp(s, "no"))
        return true;

    char *e;
    double d = strtod(s, &e);
    if (*e != 0) return false;

    return d == 0.0 || d == 1.0;
}

// TODO: accept #AABBCC and #AABBCCDD as number for use as color
bool b4_value_is_int(const char *s)
{
    if (!*s) return false;
    char *e;

    if (s[0] == '0' && (s[1] == 'b' || s[1] == 'B')) {
        strtol(s + 2, &e, 2);
        if (*e == 0) return true;
    } else {
        strtol(s, &e, 0);
        if (*e == 0) return true;
    }

    double d  = strtod(s, &e);
    if (*e != 0) return false;

    return d == (double)(int64_t)d;
}

// TODO: support scientific notation
bool b4_value_is_float(const char *s)
{
    if (!*s) return false;
    char *e;

    if (strchr(s, '.')) {
        strtod(s, &e);
        if (*e == 0) return true;
    }

    return b4_value_is_int(s);
}

static struct value_entry *get(B4Values *vals, const char *key)
{
    for (int i = 0; i < vals->len; i++)
        if (strcmp(vals->data[i].key, key) == 0)
            return &vals->data[i];

    return NULL;
}

static struct value_entry *get_as(B4Values *vals, const char *key, uint8_t type, B4ValueStatus *err)
{
    struct value_entry *e = get(vals, key);

    if (!e) {
        if (err) *err = VALUE_NOT_FOUND;
        return NULL;
    }

    if (!(e->types & type)) {
        if (err) *err = VALUE_BAD_CONVERSION;
        return NULL;
    }

    if (err) *err = VALUE_OK;

    return e;
}

static void push(B4Values *vals, struct value_entry e)
{
    struct value_entry *old = get(vals, e.key);

    if (old) {
        free(old->key);
        free(old->s);

        *old = e;
        return;
    }

    if (vals->len == vals->cap) {
        vals->cap = vals->cap ? vals->cap * 2 : 16;
        vals->data = realloc(vals->data, vals->cap * sizeof(struct value_entry));
    }

    vals->data[vals->len++] = e;
}

static void store_bool(struct value_entry *e, const char *val)
{
    e->types |= VALUE_BOOL;

    if (stricmp(val, "true") ||
        stricmp(val, "on")   ||
        stricmp(val, "yes")) {
        e->b = true;
        return;
    }

    if (stricmp(val, "false") ||
        stricmp(val, "off")   ||
        stricmp(val, "no")) {
        e->b = false;
        return;
    }

    e->b = (strtod(val, NULL) == 1.0);
}

static void store_int(struct value_entry *e, const char *val)
{
    e->types |= VALUE_INT;

    if (val[0] == '0' && (val[1] == 'b' || val[1] == 'B'))
        e->i = strtol(val + 2, NULL, 2);
    else
        e->i = strtol(val, NULL, 0);
}

static void store_float(struct value_entry *e, const char *val)
{
    e->types |= VALUE_FLOAT;

    e->f = strtod(val, NULL);
}

B4Values *b4_value_new(void)
{
    return calloc(1, sizeof(struct values));
}

void b4_value_store(B4Values *vals, const char *key, const char *val)
{
    if (!vals || !key || !val) return;

    struct value_entry e = {0};
    e.key = trim(xstrdup(key));
    e.s = unquote(val);

    if (b4_value_is_bool(e.s))  store_bool(&e, e.s);
    if (b4_value_is_int(e.s))   store_int(&e, e.s);
    if (b4_value_is_float(e.s)) store_float(&e, e.s);

    push(vals, e);
}

void b4_value_dump(B4Values *vals)
{
    for (int i = 0; i < vals->len; i++) {
        const struct value_entry e = vals->data[i];
        fprintf(stderr, "%s\n", e.key);

        if (e.types & VALUE_BOOL)
            fprintf(stderr, "├── as bool   : %s\n", b4_value_boolstr(e.b));
        if (e.types & VALUE_INT)
            fprintf(stderr, "├── as int    : %ld\n", e.i);
        if (e.types & VALUE_FLOAT) {
            fprintf(stderr, "├── as float  : %g", e.f);

            if (e.types & VALUE_INT)
                fprintf(stderr, ".0"); // FIXME: sloppy attempt that fails miserably
                                       //        when met with scientific notation
            fprintf(stderr, "\n");
        }
        fprintf(stderr, "└── as string : \"%s\"\n", e.s);
    }
}

void b4_value_free(B4Values *vals)
{
    free(vals->data);
    free(vals);
}

const char *b4_value_error(B4ValueStatus err)
{
    switch (err) {
    case VALUE_OK:             return "OPERATION OK";
    case VALUE_NOT_FOUND:      return "KEY NOT FOUND";
    case VALUE_BAD_CONVERSION: return "CONVERSION NOT POSSIBLE";
    default:                   return "UNKNOWN ERROR";
    }
}

const char *b4_value_boolstr(bool expr)
{
    return expr ? "true" : "false";
}

char *b4_value_get_str(B4Values *vals, const char *key, B4ValueStatus *err)
{
    struct value_entry *e = get(vals, key);

    if (!e) {
        if (err) *err = VALUE_NOT_FOUND;
        return NULL;
    }

    if (err) *err = VALUE_OK;

    return e->s;
}

bool b4_value_get_bool(B4Values *vals, const char *key, B4ValueStatus *err)
{
    struct value_entry *e = get_as(vals, key, VALUE_BOOL, err);
    return e ? e->b : false;
}

int64_t b4_value_get_int(B4Values *vals, const char *key, B4ValueStatus *err)
{
    struct value_entry *e = get_as(vals, key, VALUE_INT, err);
    return e ? e->i : 0;
}

double b4_value_get_float(B4Values *vals, const char *key, B4ValueStatus *err)
{
    struct value_entry *e = get_as(vals, key, VALUE_FLOAT, err);
    return e ? e->f : 0.0;
}
