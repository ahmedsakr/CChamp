OBJECT_FILE=	benchmark
TEST_FILE=		benchmark.c

all: run clean

run: ${OBJECT_FILE}
	./${OBJECT_FILE}

${OBJECT_FILE}: ${TEST_FILE}
	gcc -o ${OBJECT_FILE} ${TEST_FILE} -lcchamp -lcurl -lc

clean: ${OBJECT_FILE}
	rm -f $<

${TEST_FILE}:
	echo "${TEST_FILE} not present."
	@exit 1

.PHONY: run
