/*
 * cpseudo.c:
 */

/* Includes */
#include <stdio.h>
#include "cmatrix.h"
#include "cpseudo.h"

/*#define DEBUG	1
*/
/* Global variables */
extern unsigned int primes[MAXPRIMEINDEX];
extern struct divisors_list phi_divisors[MAXPRIMEINDEX];
extern int prime_to_index[MAXPRIME+1];
extern char *discriminants[MAXPRIMEINDEX];


/* public functions */


/*
 * smallest_exp(): find the smallest exponent "f" such that N^f \equiv 1 mod(N)
 */
unsigned int smallest_exp(mpz_t N, unsigned int l)
{
	unsigned int i;
	unsigned int f;
	unsigned int index;

	mpz_t Nexp, l_mpz;

	/* initialize local variables */
	mpz_init(Nexp);
	mpz_init(l_mpz);

	/* ASSERT: l is supposed to be a prime number, checked by the callee */
	f = l-1;

	mpz_set_ui(l_mpz, l);

	index = prime_to_index[l];

	/* ASSERT: we don't check if index is well-formed */

	for (i = 1; i < phi_divisors[index].count; i++) {

		unsigned int exp = phi_divisors[index].divisors[i];

		/* N^exp (mod N) */
		mpz_powm_ui(Nexp, N, exp, l_mpz);

		/* is N^exp \equiv 1 (mod N) ? */
		if ( mpz_cmp_ui(Nexp, 1) == 0 ) {
			f = exp;
		}
	}

#ifdef DEBUG
		gmp_printf("N=%Zd, l=%d, exp=%d, Nexp=%Zd.\n", N, l, f, Nexp);
#endif

	/* free mem */
	mpz_clear(Nexp);
	mpz_clear(l_mpz);

	return f;
}


/*
 * cpseudo_ramifies(N, l): test if N ramifies in O(zeta_l)
 *
 * where zeta_l is the lth root of unity
 *
 * return: 1 if N ramify, 0 otherwise
 */
unsigned int cpseudo_ramifies(mpz_t N, unsigned int l)
{
	/* sanity check */
	if ( !N ) return -1;

	/* ASSERT: l is a prime number, checked by the caller */

	mpz_t disc, gcd;
	int ret = 0;
	unsigned int index = prime_to_index[l];

	/* ASSERT: we don't check if index is well-formed */

	mpz_init(disc);
	mpz_init(gcd);

	mpz_set_str(disc, discriminants[index], 10);

#ifdef DEBUG
	gmp_printf("check: N=%Zd, disc=%Zd, l=%d, index=%d.\n", N, disc, l, index);
#endif

	mpz_gcd(gcd, N, disc);
	if (mpz_cmp_ui(gcd, 1) > 0) {
		ret = 1;
	}

	/* free mem */
	mpz_clear(disc);
	mpz_clear(gcd);

	return ret;
}


/*
 *  cpseudo_fibo(): test if N is a l-Cyclotomic Fibonacci pseudoprime
 *					where l is a prime number
 *
 *                    U_{N^2(l-1)-1} == 0 (mod N)
 *
 */
int cpseudo_fibo(unsigned int *result, mpz_t N, unsigned int l)
{
	/* sanity check */
	if (!result || !N) return -1;

	/* ASSERT: l is a prime number, checked by the caller */
#ifdef DEBUG
	/* check if l is prime */
	mpz_t L;
	mpz_init(L);
	mpz_set_ui(L, l);
	if (mpz_probab_prime_p(L, 15) == 0) {
		return -1;
	}
	mpz_clear(L);
#endif

	/* ASSERT: N does not ramifies in O(zeta_l), checked by the caller */

	*result=0;

	int ret = 0;

	unsigned int exp, f;

	f = smallest_exp(N, l);
	exp = 2 * f;

	mpz_t N_exp;
	struct cmatrix matrix_N;

	mpz_init(N_exp);

	mpz_pow_ui(N_exp, N, exp);

	ret = cmatrix_init(&matrix_N, l);
	if ( ret ) { return -1; }

	ret = cmatrix_power(&matrix_N, &matrix_N, N_exp, N);
	if ( ret ) { return -1; }

	struct cyclo U_N_m1;

	ret = cyclo_init(&U_N_m1, l);
	if ( ret ) { return -1; }

	ret = cmatrix_getvalue_22(&U_N_m1, &matrix_N);
	if ( ret ) { return -1; }

#ifdef DEBUG
	gmp_printf("N=%Zd, exp=%d, N_exp=%Zd.\n", N, exp, N_exp);
	cmatrix_print(&matrix_N);
/*	cyclo_print(&U_N_m1);*/
#endif

	if (cyclo_is_zero(&U_N_m1) == 1) {
		*result = 1;
	}

	cyclo_free(&U_N_m1);	/* free mem */

	return ret;
}


/*
 *  cpseudo_test(): test code for debugging & benchmarking purposes
 */
int cpseudo_test()
{
	unsigned int result = 0;
	unsigned int l = 3;
	mpz_t N;

	mpz_init(N);
	mpz_set_ui(N, 11);

	cpseudo_fibo(&result, N, l);

	mpz_clear(N);

	return 0;
}

