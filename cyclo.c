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
 * cyclo.c
 */

/* Includes */
#include <stdio.h>
#include "cyclo.h"
#include "flint/fmpz.h"
#include "flint/fmpz_poly.h"


/* public functions */

/*
 *  cyclo_init(): initialization function
 */
int cyclo_init(struct cyclo *number, unsigned int size)
{
	/* sanity check */
	if (!number) return -1;

	/* check static bounds */
	if (size > CYCLO_MAX_SIZE || size == 0) return -1;

	/* initialize elements */
	number->size = size;

	unsigned int i;
	for (i = 0; i < size; i++) {
		mpz_init_set_ui(number->values[i], 0);
	}

	return 0;
}


/*
 * cyclo_free(): free memory
 */
int cyclo_free(struct cyclo *number)
{
	/* sanity check */
	if (!number) return -1;

	/* free mem */
	unsigned int i;
	for (i = 0; i < number->size; i++) {
		mpz_clear(number->values[i]);
	}

	return 0;
}


/*
 * cyclo_copy(): copy src into dst
 */
int cyclo_copy(struct cyclo *dst, struct cyclo *src)
{
	/* sanity check */
	if (!src || !dst) return -1;

	/* check compatibility */
	if (src->size != dst->size) return -1;

	/* copy values */
	unsigned int i;
	for (i = 0; i < src->size; i++) {
		mpz_set(dst->values[i], src->values[i]);
	}

	return 0;
}


/*
 *  cyclo_zero(): zero all coordinates
 */
int cyclo_zero(struct cyclo *number)
{
	/* sanity check */
	if (!number) return -1;

	unsigned int i;
	for (i = 0; i < number->size; i++) {
		mpz_set_ui(number->values[i], 0);
	}

	return 0;
}


/*
 *  cyclo_is_zero(): check is this number is zero
 */
int cyclo_is_zero(struct cyclo *number)
{
	/* sanity check */
	if (!number) return -1;

	unsigned int i;
	for (i = 0; i < number->size; i++) {
		if (mpz_cmp_ui(number->values[i], 0) != 0) { return 0; }
	}

	return 1;
}


/*
 *  cyclo_is_equal(): check whether two numbers are equal
 */
int cyclo_is_equal(struct cyclo *n1, struct cyclo *n2)
{
	/* sanity check */
	if (!n1 || !n2) return -1;

	/* check if the two numbers are compatible */
	if (n1->size != n2->size) return -1;

	unsigned int i;
	for (i = 0; i < n1->size; i++) {
		if (mpz_cmp(n1->values[i], n2->values[i]) != 0) { return 0; }
	}

	return 1;
}


/*
 *  cyclo_set_coord(): set the ith coordinate of the algebraic integer
 */
int cyclo_set_coord(struct cyclo *number, mpz_t value, unsigned int i)
{
	/* sanity check */
	if (!number || !value) return -1;

	/* bound check */
	if (i >= number->size) return -1;

	/* set value */
	mpz_set(number->values[i], value);

	return 0;
}


/*
 *  cyclo_mult(): multiply 2 algebraic integers in O(zeta_l) modulo N
 */
int cyclo_mult(
	struct cyclo *result,
	struct cyclo *n1,
	struct cyclo *n2,
	mpz_t N)
{
	unsigned int ret = 0;
	struct cyclo r;

	/* sanity check */
	if (!result || !n1 || !n2 || !N) return -1;

	/* ASSERT: n1->size == n2->size == result->size, checked by the caller */
	unsigned int i, size;
	size = n1->size;

	/* initialize local variables */
	ret = cyclo_init(&r, size);

	if ( ret ) { return -1; }

	fmpz_poly_t n1_poly, n2_poly, r_poly;

	/* allocate polynomials */
	fmpz_poly_init2(n1_poly, size);
	fmpz_poly_init2(n2_poly, size);
	fmpz_poly_init2(r_poly,  size * 2);

	/* convert n1 and n2 into nmod_poly_t */
	fmpz_t ftmp;
	fmpz_init(ftmp);
	for (i = 0; i < size; i++) {

		fmpz_set_mpz(ftmp, n1->values[i]);
		fmpz_poly_set_coeff_fmpz(n1_poly, i, ftmp);

		fmpz_set_mpz(ftmp, n2->values[i]);
		fmpz_poly_set_coeff_fmpz(n2_poly, i, ftmp);
	}

	/* multiply n1_poly and n2_poly */
	fmpz_poly_mul(r_poly, n1_poly, n2_poly);

	/* convert back r_poly into a cyclotomic integer (collapse equivalent powers) */
	mpz_t tmp;
	mpz_init(tmp);
	for (i = 0; i < 2*size; i++) {

		fmpz_poly_get_coeff_fmpz(ftmp, r_poly, i);
		fmpz_get_mpz(tmp, ftmp);
		mpz_add(r.values[i%size], r.values[i%size], tmp);
	}

	/* free mem */
	fmpz_poly_clear(n1_poly);
	fmpz_poly_clear(n2_poly);
	fmpz_poly_clear(r_poly);
	fmpz_clear(ftmp);
	mpz_clear(tmp);

	/* copy result */
	cyclo_copy(result, &r);

	/* free memory */
	cyclo_free(&r);

	return ret;
}


/*
 *  cyclo_mult_by_zeta(): multiply by zeta an algebraic integers in O(zeta_l) modulo N
 */
int cyclo_mult_by_zeta(
	struct cyclo *result,
	struct cyclo *n)
{
	unsigned int ret = 0;

	/* sanity check */
	if (!result || !n) return -1;

	/* ASSERT: n->size == result->size, checked by the caller */
	unsigned int i, size;
	size = n->size;

	for (i = 0; i < size; i++) {
		mpz_set(result->values[ (i+1) % size ], n->values[i]);
	}

	return ret;
}


/*
 *  cyclo_add(): sum two algebraic integers in O(zeta_l) modulo N
 */
int cyclo_add(
	struct cyclo *result,
	struct cyclo *n1,
	struct cyclo *n2,
	mpz_t N)
{
	/* sanity check */
	if (!result || !n1 || !n2 || !N) return -1;

	int ret = 0;
	unsigned int size = n1->size;
	struct cyclo r;

	/* initialize local variables */
	ret = cyclo_init(&r, size);
	if ( ret ) { return -1; }

	/* ASSERT: n1->size == n2->size == result->size, checked by the caller */
	unsigned int i;
	for (i = 0; i < size; i++) {
		mpz_add(r.values[i], n1->values[i], n2->values[i]);
		mpz_mod(r.values[i], r.values[i], N);
	}

	/* copy result */
	cyclo_copy(result, &r);

	/* free mem */
	cyclo_free(&r);

	return ret;
}


int cyclo_print(struct cyclo *number)
{
	/* sanity check */
	if (!number) return -1;

	/* ASSERT: number->size > 0, checked by the initializing function */
	unsigned int size=number->size;
	gmp_printf("(%Zd", number->values[size-1]);

	int i;
	for (i = size - 2; i >= 0; i--) {
		gmp_printf("\t%Zd", number->values[i]);
	}

	printf(")");

	return 0;
}


/*
 *  cyclo_test(): test code for debugging & benchmarking purposes
 */
int cyclo_test()
{
	unsigned int size = 3;
	mpz_t N, one, two;
	struct cyclo n1, n2, r;

	cyclo_init(&n1, size);
	cyclo_init(&n2, size);
	cyclo_init(&r, size);

	mpz_init(N);
	mpz_set_ui(N, 1000);

	mpz_init(one);
	mpz_set_ui(one, 1);

	mpz_init(two);
	mpz_set_ui(two, 2);

	cyclo_set_coord(&n1, one, 0);
	cyclo_set_coord(&n2, two, 1);

	/*cyclo_copy(&r, &n2);*/
	/*cyclo_add(&r, &n1, &n2, N);*/
	cyclo_mult(&r, &n1, &n2, N);

	cyclo_print(&n1);
	printf("\n");

	cyclo_print(&n2);
	printf("\n");

	cyclo_print(&r);
	printf("\n");

	cyclo_free(&n1);
	cyclo_free(&n2);
	cyclo_free(&r);

	mpz_clear(N);

	return 0;
}

