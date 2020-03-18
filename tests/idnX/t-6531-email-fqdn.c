#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <errno.h>
#include <eav.h>
#include <eav/auto_tld.h>
#include "common.h"


extern int
main (int argc, char *argv[])
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    int t;
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
        t = is_6531_email (&r, line, len, true);

        if (t >= 0) {
            printf ("PASS: %s\n", sanitize_utf8(line, len));
            passed++;
        }
        else {
            printf ("FAIL: %s\n", sanitize_utf8(line, len));
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

    if (line != NULL)
        free (line);
    fclose (fh);

    msg_ok ("%s: PASS\n", argv[0]);

    return 0;
}
