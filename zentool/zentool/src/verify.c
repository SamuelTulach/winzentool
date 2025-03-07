/*
 * Copyright 2025 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>

#include "util.h"
#include "ucode.h"
#include "risc86.h"
#include "crypt.h"
#include "options.h"
#include "win.h"
#include "win_getopt.h"

static const struct option kLongOpts[] = {
    {          "help", false, NULL, 'h' },
    {0},
};

static const char *kOptHelp[] = {
    "print this message",
};

static int print_usage(const char *name)
{
    return print_usage_generic(name, "FILE...", kLongOpts, kOptHelp);
}

int cmd_verify_main(int argc, char **argv)
{
    int longopt;
    int result = 0;
    int c;

    reset_getopt();

    while ((c = getopt_long(argc, argv, "h", kLongOpts, &longopt)) != -1) {
        switch (c) {
            case 'h': print_usage(*argv);
                      return 0;
            case '?': print_usage(*argv);
                      errx(EXIT_FAILURE, "invalid options");
        }
    }

    if (optind == argc) {
        print_usage(*argv);
        errx(EXIT_FAILURE, "must provide at least a filename");
    }

    for (int i = optind; i < argc; i++) {
        patch_t *patch;

        patch = load_patch_file(argv[i]);

        if (patch == NULL) {
            errx(EXIT_FAILURE, "failed to parse file %s", argv[i]);
        }

        logstr("%s: ", argv[i]);

        result |= dump_patch_sig(patch);

        free_patch_file(patch);
    }

    if (result) {
        logerr("use `resign` command to adjust the hash of edited files");
    }

    return result;
}
