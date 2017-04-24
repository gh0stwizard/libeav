#include <stdio.h>
#include <stdlib.h>
#include <eav.h>
#include "private.h"
#include "auto_tld.h"


static const char * const errors[EEAV_MAX] = {
    "no error"                              /* EEAV_NO_ERROR */,
    "invalid RFC specified"                 /* EEAV_INVALID_RFC */,
    "idn internal error"                    /* EEAV_IDN_ERROR */,
    "empty email address"                   /* EEAV_EMAIL_EMPTY */,
    "local-part is empty"                   /* EEAV_LPART_EMPTY */,
    "local-part is too long"                /* EEAV_LPART_TOO_LONG */,
    "local-part has non-ascii characters"   /* EEAV_LPART_NOT_ASCII */,
    "local-part has special characters"     /* EEAV_LPART_SPECIAL */,
    "local-part has control characters"     /* EEAV_LPART_CTRL_CHAR */,
    "local-part has misplaced double quote" /* EEAV_LPART_MISPLACED_QUOTE */,
    "local-part has open double quote"      /* EEAV_LPART_UNQUOTED */,
    "local-part has too many dots"          /* EEAV_LPART_TOO_MANY_DOTS */,
    "local-part has unquoted characters"    /* EEAV_LPART_UNQUOTED_FWS */,
    "local-part has invalid UTF-8 data"     /* EEAV_LPART_INVALID_UTF8 */,
    "domain is empty"                       /* EEAV_DOMAIN_EMPTY */,
    "domain label is too long"              /* EEAV_DOMAIN_LABEL_TOO_LONG */,
    "domain has misplaced hyphen"           /* EEAV_DOMAIN_MISPLACED_HYPHEN */,
    "domain has misplaced delimiter"        /* EEAV_DOMAIN_MISPLACED_DELIMITER */,
    "domain has invalid characters"         /* EEAV_DOMAIN_INVALID_CHAR */,
    "domain is too long"                    /* EEAV_DOMAIN_TOO_LONG */,
    "domain is all-numeric"                 /* EEAV_DOMAIN_NUMERIC */,
    "domain is not FQDN"                    /* EEAV_DOMAIN_NOT_FQDN */,
    "ip-addr is incorrect"                  /* EEAV_IPADDR_INVALID */,
    "ip-addr has unpaired bracket"          /* EEAV_IPADDR_BRACKET_UNPAIR */,
    "invalid TLD"                           /* EEAV_TLD_INVALID */,
    "not assigned TLD"                      /* EEAV_TLD_NOT_ASSIGNED */,
    "country-code TLD"                      /* EEAV_TLD_COUNTRY_CODE */,
    "generic TLD"                           /* EEAV_TLD_GENERIC */,
    "generic-restricted TLD"                /* EEAV_TLD_GENERIC_RESTRICTED */,
    "infrastructure TLD"                    /* EEAV_TLD_INFRASTRUCTURE */,
    "sponsored TLD"                         /* EEAV_TLD_SPONSORED */,
    "test TLD"                              /* EEAV_TLD_TEST */,
    "special TLD"                           /* EEAV_TLD_SPECIAL */
};


static int
init_idn (eav_t *eav)
{
    idn_result_t r;


    if (eav->initialized)
        return EEAV_NO_ERROR;

    r = idn_resconf_initialize ();

    if (r != idn_success) {
        eav->idnmsg = idn_result_tostring (r);
        return inverse(EEAV_IDN_ERROR);
    }

    r = idn_resconf_create (&(eav->idn));

    if (r != idn_success) {
        eav->idnmsg = idn_result_tostring (r);
        return inverse(EEAV_IDN_ERROR);
    }

    eav->initialized = true;

    return EEAV_NO_ERROR;
}


extern void
eav_init (eav_t *eav)
{
    eav->actions = IDN_ENCODE_REGIST;
    eav->utf8 = false;
    eav->rfc = EAV_RFC_6531;
    eav->tld_check = true;
    eav->allow_tld =    EAV_TLD_COUNTRY_CODE |
                        EAV_TLD_GENERIC |
                        EAV_TLD_GENERIC_RESTRICTED |
                        EAV_TLD_INFRASTRUCTURE |
                        EAV_TLD_SPONSORED |
                        EAV_TLD_SPECIAL;
    eav->utf8_cb = NULL;
    eav->ascii_cb = NULL;
    eav->initialized = false;
    eav->errcode = EEAV_NO_ERROR;
}


extern int
eav_setup (eav_t *eav)
{
    if (eav->utf8 || eav->rfc == EAV_RFC_6531) {
        eav->utf8 = true;
        eav->utf8_cb = is_6531_email;
        return init_idn (eav);
    }

    /* ASCII requested */

    /* TODO if (eav->tld_check) */

    eav->utf8 = false;

    switch (eav->rfc) {
    case EAV_RFC_822:   eav->ascii_cb = is_822_email;    break;
    case EAV_RFC_5321:  eav->ascii_cb = is_5321_email;   break;
    case EAV_RFC_5322:  eav->ascii_cb = is_5322_email;   break;
    default:
        return EEAV_INVALID_RFC;
    }

    return EEAV_NO_ERROR;
}


extern void
eav_free (eav_t *eav)
{
    if (eav != NULL && eav->initialized)
        idn_resconf_destroy (eav->idn);

    /* the *eav might be on stack, that's why we don't free it */
}


extern int
eav_is_email (eav_t *eav, const char *email, size_t length)
{
    int rc;
    bool tld_test = false;
    idn_result_t r;


    if (eav->utf8)
        rc = eav->utf8_cb
            (eav->idn, eav->actions, &r, email, length, eav->tld_check);
    else
        rc = eav->ascii_cb(email, length, eav->tld_check);

    if (rc == 0) {
        eav->idnmsg = NULL;
        eav->errcode = EEAV_NO_ERROR;
        return (YES);
    }

    if (rc < 0) {
        eav->errcode = inverse(rc);

        if (eav->errcode == EEAV_IDN_ERROR)
            eav->idnmsg = idn_result_tostring (r);

        return (NO);
    }

    /* user tld preferences */
    switch (rc) {
    case TLD_TYPE_NOT_ASSIGNED:
        eav->errcode = EEAV_TLD_NOT_ASSIGNED;
        tld_test = (eav->allow_tld & EAV_TLD_NOT_ASSIGNED);
        break;
    case TLD_TYPE_COUNTRY_CODE:
        eav->errcode = EEAV_TLD_COUNTRY_CODE;
        tld_test = (eav->allow_tld & EAV_TLD_COUNTRY_CODE);
        break;
    case TLD_TYPE_GENERIC:
        eav->errcode = EEAV_TLD_GENERIC;
        tld_test = (eav->allow_tld & EAV_TLD_GENERIC);
        break;
    case TLD_TYPE_GENERIC_RESTRICTED:
        eav->errcode = EEAV_TLD_GENERIC_RESTRICTED;
        tld_test = (eav->allow_tld & EAV_TLD_GENERIC_RESTRICTED);
        break;
    case TLD_TYPE_INFRASTRUCTURE:
        eav->errcode = EEAV_TLD_INFRASTRUCTURE;
        tld_test = (eav->allow_tld & EAV_TLD_INFRASTRUCTURE);
        break;
    case TLD_TYPE_SPONSORED:
        eav->errcode = EEAV_TLD_SPONSORED;
        tld_test = (eav->allow_tld & EAV_TLD_SPONSORED);
        break;
    case TLD_TYPE_TEST:
        eav->errcode = EEAV_TLD_TEST;
        tld_test = (eav->allow_tld & EAV_TLD_TEST);
        break;
    case TLD_TYPE_SPECIAL:
        eav->errcode = EEAV_TLD_SPECIAL;
        tld_test = (eav->allow_tld & EAV_TLD_SPECIAL);
        break;
    default:
        /* should not happen */
        abort ();
    };

    if (tld_test) {
        eav->idnmsg = NULL;
        eav->errcode = EEAV_NO_ERROR;
        return (YES);
    }

    return (NO);
}


extern const char *
eav_errstr (eav_t *eav)
{
    if (eav->errcode == EEAV_IDN_ERROR)
        return eav->idnmsg;
    else
        return errors[ eav->errcode ];
}
