#include <stdio.h>
#include <ctype.h>
#include "is_local.h"
#include "utf8_decode.h"


/* is_6321_local: based on rfc5322 with additional notes:

    3.3. Extended Mailbox Address Syntax

    The definition of <atext> is extended to permit both the RFC 5321
    definition and a UTF-8 string. That string MUST NOT contain any
    of the ASCII graphics or control characters.

 * sub-domain =/ U-label
 * atext =/ UTF8-non-ascii
    ; extend the implicit definition of atext in
    ; RFC 5321, Section 4.1.2, which ultimately points to
    ; the actual definition in RFC 5322, Section 3.2.3
 * qtextSMTP =/ UTF8-non-ascii
 */
extern int
is_6321_local (const char *start, const char *end)
{
    const char *cp;
    int qpair = 0;
    int quote = 0;
    int ch;
    int prev = 0;


    if (start == end)
        return (NO);

    utf8_decode_init (start, end - start);
    while ((ch = utf8_decode_next ()) >= 0) {
        printf("U+%04X (%c)\n", ch, ch);

        /* skip non-ASCII characters */
        if (ch > 0x007f)
            continue;



        /* rfc5322 allows next CTRLs in qtext:
         *    %d1-8 / %d11 / %d12 / %d14-31 / %d127
         * in quoted-pairs:
         *    %d0 / %d1-8 / %d11 / %d12 / %d14-31 / %d127 / LF / CR
         */
        if (ISCNTRL(ch) && !quote && !qpair)
            return (NO);

        if (!quote) {
            switch (ch) {
            case '"': {
                /* quote-strings are allowed at the start
                 * or with preciding '.' only
                 */
                if (! prev || start[prev] == '.')
                    quote = 1;
                else
                    return (NO);
            } break;
            case '.': {
                /* '.' is allowed after an atom and only once */
                if (prev == 0 || start[prev] == '.')
                    return (NO);
            } break;
            /* specials & SPACE are not allowed outside quote-string */
            case '(': case ')': case '<': case '>': case '@':
            case ',': case ';': case ':': case '\\':
            case '[': case ']': case ' ':
                return (NO);
            }
        }
        else if (qpair) /* everything, even control chars */
            qpair = 0;
        else {
            switch (ch) {
            case '"':   quote = 0; break;
            case '\\':  qpair = 1; break;
            /* the next chars are not allowed in qtext: */
            /* 1) they must be in quoted-pair(s). */
            /* 2) either they are permitted right after first DQUOTE
             *    or before the last DQUOTE only.
             */
            case '\n': case '\r': case '\t': case ' ': {
                int ok = 0;

                switch (start[prev]) {
                    case '"':
                    case '\n': case '\r': case '\t': case ' ':
                        ok = 1;
                        break;
                }

                if (! ok && (cp + 1) <= end) {
                    switch (cp[1]) {
                        case '"':
                        case '\n': case '\r': case '\t': case ' ':
                            break;
                        default:
                            return (NO);
                    }
                }
            } break;
            } /* switch (ch) */
        } /* if (!quote) / else */

        prev = utf8_decode_at_byte();
    }

    /* local-part with open quote is not allowed */
    if (quote)
        return (NO);

    return (YES);
}
