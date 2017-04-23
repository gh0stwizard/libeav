#include <stdio.h>
#include <string.h>
#include <idn/api.h>
#include <eav.h>
#include "private.h"


extern int
is_utf8_domain (idn_resconf_t ctx,
                idn_action_t actions,
                idn_result_t *r,
                const char *start,
                const char *end)
{
    char domain[DOMAIN_SIZE];
    extern int is_ascii_domain (const char *start, const char *end);


    if (start == end)
        return inverse(EEAV_DOMAIN_EMPTY);

    *r = idn_res_encodename (ctx, actions, start, domain, DOMAIN_SIZE - 1);

    if (*r != idn_success)
        return inverse(EEAV_IDN_ERROR);

    /* idn_res_encodename() does NOT check numeric domains */
    return is_ascii_domain (domain, domain + strlen (domain));
}
