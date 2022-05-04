/*
 * Copyright 2022 Paolo Tassotti
 *
 * This file is part of Primality.
 *
 * Primality is free software: you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation, either version
 * 3 of the License, or (at your option) any later version.
 *
 * Primality is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with Primality.
 * If not, see <https://www.gnu.org/licenses/>.
 */


/*
 * isprimemain.c
 */


/* Includes */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include "isprime.h"



/* Utility Functions */
static void usage(const char *message)
{
    if (message[0] != '\0')
        printf("%s\n", message);

    printf(
        "Usage: isprime <number>: check if <number> is prime.\n\
<number> is supposed to be in decimal base.\n\
isprime -h: print this help.\n\
isprime -v: verbose output.\n");

    exit(1);
}


/* Main */
int main(int argc, char **argv)
{
	char c;
	int ret = 0;
	int verbose = 0;
	mpz_t N;

	mpz_init(N);

	opterr = 0;
	while ( (c = getopt(argc, argv, "hv")) != -1) {

	switch (c) {

		case 'h':
			usage("");
			break;

		case 'v':
			verbose=1;
			break;

		case '?':
			usage("Unrecognized option.");

		}
	}

	argc -= optind;
	argv += optind;

	if (argc == 0) {
		usage("Missing argument.");
	} else {

        ret = mpz_set_str(N, argv[0], 10);

        if ( ret ) {
            usage("Bad integer specification.");
        }
    }

	if (is_prime(N, verbose)) {
		gmp_printf("%Zd is prime.\n", N);
		fflush(stdout);
	} else {
		gmp_printf("%Zd is not prime.\n", N);
	}

	return 0;
}

