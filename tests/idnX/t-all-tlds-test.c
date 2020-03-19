#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <errno.h>
#include <eav.h>
#include <eav/auto_tld.h>
#include "common.h"


/* how many expect TLDs with test type */
#define TEST_CHECK          (11)
/* how many expect not assigned TLDs */
#define NOT_ASSIGNED_CHECK  (49)


extern int
main (int argc, char *argv[])
{
    char *line = NULL;
    size_t len;
    ssize_t read;
    static int tld_count[TLD_TYPE_MAX]; /* zero everything */
    int t;
    int r;
    FILE *fh;


    if (argc >= 3 || argc < 2) {
        msg_warn ("usage: %s FILE\n", argv[0]);
        return 2;
    }

    setlocale(LC_ALL, "");

    fh = fopen (argv[--argc], "r");

    if (fh == NULL) {
        msg_warn ("open: %s: %s", argv[argc], strerror(errno));
        return 3;
    }

    while ((read = getline (&line, &len, fh)) != EOF) {
        line[read-1] = '\0';

        if (line[0] == '#') /* skip comments */
            continue;

        t = is_utf8_domain (&r, line, line + strlen (line), true);

        if (t >= 0 &&
            t != TLD_TYPE_NOT_ASSIGNED &&
            t != TLD_TYPE_TEST &&
            t != TLD_TYPE_SPECIAL)
        {
            printf ("PASS: %s\n", line);
        }
        else
        {
            printf ("FAIL: %s (rv: %d)\n", line, t);
        }

        if (t >= 0)
            tld_count[t]++;
    }

    if (tld_count[TLD_TYPE_TEST] != TEST_CHECK) {
        msg_warn ("%s: expected %d test TLDs, but got %d [%d]\n",
                argv[0],
                TEST_CHECK,
                tld_count[TLD_TYPE_TEST],
                TLD_TYPE_TEST);
        return 5;
    }

    if (tld_count[TLD_TYPE_NOT_ASSIGNED] != NOT_ASSIGNED_CHECK) {
        msg_warn ("%s: expected %d not assigned TLDs, but got %d [%d]\n",
                argv[0],
                NOT_ASSIGNED_CHECK,
                tld_count[TLD_TYPE_NOT_ASSIGNED],
                TLD_TYPE_NOT_ASSIGNED);
        return 6;
    }

    if (line != NULL)
        free (line);
    fclose (fh);


    msg_ok ("%s: PASS\n", argv[0]);

    return 0;
}