#include <stdio.h>
#include <stdlib.h>
#include <eav.h>
#include "private.h"
#include "auto_tld.h"


static int
init_idn (eav_t *eav)
{
    idn_result_t r;


    r = idn_resconf_initialize ();

    if (r != idn_success) {
        eav->errmsg = idn_result_tostring (r);
        return inverse(EEAV_IDN_INIT_FAIL);
    }

    r = idn_resconf_create (&(eav->idn));

    if (r != idn_success) {
        eav->errmsg = idn_result_tostring (r);
        return inverse(EEAV_IND_CONF_FAIL);
    }

    return EEAV_NO_ERROR;
}


extern void
eav_init (eav_t *eav)
{
    eav->actions = IDN_ENCODE_REGIST;
    eav->utf8 = true;
    eav->tld_check = true;
    eav->rfc = EAV_RFC_6321;
    eav->tld_check = true;
    eav->allow_tld =    EAV_TLD_COUNTRY_CODE |
                        EAV_TLD_GENERIC |
                        EAV_TLD_GENERIC_RESTRICTED |
                        EAV_TLD_INFRASTRUCTURE |
                        EAV_TLD_SPONSORED;
    eav->utf8_cb = NULL;
    eav->ascii_cb = NULL;
}


extern int
eav_setup (eav_t *eav)
{
    if (eav->utf8 || eav->rfc == EAV_RFC_6321) {
        if (eav->tld_check)
            eav->utf8_cb = is_email_rfc6321_fqdn;
        else
            eav->utf8_cb = is_email_rfc6321;

        /* always set */
        eav->utf8 = true;

        return init_idn (eav);
    }

    /* ASCII requested */

    /* TODO if (eav->tld_check) */

    eav->utf8 = false;

    switch (eav->rfc) {
    case EAV_RFC_822:   eav->ascii_cb = is_email_rfc822;    break;
    case EAV_RFC_5321:  eav->ascii_cb = is_email_rfc5321;   break;
    case EAV_RFC_5322:  eav->ascii_cb = is_email_rfc5322;   break;
    default:
        return EEAV_INVALID_RFC;
    }

    return EEAV_NO_ERROR;
}


extern void
eav_free (eav_t *eav)
{
    if (eav != NULL) {
        idn_resconf_destroy (eav->idn);
/*        free (eav);*/
    }
}


extern int
eav_is_email (eav_t *eav, const char *email, size_t length)
{
    int r;


    if (eav->utf8)
        r = eav->utf8_cb(eav->idn, eav->actions, email, length);
    else
        r = eav->ascii_cb(email, length);

    if (r == 0) {
        eav->errmsg = NULL;
        return (YES);
    }

    if (r < 0) {
        switch (r) {
        case inverse(EEAV_NO_ERROR):
            eav->errmsg = "no error";
            break;
        case inverse(EEAV_EMPTY):
            eav->errmsg = "empty email address";
            break;
        case inverse(EEAV_IDNKIT_ERROR):
            /* TODO handle idnkit errors properly */
            eav->errmsg = "idnkit error";
            break;
        case inverse(EEAV_NOT_ASCII):
            eav->errmsg = "email contains non-ascii characters";
            break;
        case inverse(EEAV_SPECIAL):
            eav->errmsg = "email contains special characters";
            break;
        case inverse(EEAV_CTRL_CHAR):
            eav->errmsg = "email contains control characters";
            break;
        case inverse(EEAV_LPART_MISPLACED_QUOTE):
            eav->errmsg = "misplaced quote found";
            break;
        case inverse(EEAV_LPART_SPECIAL):
            eav->errmsg = "local-part contains special characters";
            break;
        case inverse(EEAV_LPART_UNQUOTED):
            eav->errmsg = "local-part contains no pair quote";
            break;
        case inverse(EEAV_LPART_TOO_MANY_DOTS):
            eav->errmsg = "local-part contains too many dots";
            break;
        case inverse(EEAV_LPART_UNQUOTED_FWS):
            eav->errmsg = "local-part contains unquoted white-space characters";
            break;
        case inverse(EEAV_LPART_INVALID_UTF8):
            eav->errmsg = "local-part contains invalid utf-8 data";
            break;
        case inverse(EEAV_DOMAIN_LABEL_TOO_LONG):
            eav->errmsg = "domain label is too long";
            break;
        case inverse(EEAV_DOMAIN_MISPLACED_DELIMITER):
            eav->errmsg = "ip-addr contans misplaced delimiter";
            break;
        case inverse(EEAV_DOMAIN_MISPLACED_HYPHEN):
            eav->errmsg = "domain contains misplaced hyphen";
            break;
        case inverse(EEAV_DOMAIN_INVALID_CHAR):
            eav->errmsg = "domain contains invalid characters";
            break;
        case inverse(EEAV_DOMAIN_TOO_LONG):
            eav->errmsg = "domain length is too long";
            break;
        case inverse(EEAV_DOMAIN_NUMERIC):
            eav->errmsg = "numeric-only domain";
            break;
        case inverse(EEAV_DOMAIN_NOT_FQDN):
            eav->errmsg = "domain is not fqdn";
            break;
        case inverse(EEAV_EMAIL_HAS_NO_DOMAIN):
            eav->errmsg = "email has no domain part";
            break;
        case inverse(EEAV_LPART_TOO_LONG):
            eav->errmsg = "local-part is too long";
            break;
        case inverse(EEAV_EMAIL_INVALID_IPADDR):
            eav->errmsg = "invalid ip address";
            break;
        case inverse(EEAV_EMAIL_NO_PAIRED_BRACKET):
            eav->errmsg = "no pair bracket";
            break;
        default:
#ifdef _DEBUG
            fprintf (stderr, "!!! unknown error #%d\n", r);
#endif
            eav->errmsg = "unknown error!";
            break;
        };

        return (NO);
    }

    /* user tld preferences */
    switch (r) {
    case TLD_TYPE_INVALID:
        eav->errmsg = "invalid TLD";
        return (NO);
    case TLD_TYPE_NOT_ASSIGNED:
        eav->errmsg = "not assigned TLD";
        return (eav->allow_tld & EAV_TLD_NOT_ASSIGNED);
    case TLD_TYPE_COUNTRY_CODE:
        eav->errmsg = "country-code TLD";
        return (eav->allow_tld & EAV_TLD_COUNTRY_CODE);
    case TLD_TYPE_GENERIC:
        eav->errmsg = "generic TLD";
        return (eav->allow_tld & EAV_TLD_GENERIC);
    case TLD_TYPE_GENERIC_RESTRICTED:
        eav->errmsg = "generic-restricted TLD";
        return (eav->allow_tld & EAV_TLD_GENERIC_RESTRICTED);
    case TLD_TYPE_INFRASTRUCTURE:
        eav->errmsg = "infrastructure TLD";
        return (eav->allow_tld & EAV_TLD_INFRASTRUCTURE);
    case TLD_TYPE_SPONSORED:
        eav->errmsg = "sponsored TLD";
        return (eav->allow_tld & EAV_TLD_SPONSORED);
    case TLD_TYPE_TEST:
        eav->errmsg = "test TLD";
        return (eav->allow_tld & EAV_TLD_TEST);
    default:
        eav->errmsg = "unknown TLD";
        return (NO);
    };
}


extern const char *
eav_errstr (eav_t *eav)
{
    return eav->errmsg;
}
