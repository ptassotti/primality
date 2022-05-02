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
