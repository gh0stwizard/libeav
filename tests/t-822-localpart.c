#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <errno.h>
#include <eav.h>
#include "common.h"

/* how many times idnkit should fail */
#define PASSED_CHECK    (24)
#define FAILED_CHECK    (18)


extern int
main (int argc, char *argv[])
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    int r = 0;
    FILE *fh;
    int passed = 0;
    int failed = 0;


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

        len = strlen (line);
        r = is_822_local (line, line + len);

        if (r == 0) {
            printf ("PASS: %s\n", sanitize(line, len));
            passed++;
        }
        else {
            printf ("FAIL: %s\n", sanitize(line, len));
            failed++;
        }
    }

    if (passed != PASSED_CHECK) {
        msg_warn ("%s: expected %d passed checks, but got %d\n",
                argv[0],
                PASSED_CHECK,
                passed);
        return 4;
    }

    if (failed != FAILED_CHECK) {
        msg_warn ("%s: expected %d failed checks, but got %d\n",
                argv[0],
                FAILED_CHECK,
                failed);
        return 4;
    }

    if (line != NULL)
        free (line);
    fclose (fh);
    msg_ok ("%s: PASS\n", argv[0]);
    return 0;
}
