/*
 *  cyclo.h
 */

#ifndef __CYCLO_H
#define __CYCLO_H


/* Includes */
#include "gmp.h"


/* Constants */
#define CYCLO_MAX_SIZE	4096


/* Structures Declarations */

/*
 * Represents an algebraic integer in the cyclotomic ring O(zeta_l)
 *
 */
struct cyclo {
	unsigned int size;
	mpz_t values[CYCLO_MAX_SIZE];
};


/* Functions Declarations */

int cyclo_init(struct cyclo *, unsigned int);
int cyclo_free(struct cyclo *);

int cyclo_copy(struct cyclo *, struct cyclo *);
int cyclo_zero(struct cyclo *);

int cyclo_is_zero(struct cyclo *);
int cyclo_is_equal(struct cyclo *, struct cyclo *);

int cyclo_set_coord(struct cyclo *, mpz_t, unsigned int);

int cyclo_add (struct cyclo *, struct cyclo *, struct cyclo *, mpz_t);
int cyclo_mult(struct cyclo *, struct cyclo *, struct cyclo *, mpz_t);

int cyclo_mult_by_zeta(struct cyclo *, struct cyclo *n);

int cyclo_print(struct cyclo *);

int cyclo_test();

#endif
