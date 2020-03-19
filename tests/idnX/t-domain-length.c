#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <errno.h>
#include <eav.h>
#include <eav/auto_tld.h>
#include "common.h"
#include <idn2.h>


/* how many times idn library should fail */
#define IDN_ERRORS    (3)


extern int
main (int argc, char *argv[])
{
    char *line = NULL;
    size_t len;
    ssize_t read;
    static int error_count[EEAV_MAX]; /* zero everything */
    int t;
    int r = 0;
    FILE *fh;
    char *file = NULL;
    int expect_pass = -1;
    int expect_fail = -1;
    int passed = 0;
    int failed = 0;


    if (argc >= 5 || argc < 4) {
        msg_warn ("usage: %s PASS_COUNT FAIL_COUNT FILE\n", argv[0]);
        return 2;
    }

    setlocale(LC_ALL, "");

    file = argv[3];
    expect_pass = atoi (argv[1]);
    expect_fail = atoi (argv[2]);

    fh = fopen (file, "r");

    if (fh == NULL) {
        msg_warn ("error: open %s: %s", file, strerror(errno));
        return 3;
    }

    while ((read = getline (&line, &len, fh)) != EOF) {
        line[read-1] = '\0';

        if (line[0] == '#') /* skip comments */
            continue;

        r = 0; /* reset */
        t = is_utf8_domain (&r, line, line + strlen (line), false);

        if (t >= 0) {
            if (t != TLD_TYPE_NOT_ASSIGNED && t != TLD_TYPE_TEST) {
                printf ("PASS: %s\n", line);
                passed++;
            }
            else {
                printf ("FAIL: %s\n", line);
                failed++;
                printf ("\t t = %d; r = %d; idnerr = %s\n",
                        t, r, idn2_strerror (r));
            }
        }
        else {
            printf ("FAIL: %s\n", line);
            error_count[-1 * t]++;
            failed++;
            printf ("\t t = %d; r = %d; idnerr = %s\n",
                    t, r, idn2_strerror (r));
        }
    }

    if (passed != expect_pass) {
        msg_warn ("%s: expected %d passed checks, but got %d\n",
                argv[0],
                expect_pass,
                passed);
        return 4;
    }

    if (failed != expect_fail) {
        msg_warn ("%s: expected %d failed checks, but got %d\n",
                argv[0],
                expect_fail,
                failed);
        return 5;
    }

    if (error_count[EEAV_IDN_ERROR] != IDN_ERRORS) {
        msg_warn ("%s: expected %d idn error(s), but got %d\n",
                argv[0],
                IDN_ERRORS,
                error_count[EEAV_IDN_ERROR]);
        return 6;
    }

#ifdef _DEBUG
    for (int i = 0; i < EEAV_MAX; i++) {
        printf ("error #%d: count = %d\n", i, error_count[i]);
    }
#endif

    if (line != NULL)
        free (line);

    fclose (fh);

    msg_ok ("%s: PASS\n", argv[0]);

    return 0;
}

