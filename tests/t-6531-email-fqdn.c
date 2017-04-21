#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <errno.h>
#include <eav.h>
#include "../src/auto_tld.h"
#include "common.h"

typedef int (*utf8_email) ( idn_resconf_t ctx,
                            idn_action_t a,
                            const char *s,
                            const char *e);

static void
init_idn (idn_resconf_t *ctx)
{
    idn_result_t r;


    r = idn_resconf_initialize ();

    if (r != idn_success) {
        msg_warn ("idn_resconf_initialize: %s\n", idn_result_tostring (r));
        exit (EXIT_FAILURE);
    }

    r = idn_resconf_create (ctx);

    if (r != idn_success) {
        msg_warn ("idn_resconf_create: %s\n", idn_result_tostring (r));
        exit (EXIT_FAILURE);
    }
}


extern int
main (int argc, char *argv[])
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    idn_resconf_t ctx;
    idn_action_t actions = IDN_ENCODE_REGIST;
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
    init_idn (&ctx);

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
        r = is_6531_email_fqdn (ctx, actions, line, len);

        if (r > 0 &&
            r != TLD_TYPE_INVALID &&
            r != TLD_TYPE_NOT_ASSIGNED &&
            r != TLD_TYPE_TEST)
        {
            printf ("PASS: %s\n", sanitize_utf8(line, len));
            tld_count[r]++;
            passed++;
        }
        else if (r == 0) {
            printf ("PASS: %s\n", sanitize_utf8(line, len));
            tld_count[r]++;
            passed++;
        }
        else {
            printf ("FAIL: %s\n", sanitize_utf8(line, len));
            failed++;
            if (r > 0)
                tld_count[r]++;
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

    if (line != NULL)
        free (line);
    fclose (fh);
    idn_resconf_destroy (ctx);

    msg_ok ("%s: PASS\n", argv[0]);

    return 0;
}
