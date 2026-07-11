#ifndef B4_VALUE_PRIV_H
#define B4_VALUE_PRIV_H

#include "../include/value.h"

enum value_type {
    B4VAL_BOOL  = 1u << 0,
    B4VAL_INT   = 1u << 1,
    B4VAL_FLOAT = 1u << 2
};

struct value_entry {
    char *key;

    char *s;
    int64_t i;
    double f;
    bool b;

    uint8_t types;
};

struct values {
    struct value_entry *data;
    int len, cap;
};

// TODO: get from b4se/strplus.h
char *xstrdup(const char *s);
bool stricmp(const char *a, const char *b);
char *trim(char *s);
char *unquote(const char *s);

#endif // B4_VALUE_PRIV_H
