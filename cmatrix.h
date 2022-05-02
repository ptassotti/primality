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
