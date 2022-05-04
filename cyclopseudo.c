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
 * cyclopseudo.c: print the list of cyclotomic pseudoprimes for a given parameter l
 */


/* Includes */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#include "cpseudo.h"

/* Constants */
#define DEFAULT_OFFSET	100000
#define DEFAULT_MIN		3


/* Utility Functions */
static void usage(const char *message)
{
	if (message[0] != '\0')
		printf("%s\n", message);

	printf(
		"Usage: cyclopseudo l [MIN [MILESTONE]]\n\n\
Print all cyclotomic pseudoprimes for the given parameter l.\n\
Starts at the integer MIN if specified.\n\
If MILESTONE is specified, print a progress status at every MILESTONE.\n\
\n\
Options:\n\
     -h: print this help.\n");

	exit(1);
}


/* Main */
int main(int argc, char **argv)
{
	char c;
	int ret = 0;
	unsigned int l;
	unsigned int offset = DEFAULT_OFFSET;
	int min = DEFAULT_MIN;

	opterr = 0;
	while ( (c = getopt(argc, argv, "h")) != -1) {

		switch (c) {

			case 'h':
				usage("");
				break;

			case '?':
				usage("Unrecognized option.");

		}
	}

	argc -= optind;
	argv += optind;

	/* parse mandatory arguments */
	if (argc == 0) {
		usage("Missing argument.");
	}

	l = strtol(argv[0], (char **) NULL, 10);
	if (l == 0) {
		usage("Bad integer specification.");
	}

	if ( (l < 3)  || ( (l % 2) == 0 ) )
		usage("l must be an odd integer greater or equal to 3.");

	/* parse optional arguments */

	/* min */
	if (argc >= 2) {
		min = strtol(argv[1], (char **) NULL, 10);
		if (min == 0) {
			usage("Bad integer specification.");
		}
	}

	/* offset */
	if (argc >= 3) {
		offset = strtol(argv[2], (char **) NULL, 10);
		if (offset == 0) {
			usage("Bad integer specification.");
		}
	}

	mpz_t N;

	mpz_init(N);

	int i;
	int milestone = (min - (min % offset)) + offset;

	printf("Cyclotomic pseudoprimes for l=%d.\n", l);
	printf("Starting from %d (milestone at every %d, next milestone %d).\n",
			min, offset, milestone);

	for (i = min; /* infinite loop */ ; i=i+2) {

		if (i > milestone) {
			printf("Reached %d.\n", milestone);
			milestone += offset;
		}

		/* N */
		mpz_set_ui(N, i);

		/* if N is not prime */
		if ( mpz_probab_prime_p(N, 50) == 0 ) {

			unsigned int is_pseudo;

			unsigned int ramify;

			ramify = cpseudo_ramifies(N, l);

			/* skip if N ramifies */
			if (ramify != 0) {
				 continue;
			}

			is_pseudo=0;
			ret = cpseudo_fibo(&is_pseudo, N, l, 0);
			if ( ret ) {
				printf("Unexpected error.\n");
				return -1;
			}

			if (is_pseudo) {
				gmp_printf("%Zd\n", N);
			}

		}
	}

	/* free mem */
	mpz_clear(N);

	return 0;
}

