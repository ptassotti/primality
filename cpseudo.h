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

#ifndef __CPSEUDO_H
#define __CPSEUDO_H


/* Includes */
#include "gmp.h"


/* Constants */
#define MAXPHIDIVISORS	40
#define MAXPRIMEINDEX	563
#define MAXPRIME		4093


/* Structures Declarations */
struct divisors_list {
	unsigned int divisors[MAXPRIME];
	unsigned int count;
};


/* Globals */
extern int			 		prime_to_index[MAXPRIME+1];
extern unsigned int			primes[MAXPRIMEINDEX];
extern struct divisors_list phi_divisors[MAXPRIMEINDEX];


/* Functions Declarations */
int cpseudo_fibo    (unsigned int *, mpz_t, unsigned int, unsigned int);

unsigned int smallest_exp(mpz_t, unsigned int);
unsigned int cpseudo_ramifies(mpz_t, unsigned int);

void print_phi_divisors();
void print_prime_to_index();

int cpseudo_print_list(unsigned int);

int cpseudo_test();

#endif
