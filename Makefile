####################################################################
## Set up project root
####################################################################
CWD 	:= $(shell echo $$(pwd))

## ROOT DIRS
HDR 	:= ${CWD}/hdr
SRC 	:= ${CWD}/src
LIB 	:= ${CWD}/lib

TESTS 	:= ${CWD}/tests
BUILD 	:= ${CWD}/build

## Source files 
SRC_APP := ${SRC}/app

####################################################################
## Output
####################################################################
## lib objects
BUILD_LIB 	:= ${BUILD}/lib
LIB_OBJECTS 	:= actions.o
LIB_OBJECTS 	:= ${addprefix ${BUILD_LIB}/, ${LIB_OBJECTS}}

## main app dir
BUILD_APP	:= ${BUILD}/src

## tests objectsV
BUILD_TESTS 	:= ${BUILD}/tests
TEST_OBJECTS	:= test_query_statements.o
TEST_OBJECTS 	:= ${addprefix ${BUILD_TESTS}/, ${TEST_OBJECTS}}

####################################################################
## Compiler flags
####################################################################
INCLUDE 	:= -I${HDR}
CXX 		:= gcc
COMPILER_FLAGS 	:= -g -O2 \
		   -Wall -Wextra -Wpedantic -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough \
		   -Werror=format-security \
		   -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 \
		   -D_GLIBCXX_ASSERTIONS \
		   -fstrict-flex-arrays=3 \
		   -fstack-clash-protection -fstack-protector-strong \
		   -Wl,-z,nodlopen -Wl,-z,noexecstack \
		   -Wl,-z,relro -Wl,-z,now \
		   ${INCLUDE} \

LINKER_FLAGS	:=

.PHONY: app
all: ${BUILD}/app

${BUILD}/app: ${BUILD_APP}/app.o ${LIB_OBJECTS}
	${CXX} ${LINKER_FLAGS} -o $@ $^

## build main.c
${BUILD_APP}/%.o: ${SRC_APP}.c | setup lib
	${CXX} ${COMPILER_FLAGS} -o $@ -c $^

.PHONY: test
test: ${BUILD}/run_tests

## link runner and objects
${BUILD}/run_tests: ${BUILD_TESTS}/run_tests.o ${TEST_OBJECTS} ${LIB_OBJECTS}
	${CXX} ${LINKER_FLAGS} -o $@ $^

## build tests and runner
${BUILD_TESTS}/%.o: ${TESTS}/%.c | setup lib
	${CXX} ${COMPILER_FLAGS} -I${TESTS} -o $@ -c $^

# build all lib source files.
.PHONY: lib
${BUILD_LIB}/%.o: ${LIB}/%.c | setup
	${CXX} ${COMPILER_FLAGS} -o $@ -c $^

.PHONY: setup
setup:
	$(shell mkdir -p ${BUILD_APP} ${BUILD_LIB} ${BUILD_TESTS}) 

####################################################################
## Cleanup
####################################################################
.PHONY: clean
clean:
	rm -r ${BUILD}
