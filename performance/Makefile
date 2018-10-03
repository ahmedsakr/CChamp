OBJECT_FILE=	benchmark
TEST_FILE=		benchmark.c

run: compile
	./${OBJECT_FILE}

compile: ${TEST_FILE}
	gcc -o ${OBJECT_FILE} ${TEST_FILE} -lcchamp -lcurl -lc

clean: ${OBJECT_FILE}
	rm -f $<

.PHONY: run
