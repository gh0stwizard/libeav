#include <stdio.h>
#include <string.h>
#include <eav.h>
#include "private.h"
#include "private_email.h"


extern int
is_email_rfc6321_fqdn ( idn_resconf_t ctx,
                        idn_action_t actions,
                        const char *email,
                        size_t length)
{
    char *ch = NULL;
    char *brs = NULL;
    char *bre = NULL;
    int r = 0;
    const char *end = email + length;


    if (length == 0)
        return inverse(EEAV_EMPTY);

    ch = strrchr (email, '@');

    if (ch == NULL)
        return inverse(EEAV_EMAIL_HAS_NO_DOMAIN);

#ifdef _DEBUG
    printf (">>> local-part: %.*s (%zu)\n", ch - email, email, ch - email);
    printf (">>> domain: %s\n", ch + 1);
#endif

    if (ch - email > VALID_LPART_LEN)
        return inverse(EEAV_LPART_TOO_LONG);

    r = is_6321_local (email, ch);

    if (r != EEAV_NO_ERROR)
        return r;

    brs = strchr(ch + 1, '[');

    if (brs == NULL)
       return (is_utf8_inet_domain (ctx, actions, ch + 1, end));

    /* seems to be an ip address */
    check_ip(); /* see private_email.h */
}


/* allow aliases / non-fqdn */
extern int
is_email_rfc6321 (  idn_resconf_t ctx,
                    idn_action_t actions,
                    const char *email,
                    size_t length)
{
    char *ch = NULL;
    char *brs = NULL;
    char *bre = NULL;
    int r = 0;
    const char *end = email + length;


    if (length == 0)
        return inverse(EEAV_EMPTY);

    ch = strrchr (email, '@');

    if (ch == NULL)
        return inverse(EEAV_EMAIL_HAS_NO_DOMAIN);

#ifdef _DEBUG
    printf (">>> local-part: %.*s (%zu)\n", ch - email, email, ch - email);
    printf (">>> domain: %s\n", ch + 1);
#endif

    if (ch - email > VALID_LPART_LEN)
        return inverse(EEAV_LPART_TOO_LONG);

    r = is_6321_local (email, ch);

    if (r != EEAV_NO_ERROR)
        return r;

    brs = strchr(ch + 1, '[');

    if (brs == NULL)
       return (is_utf8_domain (ctx, actions, ch + 1, end));

    /* seems to be an ip address */
    check_ip(); /* see private_email.h */
}
