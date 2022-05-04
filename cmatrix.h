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
 *  cmatrix.h
 */

#ifndef __CMATRIX_H
#define __CMATRIX_H


/* Includes */
#include "gmp.h"
#include "cyclo.h"


/* Structures Declarations */

/*
 * Represents the kth power modulo N of the Q-matrix for Cyclotomic Fibonacci numbers
 *
 *		q12 = q21 = F_k,
 *		q11 = F_(k+1),
 *		q22 = F_(k-1)
 *
 * initial values (k=0):
 *		q11 = zeta
 *		q12 = q21 = 1,
 *		q22 = 0
 *
 */
struct cmatrix {
	unsigned int size;	/* size of the algebraic integers*/
	struct cyclo q11, q12, q21, q22;
};


/* Functions Declarations */

int cmatrix_init(struct cmatrix *, unsigned int);
int cmatrix_init_identity(struct cmatrix *, unsigned int);
int cmatrix_free(struct cmatrix *);

int cmatrix_copy(struct cmatrix *, struct cmatrix *);
int cmatrix_set_identity(struct cmatrix *);

int cmatrix_mult(struct cmatrix *,struct cmatrix *, struct cmatrix *, mpz_t);
int cmatrix_power(struct cmatrix *, struct cmatrix *, mpz_t, mpz_t);

int cmatrix_getvalue_11(struct cyclo *, struct cmatrix *);
int cmatrix_getvalue_12(struct cyclo *, struct cmatrix *);
int cmatrix_getvalue_22(struct cyclo *, struct cmatrix *);
/* get_value21 is useless since the matrix is always symmetric */

int cmatrix_print(struct cmatrix *);

int cmatrix_test();

#endif
