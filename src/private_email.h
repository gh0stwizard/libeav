#ifndef PRIVATE_EMAIL_H
#define PRIVATE_EMAIL_H


#define check_ip() do { \
    if (end - brs <= 8) /* minimum allowed IP: 1.2.3.4 */ \
        return inverse(EEAV_IPADDR_INVALID); \
 \
    bre = strchr(email, ']'); \
 \
    if (bre == NULL) \
        return inverse(EEAV_IPADDR_BRACKET_UNPAIR); \
 \
    if (ISDIGIT(brs[1])) { /* ip address, possibly ipv4 */ \
        if (is_ipaddr (brs + 1, bre) == 0) \
            return inverse(EEAV_IPADDR_INVALID); \
    } \
    else { \
        ch = strchr (brs + 1, ':'); \
        if (ch == NULL) \
            return inverse(EEAV_IPADDR_INVALID); \
        if (is_ipaddr (ch + 1, bre) == 0) \
            return inverse(EEAV_IPADDR_INVALID); \
    } \
    /* valid ip addr. */ \
    return EEAV_NO_ERROR; \
} while (0)

#endif /* PRIVATE_EMAIL_H */
