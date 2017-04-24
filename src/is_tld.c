#include <string.h>
#include <strings.h>
#include <eav.h>
#include "auto_tld.h"
#include "private.h"


extern int
is_tld (const char *start, const char *end)
{
    if (start == end)
        return inverse(EEAV_TLD_INVALID);

    for (const tld_t *tld = tld_list; tld->domain != NULL ; tld++) {
        if (strncasecmp (tld->domain, start, tld->length) == 0)
            return tld->type;
    }

    return inverse(EEAV_TLD_INVALID);
}
