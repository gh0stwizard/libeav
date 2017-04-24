#include <stdio.h>
#include <string.h>
#include <eav.h>
#include "private.h"


typedef struct reserved_s {
    const char *domain;
    size_t length;
} reserved_t;

static const reserved_t const reserved[] = {
    { "test",           4 },
    { "example",        7 },
    { "invalid",        7 },
    { "localhost",      9 },
    { "onion",          5 }
};

static const reserved_t const example[] = {
    { "com", 3 },
    { "net", 3 },
    { "org", 3 },
};

/*
 * is_special_domain: checks whether a domain is reserved or special.
 *
 * RFC2606 & RFC6761:
 *
 * Special TLDs:
 *
 * test.
 * example.
 * invalid.
 * localhost.
 *
 * Reserved Example Second Level Domain Names:
 *
 * example.com.
 * example.net.
 * example.org.

 * RFC 7686:
 *
 * onion.
 *
 */
extern int
is_special_domain (const char *start, const char *end)
{
#define LABEL_SIZE (64)
    const char *cp = NULL;
    char *ch = NULL;
    char label[LABEL_SIZE];
    size_t len = 0;
    int count = 0;


    /* count labels */
    for (cp = start; (ch = strchr (cp, '.')) != 0; cp = ch + 1, count++);

    /* shortcut for non-fqdn */
    if (count == 0) {
        for (size_t i = 0; i < ARRAY_SIZE(reserved); i++) {
            if (strncmp (start, reserved[i].domain, reserved[i].length) == 0)
                return (YES);
        }

        return (NO);
    }

    /* check fqdn */
    cp = start;

    /* don't count root if it exists */
    if (end[-1] == '.')
        count--;

    /* we're interested in last two labels only: skip the rest. */
    while (count >= 2) {
        ch = strchr (cp, '.');
        cp = ch + 1;
        count--;
    }

    /* first label */
    ch = strchr (cp, '.');
    len = ch - cp;
    memcpy (label, cp, len);
    label[len] = 0;

    /* prepare length of the next label */
    cp = ch + 1;
    ch = strchr (cp, '.');

    if (ch == NULL)
        len = end - cp;
    else
        len = end - cp - 1;

    if (strncmp ("example", label, 7) == 0) {
        if (len == 0) /* just 'example.' */
            return (YES);

        if (len != 3) /* there are only com, net, org */
            return (NO);

        memcpy (label, cp, len);
        label[len] = 0;

        /* probably reserved example.tld */
        for (size_t i = 0; i < ARRAY_SIZE(example); i++)
            if (strncmp (example[i].domain, label, example[i].length) == 0)
                return (YES);
    }
    else {
        if (len == 0) /* unreserved */
            return (NO);

        if (len < 4 || len > 9 || len == 6 || len == 8)
            return (NO);

        memcpy (label, cp, len);
        label[len] = 0;

        /* probably reserved fqdn */
        for (size_t i = 0; i < ARRAY_SIZE(reserved); i++) {
            if (strncmp (reserved[i].domain, label, reserved[i].length) == 0)
                return (YES);
        }
    }

    return (NO);
}
