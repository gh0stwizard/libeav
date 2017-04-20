#include <stdio.h>
#include <string.h>
#include <idn/api.h>
#include <eav.h>
#include "private.h"


extern int
is_utf8_domain (idn_resconf_t ctx,
                idn_action_t actions,
                const char *start,
                const char *end)
{
    idn_result_t r;
/*
    idn_action_t actions = IDN_MAP | IDN_ASCLOWER | IDN_RTCONV 
            | IDN_IDNCONV | IDN_UNASCHECK | IDN_PROHCHECK | IDN_NFCCHECK;
    idn_action_t actions = IDN_ENCODE_REGIST;
*/
    char domain[DOMAIN_SIZE];
    extern int is_ascii_domain (const char *start, const char *end);


    if (start == end)
        return inverse(EEAV_EMPTY);

    r = idn_res_encodename (ctx, actions, start, domain, DOMAIN_SIZE - 1);

    if (r != idn_success) {
#ifdef _DEBUG_IS_UTF8_DOMAIN
        fprintf (stderr, "idn_res_encodename '%.*s': %s\n",
                end - start, start, idn_result_tostring (r));
#endif
        return inverse(EEAV_IDNKIT_ERROR);
    }

    /* idn_res_encodename() does NOT check numeric domains */
#if 0
    fprintf (stderr, ">>> %s\n", domain);
#endif
    return (is_ascii_domain (domain, domain + strlen (domain)));
}
