#ifdef have_config_h
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <gpgme.h>

#define fail_if_err(err)                                    \
    do {                                                    \
        if (err) {                                          \
            fprintf (stderr, "%s:%d: %s: %s\n",             \
                __file__, __line__, gpgme_strsource (err),  \
                gpgme_strerror (err));                      \
            exit (1);                                       \
        }                                                   \
    }                                                       \
    while (0)

void gpgsign(const char *filetobesigned, const char *outputfilename) {
    gpgme_ctx_t ctx;
    gpgme_error_t err;
    gpgme_data_t in, out;
    FILE *outputfile;
    int buf_size = 512;
    char buf[buf_size + 1];
    int ret;
    /* set the gpgme signature mode
        gpgme_sig_mode_normal : signature with data
        gpgme_sig_mode_clear  : clear signed text
        gpgme_sig_mode_detach : detached signature */
    gpgme_sig_mode_t sigmode = gpgme_signers_clear;

    /* begin setup of gpgme */
    gpgme_check_version (null);
    setlocale (lc_all, "");
    gpgme_set_locale (null, lc_ctype, setlocale (lc_ctype, null));
#ifndef have_w32_system
    gpgme_set_locale (null, lc_messages, setlocale (lc_messages, null));
#endif
    /* end setup of gpgme */

    // create the gpgme context
    err = gpgme_new (&ctx);
    // error handling
    fail_if_err (err);

    // set the context to textmode
    gpgme_set_textmode (ctx, 1);
    // enable ascii armor on the context
    gpgme_set_armor (ctx, 1);

    // create a data object pointing to the input file
    err = gpgme_data_new_from_file (&in, filetobesigned, 1);
    // error handling
    fail_if_err (err);

    // create a data object pointing to the out buffer
    err = gpgme_data_new (&out);
    // error handling
    fail_if_err (err);

    // sign the contents of "in" using the defined mode and place it into "out"
    err = gpgme_op_sign (ctx, in, out, sigmode);
    // error handling
    fail_if_err (err);

    // open the output file
    outputfile = fopen (outputfilename, "w+");

    // rewind the "out" data object
    ret = gpgme_data_seek (out, 0, seek_set);
    // error handling
    if (ret)
        fail_if_err (gpgme_err_code_from_errno (errno));

    // read the contents of "out" and place it into buf
    while ((ret = gpgme_data_read (out, buf, buf_size)) > 0) {
        // write the contents of "buf" to "outputfile"
        fwrite (buf, ret, 1, outputfile);
    }

    // error handling
    if (ret < 0)
        fail_if_err (gpgme_err_code_from_errno (errno));

    // close "outputfile"
    fclose(outputfile);
    // release the "in" data object
    gpgme_data_release (in);
    // release the "out" data object
    gpgme_data_release (out);
    // release the context
    gpgme_release (ctx);
}

int 
main (int argc, char **argv) {
    if (argc != 3) {
        printf("usage: gpgsign <input file> <output file>\n");
        exit (1);
    }
    printf("signing %s and placing the result into %s\n", argv[1], argv[2]);
    gpgsign(argv[1], argv[2]);
    return 0;
}
