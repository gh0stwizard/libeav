#ifndef EAV_H
#define EAV_H

#include <idn/api.h>

/* libeav error codes */
enum {
    EEAV_NO_ERROR,
    EEAV_EMPTY,
    EEAV_IDNKIT_ERROR,
    EEAV_NOT_ASCII,
    EEAV_SPECIAL,
    EEAV_CTRL_CHAR,
    EEAV_LPART_MISPLACED_QUOTE,
    EEAV_LPART_SPECIAL,
    EEAV_LPART_UNQUOTED,
    EEAV_LPART_TOO_MANY_DOTS,
    EEAV_LPART_UNQUOTED_FWS,
    EEAV_LPART_INVALID_UTF8,
    EEAV_DOMAIN_LABEL_TOO_LONG,
    EEAV_DOMAIN_MISPLACED_DELIMITER,
    EEAV_DOMAIN_MISPLACED_HYPHEN,
    EEAV_DOMAIN_INVALID_CHAR,
    EEAV_DOMAIN_TOO_LONG,
    EEAV_DOMAIN_NUMERIC,
    EEAV_DOMAIN_NOT_FQDN,
    EEAV_EMAIL_HAS_NO_DOMAIN,
    EEAV_LPART_TOO_LONG,
    EEAV_EMAIL_INVALID_IPADDR,
    EEAV_EMAIL_NO_PAIRED_BRACKET,
    EEAV_MAX /* debug & test only */
};

extern int
is_6321_local (const char *start, const char *end);

extern int
is_5322_local (const char *start, const char *end);

extern int
is_5321_local (const char *start, const char *end);

extern int
is_822_local (const char *start, const char *end);

extern int
is_ipv4 (const char *start, const char *end);

extern int
is_ipv6 (const char *start, const char *end);

extern int
is_ascii_domain (const char *start, const char *end);

extern int
is_utf8_domain (idn_resconf_t ctx,
                idn_action_t actions,
                const char *start,
                const char *end);

extern int
is_tld (const char *start, const char *end);

extern int
is_utf8_inet_domain(idn_resconf_t ctx,
                    idn_action_t actions,
                    const char *start,
                    const char *end);

extern int
is_special_domain (const char *start, const char *end);

extern int
is_email_rfc6321 (  idn_resconf_t ctx,
                    idn_action_t actions,
                    const char *email,
                    size_t length);

extern int
is_email_rfc6321_fqdn ( idn_resconf_t ctx,
                        idn_action_t actions,
                        const char *email,
                        size_t length);

extern int
is_email_rfc822 (const char *email, size_t length);

extern int
is_email_rfc5321 (const char *email, size_t length);

extern int
is_email_rfc5322 (const char *email, size_t length);

extern int
is_ipaddr (const char *start, const char *end);


#endif /* EAV_H */
