#include <stdio.h>
#include <string.h>
#include <eav.h>
#include "private.h"
#include "private_email.h"


extern int
is_6531_email ( idn_resconf_t ctx,
                idn_action_t actions,
                idn_result_t *r,
                const char *email,
                size_t length,
                bool tld_check)
{
    char *ch = NULL;
    char *brs = NULL;
    char *bre = NULL;
    int rc = 0;
    const char *end = email + length;


    if (length == 0)
        return inverse(EEAV_EMAIL_EMPTY);

    ch = strrchr (email, '@');

    if (ch == NULL || ch == end)
        return inverse(EEAV_DOMAIN_EMPTY);

#ifdef _DEBUG
    printf (">>> local-part: %.*s (%zu)\n", ch - email, email, ch - email);
    printf (">>> domain: %s\n", ch + 1);
#endif

    if (ch - email > VALID_LPART_LEN)
        return inverse(EEAV_LPART_TOO_LONG);

    rc = is_6531_local (email, ch);

    if (rc != EEAV_NO_ERROR)
        return rc;

    brs = ch + 1;

    if (*brs != '[')
       return (is_utf8_domain (ctx, actions, r, ch + 1, end, tld_check));

    /* seems to be an ip address */
    check_ip(); /* see private_email.h */
}
