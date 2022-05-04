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
 * cmatrix.c
 */

/* Includes */
#include <stdio.h>
#include "cmatrix.h"


/* public functions */

/*
 *  cmatrix_init(): initialization function
 */
int cmatrix_init(struct cmatrix *matrix, unsigned int size)
{
	/* sanity check */
	if (!matrix) return -1;

	/* initialize matrix elements */
	matrix->size = size;
	cyclo_init(&(matrix->q11), size);
	cyclo_init(&(matrix->q12), size);
	cyclo_init(&(matrix->q21), size);
	cyclo_init(&(matrix->q22), size);

	/* local variables */
	mpz_t one;

	mpz_init_set_ui(one, 1);

	/* set initial values */
	cyclo_set_coord(&(matrix->q11), one, 1);
	cyclo_set_coord(&(matrix->q12), one, 0);
	cyclo_set_coord(&(matrix->q21), one, 0);

	/* free mem */
	mpz_clear(one);

	return 0;
}


/*
 *  cmatrix_init_identity(): create an identity matrix
 */
int cmatrix_init_identity(struct cmatrix *matrix, unsigned int size)
{
	/* sanity check */
	if (!matrix) return -1;

	/* initialize matrix elements */
	matrix->size = size;
	cyclo_init(&(matrix->q11), size);
	cyclo_init(&(matrix->q12), size);
	cyclo_init(&(matrix->q21), size);
	cyclo_init(&(matrix->q22), size);

	/* local variables */
	mpz_t one;

	mpz_init_set_ui(one, 1);

	/* set initial values */
	cyclo_set_coord(&(matrix->q11), one, 0);
	cyclo_set_coord(&(matrix->q22), one, 0);

	/* free mem */
	mpz_clear(one);

	return 0;
}


/*
 * cmatrix_free(): free memory
 */
int cmatrix_free(struct cmatrix *matrix)
{
	/* sanity check */
	if (!matrix) return -1;

	/* free mem */
	cyclo_free(&(matrix->q11));
	cyclo_free(&(matrix->q12));
	cyclo_free(&(matrix->q21));
	cyclo_free(&(matrix->q22));

	return 0;
}


/*
 *  cmatrix_copy(): cloning function
 */
int cmatrix_copy(struct cmatrix *dst, struct cmatrix *src)
{
	/* sanity check */
	if (!dst || !src) return -1;

	/* copy values */
	dst->size = src->size;
	cyclo_copy(&(dst->q11), &(src->q11));
	cyclo_copy(&(dst->q12), &(src->q12));
	cyclo_copy(&(dst->q21), &(src->q21));
	cyclo_copy(&(dst->q22), &(src->q22));

	return 0;
}


/*
 *  cmatrix_set_identity(): set identity matrix
 */
int cmatrix_set_identity(struct cmatrix *matrix)
{
	/* sanity check */
	if (!matrix) return -1;

	/* zero on the secondary diagonal */
	cyclo_zero(&(matrix->q12));
	cyclo_zero(&(matrix->q21));
	cyclo_zero(&(matrix->q11));
	cyclo_zero(&(matrix->q22));

	/* local variables */
	mpz_t one;

	mpz_init_set_ui(one, 1);

	/* one on the main diagonal */
	cyclo_set_coord(&(matrix->q11), one, 0);
	cyclo_set_coord(&(matrix->q22), one, 0);

	/* free mem */
	mpz_clear(one);

	return 0;
}


/*
 *  cmatrix_mult(): standard 2x2 matrix multiplication modulo N
 */
int cmatrix_mult(
	struct cmatrix *result,
	struct cmatrix *m1,
	struct cmatrix *m2,
	mpz_t N)
{
	struct cyclo tmp;
	struct cyclo r12, r22;

	/* sanity check */
	if (!result || !m1 || !m2 || !N) return -1;

	/* ASSERT: m1->size == m2->size, checked by the caller */
	unsigned int size = m1->size;

	/* initialize local variable */
	cyclo_init(&tmp, size);
	cyclo_init(&r12, size);
	cyclo_init(&r22, size);

	/* element (1,2) */
	cyclo_mult(&r12, &(m1->q11), &(m2->q12), N);
	cyclo_mult(&tmp, &(m1->q12), &(m2->q22), N);
	cyclo_add(&r12, &r12, &tmp, N);

	/* element (2,2) */
	cyclo_mult(&r22, &(m1->q21), &(m2->q12), N);
	cyclo_mult(&tmp, &(m1->q22), &(m2->q22), N);
	cyclo_add(&r22, &r22, &tmp, N);

	/* assign values */
	cyclo_copy(&(result->q12), &r12);
	cyclo_copy(&(result->q22), &r22);
	cyclo_copy(&(result->q21), &(result->q12));	/* the matrix is symmetric */

	/* compute r11 by the recurrence's rule without additional multiplications */
	/* q11 = zeta * q12 + q22 */
	cyclo_mult_by_zeta(&tmp, &(result->q12));
	cyclo_add(&(result->q11), &tmp, &(result->q22), N);

	/* free memory */
	cyclo_free(&tmp);
	cyclo_free(&r12);
	cyclo_free(&r22);

	return 0;
}


/*
 *  cmatrix_power(): calculates the kth power modulo N of given matrix
 */
int cmatrix_power(
	struct cmatrix *result,
	struct cmatrix *matrix,
	mpz_t k,
	mpz_t N)
{
	mpz_t exp;
	struct cmatrix tmp, power;

	/* sanity check */
	if (!result || !matrix || !k || !N) return -1;

	unsigned int size = matrix->size;

	if (mpz_cmp_ui(k, 0) == 0) {
		cmatrix_init_identity(result, size);
		return 0;
	}

	mpz_init(exp);
	mpz_set(exp, k);

	cmatrix_init_identity(&tmp, size);
	cmatrix_init(&power, size);

	cmatrix_copy(&power, matrix);

    while ( mpz_cmp_ui(exp, 0) > 0 ) {	/* k > 0 */

		if (!mpz_divisible_ui_p(exp, 2)) {		/* if k is odd */
			cmatrix_mult(&tmp, &tmp, &power, N);
		}

		cmatrix_mult(&power, &power, &power, N);

		/* exp = exp / 2 */
		mpz_tdiv_q_2exp(exp, exp, 1);
	}
 
	/* store result matrix */
	cmatrix_copy(result, &tmp);

	/* free mem */
	mpz_clear(exp);
	cmatrix_free(&tmp);
	cmatrix_free(&power);

	return 0;
}


int cmatrix_getvalue_11(struct cyclo *value, struct cmatrix *matrix)
{
	/* sanity check */
	if (!matrix || !value) return -1;

	cyclo_copy(value, &(matrix->q11));

	return 0;
}


int cmatrix_getvalue_12(struct cyclo *value, struct cmatrix *matrix)
{
	/* sanity check */
	if (!matrix || !value) return -1;

	cyclo_copy(value, &(matrix->q12));

	return 0;
}


int cmatrix_getvalue_22(struct cyclo *value, struct cmatrix *matrix)
{
	/* sanity check */
	if (!matrix || !value) return -1;

	cyclo_copy(value, &(matrix->q22));

	return 0;
}


int cmatrix_print(struct cmatrix *matrix)
{
	/* sanity check */
	if (!matrix) return -1;

	printf("[ ");
	cyclo_print(&(matrix->q11));
	printf(", ");
	cyclo_print(&(matrix->q12));
	printf(" ]\n[ ");
	cyclo_print(&(matrix->q21));
	printf(", ");
	cyclo_print(&(matrix->q22));
	printf(" ]\n");

	return 0;
}


/*
 *  cmatrix_test(): test code for debugging & benchmarking purposes
 */
int cmatrix_test()
{
	unsigned int size = 3;
	mpz_t exp, N;
	struct cmatrix m1, m2, r;

	cmatrix_init(&m1, size);
	cmatrix_init(&m2, size);
	cmatrix_init(&r, size);

	mpz_init(exp);
	mpz_set_ui(exp, 4);

	mpz_init(N);
	mpz_set_ui(N, 110);


/*	cmatrix_mult(&r, &m1, &m1);
*/

/*	cmatrix_power(&r, &m1, k, N);
*/

	printf("q-matrix:\n");
	cmatrix_print(&m1);
	printf("\n");

	printf("identity:\n");
	cmatrix_set_identity(&m2);
	cmatrix_print(&m2);
	printf("\n");

	printf("copy of q-matrix:\n");
	cmatrix_copy(&r, &m1);
	cmatrix_print(&r);
	printf("\n");

	printf("square of q-matrix:\n");
	cmatrix_mult(&r, &m1, &m1, N);
	cmatrix_print(&r);
	printf("\n");
/*
	printf("third power of q-matrix:\n");
	cmatrix_mult(&r, &r, &m1, N);
	cmatrix_print(&r);
	printf("\n");

	printf("fourth power of q-matrix:\n");
	cmatrix_power(&r, &m1, exp, N);
	cmatrix_print(&r);
	printf("\n");
*/
	printf("fourth power of q-matrix:\n");
	cmatrix_mult(&r, &r, &r, N);
	cmatrix_print(&r);
	printf("\n");


	/* free mem */
	cmatrix_free(&m1);
	cmatrix_free(&m2);
	cmatrix_free(&r);

	mpz_clear(exp);
	mpz_clear(N);

	return 0;
}

