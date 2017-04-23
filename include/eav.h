#ifndef EAV_H
#define EAV_H

#include <idn/api.h>
#include <stdbool.h>


typedef int EAV_RFC;
enum {
    EAV_RFC_822,
    EAV_RFC_5321,
    EAV_RFC_5322,
    EAV_RFC_6531,
};

/* XXX conflicts with auto-gerated?? */
typedef int EAV_TLD;
enum {
    EAV_TLD_INVALID             = 1 << 1,
    EAV_TLD_NOT_ASSIGNED        = 1 << 2,
    EAV_TLD_COUNTRY_CODE        = 1 << 3,
    EAV_TLD_GENERIC             = 1 << 4,
    EAV_TLD_GENERIC_RESTRICTED  = 1 << 5,
    EAV_TLD_INFRASTRUCTURE      = 1 << 6,
    EAV_TLD_SPONSORED           = 1 << 7,
    EAV_TLD_TEST                = 1 << 8,
    EAV_TLD_SPECIAL             = 1 << 9,
};

/* eav.c utf-8 callback */
typedef int (*eav_utf8_f) ( idn_resconf_t,
                            idn_action_t,
                            idn_result_t *,
                            const char *,
                            size_t,
                            bool tld_check);
/* eav.c ascii callback */
typedef int (*eav_ascii_f) (const char *, size_t, bool);

typedef struct eav_s {
    int             rfc;        /* choose rfc */
    int             allow_tld;  /* flag: allow only these TLDs */
    bool            utf8;       /* allow utf8 */
    bool            tld_check;  /* do fqdn & tld checks */
    /* XXX private */
    int             errcode;
    const char      *idnmsg;
    bool            initialized;
    idn_resconf_t   idn;
    idn_action_t    actions;
    eav_utf8_f      utf8_cb;
    eav_ascii_f     ascii_cb;
} eav_t;

/* libeav error codes */
typedef int TLD_ERROR;
enum {
    EEAV_NO_ERROR,
    EEAV_INVALID_RFC,
    EEAV_IDN_ERROR,
    EEAV_EMAIL_EMPTY,
    EEAV_LPART_EMPTY,
    EEAV_LPART_TOO_LONG,
    EEAV_LPART_NOT_ASCII,
    EEAV_LPART_SPECIAL,
    EEAV_LPART_CTRL_CHAR,
    EEAV_LPART_MISPLACED_QUOTE,
    EEAV_LPART_UNQUOTED,
    EEAV_LPART_TOO_MANY_DOTS,
    EEAV_LPART_UNQUOTED_FWS,
    EEAV_LPART_INVALID_UTF8,
    EEAV_DOMAIN_EMPTY,
    EEAV_DOMAIN_LABEL_TOO_LONG,
    EEAV_DOMAIN_MISPLACED_HYPHEN,
    EEAV_DOMAIN_MISPLACED_DELIMITER,
    EEAV_DOMAIN_INVALID_CHAR,
    EEAV_DOMAIN_TOO_LONG,
    EEAV_DOMAIN_NUMERIC,
    EEAV_DOMAIN_NOT_FQDN,
    EEAV_IPADDR_INVALID,
    EEAV_IPADDR_BRACKET_UNPAIR,
    EEAV_TLD_INVALID,
    EEAV_TLD_NOT_ASSIGNED,
    EEAV_TLD_COUNTRY_CODE,
    EEAV_TLD_GENERIC,
    EEAV_TLD_GENERIC_RESTRICTED,
    EEAV_TLD_INFRASTRUCTURE,
    EEAV_TLD_SPONSORED,
    EEAV_TLD_TEST,
    EEAV_TLD_SPECIAL,
    EEAV_MAX
};


extern void
eav_init (eav_t *eav);

extern void
eav_free (eav_t *eav);

extern int
eav_setup (eav_t *eav);

extern int
eav_is_email (eav_t *eav, const char *email, size_t length);

extern const char *
eav_errstr (eav_t *eav);

extern int
is_6531_local (const char *start, const char *end);

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
                idn_result_t *r,
                const char *start,
                const char *end,
                bool tld_check);

extern int
is_tld (const char *start, const char *end);

extern int
is_special_domain (const char *start, const char *end);

extern int
is_6531_email ( idn_resconf_t ctx,
                idn_action_t actions,
                idn_result_t *r,
                const char *email,
                size_t length,
                bool tld_check);

extern int
is_822_email (const char *email, size_t length, bool tld_check);

extern int
is_5321_email (const char *email, size_t length, bool tld_check);

extern int
is_5322_email (const char *email, size_t length, bool tld_check);

extern int
is_ipaddr (const char *start, const char *end);

#endif /* EAV_H */
