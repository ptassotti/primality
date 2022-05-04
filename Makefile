CFLAGS=-Wall -D MODE=NORMAL -D STACK_STATIC -O3 -I/usr/lib
LDFLAGS=-lgmp -lflint


OBJS=cyclo.o cmatrix.o cpseudo.o smallprimes.o discriminant.o
ALLOBJS=cyclo.o cmatrix.o cpseudo.o smallprimes.o discriminant.o isprime.o


all: isprimemain primelist cyclopseudo


smallprimes: smallprimes.c
	@gcc ${CFLAGS} -c smallprimes.c

discriminant: discriminant.c
	@gcc ${CFLAGS} -c discriminant.c


cpseudo: cpseudo.c
	@gcc ${CFLAGS} -c cpseudo.c

cmatrix: cmatrix.c
	@gcc ${CFLAGS} -c cmatrix.c

cyclo: cyclo.c
	@gcc ${CFLAGS} -c cyclo.c

isprime: isprime.c
	@gcc ${CFLAGS} -c isprime.c


cyclopseudo: cyclo cmatrix cpseudo smallprimes discriminant cyclopseudo.c
	@gcc ${CFLAGS} -o cyclopseudo ${OBJS} cyclopseudo.c ${LDFLAGS}


primelist: cyclo cmatrix cpseudo smallprimes discriminant isprime primelist.c
	@gcc ${CFLAGS} -o primelist ${ALLOBJS} primelist.c ${LDFLAGS}

isprimemain: cyclo cmatrix cpseudo smallprimes discriminant isprime isprimemain.c
	@gcc ${CFLAGS} -o isprime ${ALLOBJS} isprimemain.c ${LDFLAGS}


clean:
	@rm -rf *.o .*.swp .DS_Store isprime primelist cyclopseudo

