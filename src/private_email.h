#ifndef PRIVATE_EMAIL_H
#define PRIVATE_EMAIL_H

#include "auto_tld.h"

#define basic_email_check(e) do { \
    if (length == 0) \
        return inverse(EEAV_EMAIL_EMPTY); \
\
    ch = strrchr ((e), '@'); \
\
    if (ch == NULL) \
        return inverse(EEAV_LPART_EMPTY); \
    else if (ch == end) \
        return inverse(EEAV_DOMAIN_EMPTY); \
\
    if (ch - (e) > VALID_LPART_LEN) \
        return inverse(EEAV_LPART_TOO_LONG); \
} while (0)

#define check_tld() do { \
    if (tld_check == false) \
        return EEAV_NO_ERROR; \
\
    if (is_special_domain (ch + 1, end)) \
        return TLD_TYPE_SPECIAL; \
\
    /* fqdn & tld tests */ \
    ch = strrchr(ch + 1, '.'); \
\
    if (ch == NULL) \
        return inverse(EEAV_DOMAIN_NOT_FQDN); \
\
    return (is_tld (ch + 1, end)); \
} while (0)


#define check_ip() do { \
    if (end - brs <= 8) /* minimum allowed IP length: 1.2.3.4 */ \
        return inverse(EEAV_IPADDR_INVALID); \
\
    bre = strrchr(brs, ']'); \
\
    if (bre == NULL) \
        return inverse(EEAV_IPADDR_BRACKET_UNPAIR); \
\
    if (ISDIGIT(brs[1])) { /* ip address, possibly ipv4 */ \
        if (is_ipaddr (brs + 1, bre) == 0) \
            return inverse(EEAV_IPADDR_INVALID); \
    } \
    else { /* try ipv6 */ \
        ch = strchr (brs + 1, ':'); \
        if ((ch == NULL) || (is_ipaddr (ch + 1, bre) == 0)) \
            return inverse(EEAV_IPADDR_INVALID); \
    } \
    /* valid ip addr. */ \
    return EEAV_NO_ERROR; \
} while (0)

#endif /* PRIVATE_EMAIL_H */
