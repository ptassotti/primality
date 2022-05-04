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
 *  cpseudo.h
 */

#ifndef __ISPRIME_H
#define __ISPRIME_H


/* Includes */
#include "gmp.h"
#include "cpseudo.h"


/* Globals */
extern unsigned int primes[MAXPRIMEINDEX];


/* Functions Declarations */
int is_prime(mpz_t, unsigned int);


#endif
