/* This part was taken from postfix 3.2.0.
 *
 * License:
 * The Secure Mailer license must be distributed with this software.
 *
 * Author:
 * Wietse Venema
 * IBM T.J. Watson Research
 * P.O. Box 704
 * Yorktown Heights, NY 10598, USA
 */
#include <eav.h>
#include "private.h"


extern int
is_ascii_domain (const char *start, const char *end)
{
    const char *cp;
    int     label_length = 0;
    int     label_count = 0;
    int     non_numeric = 0;
    int     ch;
    extern int is_ipv6 (const char *start, const char *end);

    /*
     * Trivial cases first.
     */
    if (start == end) {
        /* empty hostname */
        return inverse(EEAV_DOMAIN_EMPTY);
    }

    /*
     * Find bad characters or label lengths. Find adjacent delimiters.
     */
    for (cp = start; start < end && (ch = *(unsigned char *) cp) != 0; cp++) {
        if (ISALNUM(ch) || ch == '_') {     /* grr.. */
            if (label_length == 0)
                label_count++;
            label_length++;
            if (label_length > VALID_LABEL_LEN) {
                /* hostname label too long */
                return inverse(EEAV_DOMAIN_LABEL_TOO_LONG);
            }
            if (!ISDIGIT(ch))
                non_numeric = 1;
        } else if (ch == '.') {
            if (label_length == 0 || cp[1] == 0) {
                /* misplaced delimiter */
                return inverse(EEAV_DOMAIN_MISPLACED_DELIMITER);
            }
            label_length = 0;
        } else if (ch == '-') {
            non_numeric = 1;
            label_length++;
            if (label_length == 1 || cp[1] == 0 || cp[1] == '.') {
                /* misplaced hyphen */
                return inverse(EEAV_DOMAIN_MISPLACED_HYPHEN);
            }
        }
#ifdef SLOPPY_VALID_HOSTNAME
        else if (ch == ':' && is_ipv6 (start, end)) {
            non_numeric = 0;
            break;
        }
#endif
        else {
            /* invalid character */
            return inverse(EEAV_DOMAIN_INVALID_CHAR);
        }
    } /* for(...) */

    if (non_numeric == 0) {
        /* numeric hostname */
#ifndef SLOPPY_VALID_HOSTNAME
        return inverse(EEAV_DOMAIN_NUMERIC);
#endif
    }

    if (cp - start > VALID_HOSTNAME_LEN) {
        /* bad length */
        return inverse(EEAV_DOMAIN_TOO_LONG);
    }

    return EEAV_NO_ERROR;
}

