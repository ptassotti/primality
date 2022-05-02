/*
 * isprime.c
 */


/* Includes */
#include <stdio.h>
#include "cpseudo.h"

/*#define VERBOSE
*/

int is_prime(mpz_t N, unsigned int verbose)
{
	int ret = 0;
	unsigned int is_pseudo=0;

	/* trivial cases */
	if (mpz_cmp_ui(N, 0) == 0) { return 0; }
	if (mpz_cmp_ui(N, 1) == 0) { return 0; }
	if (mpz_cmp_ui(N, 2) == 0) { return 1; }
	if (mpz_cmp_ui(N, 5) == 0) { return 1; }

	unsigned int i = 1;

	/* initialize primorial upper bound*/
	mpz_t bound;
	mpz_init(bound);
	mpz_set_ui(bound, 1);

	/* N-1 */
	mpz_t N_1;
	mpz_init(N_1);
	mpz_sub_ui(N_1, N, 1);

	do {
		unsigned int ramify = 0;
		unsigned int l=primes[i];

		/* check if l is equal to l */
		if (mpz_cmp_ui(N, l) == 0) { return 1; }

		/* check if N ramifies in O(zeta_l) */
		ramify = cpseudo_ramifies(N, l);

		/* N ramifies ? */
		if (ramify == 1) {

			if ( verbose ) {
				gmp_printf("%Zd ramifies for %d. Skipping.\n", N, l);
			}

			goto next;
		}

		/* ASSERT: having reached this point, N does not ramify */

		if ( verbose ) {
			gmp_printf("Probing %d\n", l);
		}

		ret = cpseudo_fibo(&is_pseudo, N, l);
		if ( ret ) {
			printf("Unexpected error.\n");
			return -1;
		}

		if (!is_pseudo) {  return 0; }

		mpz_mul_ui(bound, bound, l);

	if ( verbose ) {
		gmp_printf("Passed. New bound=%Zd.\n", bound);
	}

next:
		i++;

	} while (i <= MAXPRIMEINDEX && mpz_cmp(bound, N) < 0);

	return 1;
}

