TARGET = min-noloop
FILE = ${TARGET}.c
TEST_FILE = ${TARGET}-test.c
TEST_HEADER = \#define TEST
FC = $(shell cat ${FILE})
FC := ${TEST_HEADER}\n${FC}
CC = clang

test: ${TEST_FILE}
	@${CC} -I /usr/local/google/home/sunghol/Documents/repo/klee/include ${TEST_FILE}
	@rm ${TEST_FILE}

%-test.c: ${FILE}
	@printf "${TEST_HEADER}\n" > ${TEST_FILE}
	@cat ${FILE} >> ${TEST_FILE}

klee: ${FILE}
	@${CC} -I /usr/local/google/home/sunghol/Documents/repo/klee/include -emit-llvm -c -g -O0 \
  -Xclang -disable-O0-optnone ${FILE}

clean:
	@rm -rf klee* \
	  ${TARGET}.bc a.out

.PHONY: test klee clean
