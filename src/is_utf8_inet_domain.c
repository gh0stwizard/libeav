#include <stdio.h>
#include <string.h>
#include <idn/api.h>
#include <eav.h>
#include "private.h"


/* is_utf8_inet_domain: validates that domain is fqdn & have valid tld */
extern int
is_utf8_inet_domain(idn_resconf_t ctx,
                    idn_action_t actions,
                    const char *start,
                    const char *end)
{
    idn_result_t r;
    char domain[DOMAIN_SIZE];
    char *ch = NULL;
    size_t len;
    extern int is_ascii_domain (const char *start, const char *end);
    extern int is_tld (const char *start, const char *end);
    extern int is_special_domain (const char *start, const char *end);


    if (start == end)
        return inverse(EEAV_EMPTY);

    r = idn_res_encodename (ctx, actions, start, domain, DOMAIN_SIZE - 1);

    if (r != idn_success) {
#ifdef _DEBUG
        fprintf (stderr, "idn_res_encodename '%.*s': %s\n",
                end - start, start, idn_result_tostring (r));
#endif
        return inverse(EEAV_IDNKIT_ERROR);
    }

    /* remove root dot if needed */
    len = strlen (domain);

    if (domain[len-1] == '.')
        domain[--len] = '\0';

#if 0
#ifdef _DEBUG
        fprintf (stderr, ">>> %.*s: length %d\n", len, domain, len);
#endif
#endif

    /* idn_res_encodename() does NOT check numeric domains */
    if (is_ascii_domain (domain, domain + len) != EEAV_NO_ERROR) {
#ifdef _DEBUG
        fprintf (stderr, "!!! %.*s: non-ascii\n", len, domain);
#endif
        return inverse(EEAV_NOT_ASCII);
    }

    /* reserved domain */
    if (is_special_domain (domain, domain + len)) {
#ifdef _DEBUG
        fprintf (stderr, "!!! %s: special\n", domain);
#endif
        return inverse(EEAV_SPECIAL);
    }

    /* fqdn & tld tests */
    ch = strrchr(domain, '.');

    if (ch == NULL) {
#ifdef _DEBUG
        fprintf (stderr, "!!! %s: non-fqdn\n", domain);
#endif
        return inverse(EEAV_DOMAIN_NOT_FQDN);
    }

    return (is_tld (ch + 1, end));
}
