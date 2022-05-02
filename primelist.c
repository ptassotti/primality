/*
 * primelist.c
 */


/* Includes */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>


/* Constants */
#define MINVALUE 3
#define MAXVALUE 1000000


int is_prime(mpz_t);

/* Utility Functions */
static void usage(const char *message)
{
	if (message[0] != '\0')
		printf("%s\n", message);

	printf(
		"Usage: primelist [min [max]]: print all odd primes greater or equal to [min] and \
less than [max].\n\
[min] and [max] are supposed to be in decimal base.\n\
[min] defaults to 3 and [max] defaults to 1000000.\n\
primelist -h: print this help.\n");

	exit(1);
}


/* Main */
int main(int argc, char **argv)
{
	char c;
	int ret = 0;

	mpz_t N, max;

	mpz_init(N);
	mpz_init(max);

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

	if (argc == 0) {
		mpz_set_ui(N,   MINVALUE);
		mpz_set_ui(max, MAXVALUE);
	} else {

		ret = mpz_set_str(N, argv[0], 10);

		if ( ret ) {
			usage("Bad integer specification.");
		}

		if (argc >= 2) {
			ret = mpz_set_str(max, argv[1], 10);

			if ( ret ) {
				usage("Bad integer specification.");
			}
		} else {
			mpz_set_ui(max, MAXVALUE);
		}
	}

#ifdef DEBUG
	gmp_printf("min=%Zd, max=%Zd\n", N, max);
#endif

	if (mpz_even_p(N)) {
		mpz_add_ui(N, N, 1);
	}

	do {

		if (is_prime(N)) {
			gmp_printf("%Zd\n", N);
			fflush(stdout);
		}

		mpz_add_ui(N, N, 1);

	} while (mpz_cmp(N, max) < 0);

	return 0;
}

