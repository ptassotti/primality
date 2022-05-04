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
