#include <stdio.h>
#include <string.h>
#include <eav.h>
#include <eav/private.h>
#include <eav/private_email.h>


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

    /* see "private_email.h" */
    basic_email_check (email);

    rc = is_6531_local (email, ch);

    if (rc != EEAV_NO_ERROR)
        return rc;

    brs = ch + 1;

    if (*brs != '[')
       return (is_utf8_domain (ctx, actions, r, ch + 1, end, tld_check));

    /* seems to be an ip address */
    check_ip(); /* see private_email.h */
}
