#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "priv.h"

char *xstrdup(const char *s)
{
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

bool stricmp(const char *a, const char *b)
{
    while (*a && *b) {
        char ca = *a++;
        char cb = *b++;

        if ('A' <= ca && ca <= 'Z')
            ca |= 0x20;
        if ('A' <= cb && cb <= 'Z')
            cb |= 0x20;

        if (ca != cb)
            return false;
    }

    return *a == *b;
}

char *trim(char *s)
{
    while (*s == ' ' || *s == '\t') s++;

    char *end = s + strlen(s);
    while (end > s && (
           end[-1] == ' '  || end[-1] == '\t' ||
           end[-1] == '\n' || end[-1] == '\r'))
        *--end = 0;

    return s;
}

char *unquote(const char *s)
{
    size_t n = strlen(s);

    if (n >= 2 && s[0] == '"' && s[n - 1] == '"') {
        char *r = malloc(n - 1);
        memcpy(r, s + 1, n - 2);
        r[n - 2] = 0;
        return r;
    }

    return xstrdup(s);
}

