### README

Primality testing using cyclotomic Fibonacci sequences.

__Example:__

./isprime 89 # should indicate prime 

./isprime 77 # should indicate composite


Use "-v" option to enable verbose output for large primes.


To build simply type "make".

The only requirements are the following libraries:
[libgmp][https://gmplib.org/] (Gnu Multiprecision Library)
[libflint][https://flintlib.org/doc/index.html] (Fast Library for Number Theory)


## Other utilities

**primelist:** print a list of the first N prime numbers.

**cyclopseudo:** print the list of the first cyclotomic pseudoprime for the given parameter.

