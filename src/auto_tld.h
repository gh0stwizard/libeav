/* this file was auto-generated */


#ifndef TLD_H
#define TLD_H


typedef struct tld_s {
    const char  *domain;
    size_t      length;
    int         type;
} tld_t;

enum {
    TLD_TYPE_INVALID,
    TLD_TYPE_NOT_ASSIGNED,
    TLD_TYPE_COUNTRY_CODE,
    TLD_TYPE_GENERIC,
    TLD_TYPE_GENERIC_RESTRICTED,
    TLD_TYPE_INFRASTRUCTURE,
    TLD_TYPE_SPONSORED,
    TLD_TYPE_TEST,
    TLD_TYPE_MAX
};

extern const tld_t const tld_list[];

#endif /* TLD_H */

