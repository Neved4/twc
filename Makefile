.POSIX:

CC     = clang
CFLAGS = -Os -I.
DEPS   = twc.c
OBJ    = twc.o

%.o: %.c ${DEPS}
	${CC} -c -o $@ $< ${CFLAGS}

twc: ${OBJ}
	${CC} -o $@ $^ ${CFLAGS}

clean:
	rm -rf *.o twc
