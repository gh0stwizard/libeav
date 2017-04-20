#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <errno.h>
#include <eav.h>
#include "../src/auto_tld.h"
#include "common.h"


extern int
main (int argc, char *argv[])
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    int tld_count[TLD_TYPE_MAX];
    int r;
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
        msg_warn ("open: %s: %s", file, strerror(errno));
        return 3;
    }

    while ((read = getline (&line, &len, fh)) != EOF) {
        line[read-1] = '\0';

        if (line[0] == '#') /* skip comments */
            continue;

        len = strlen (line);
        r = is_email_rfc5322 (line, len);

        if (r >= 0) {
            printf ("PASS: %s\n", sanitize(line, len));
            tld_count[r]++;
            passed++;
        }
        else {
            printf ("FAIL: %s\n", sanitize(line, len));
            failed++;
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

#if 0
    if (tld_count[TLD_TYPE_TEST] != TEST_CHECK) {
        msg_warn ("expected %d test TLDs, but got %d\n",
                TEST_CHECK,
                tld_count[TLD_TYPE_TEST]);
        return 4;
    }

    if (tld_count[TLD_TYPE_NOT_ASSIGNED] != NOT_ASSIGNED_CHECK) {
        msg_warn ("expected %d not assigned TLDs, but got %d\n",
                NOT_ASSIGNED_CHECK,
                tld_count[TLD_TYPE_NOT_ASSIGNED]);
        return 5;
    }
#endif

    fclose (fh);
    msg_ok ("%s: PASS\n", argv[0]);
    return 0;
}
