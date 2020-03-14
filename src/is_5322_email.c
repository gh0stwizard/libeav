#include <stdio.h>
#include <string.h>
#include <eav.h>
#include <eav/private.h>
#include <eav/private_email.h>


extern int
is_5322_email (const char *email, size_t length, bool tld_check)
{
    char *ch = NULL;
    char *brs = NULL;
    char *bre = NULL;
    int rc;
    const char *end = email + length;

    /* see "private_email.h" */
    basic_email_check (email);

    rc = is_5322_local (email, ch);

    if (rc != EEAV_NO_ERROR)
        return rc;

    brs = ch + 1;

    if (*brs != '[') {
        rc = is_ascii_domain (ch + 1, end);

        if (rc != EEAV_NO_ERROR)
            return rc;

        check_tld();
    }

    /* seems to be an ip address */
    check_ip(); /* see private_email.h */
}
