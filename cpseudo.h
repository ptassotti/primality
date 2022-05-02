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
int cpseudo_fibo    (unsigned int *, mpz_t, unsigned int);

unsigned int smallest_exp(mpz_t, unsigned int);
unsigned int cpseudo_ramifies(mpz_t, unsigned int);

void print_phi_divisors();
void print_prime_to_index();

int cpseudo_print_list(unsigned int);

int cpseudo_test();

#endif
