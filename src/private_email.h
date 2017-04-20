#ifndef PRIVATE_EMAIL_H
#define PRIVATE_EMAIL_H


#define check_ip() do { \
    if (end - brs <= 8) /* minimum allowed IP: 1.2.3.4 */ \
        return inverse(EEAV_EMAIL_INVALID_IPADDR); \
 \
    bre = strchr(email, ']'); \
 \
    if (bre == NULL) \
        return inverse(EEAV_EMAIL_NO_PAIRED_BRACKET); \
 \
    if (ISDIGIT(brs[1])) { /* ip address, possibly ipv4 */ \
        if (is_ipaddr (brs + 1, bre) == 0) \
            return inverse(EEAV_EMAIL_INVALID_IPADDR); \
    } \
    else { \
        ch = strchr (brs + 1, ':'); \
        if (ch == NULL) \
            return inverse(EEAV_EMAIL_INVALID_IPADDR); \
        if (is_ipaddr (ch + 1, bre) == 0) \
            return inverse(EEAV_EMAIL_INVALID_IPADDR); \
    } \
    /* valid ip addr. */ \
    return EEAV_NO_ERROR; \
} while (0)

#endif /* PRIVATE_EMAIL_H */
