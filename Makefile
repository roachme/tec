CC=gcc
MAKE=make
BUILD_TYPE ?= user
PROGRAM=build/_tec
DEPS=$(wildcard lib/*.h cli/*.h cli/aux/*.h)
SRCS=$(wildcard lib/*.c cli/*.c cli/aux/*.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))
LFLAGS=-lconfig
CFLAGS=-I cli -Wall -fbounds-check -Wpedantic -Wextra

#CXXFLAGS += -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align \
#            -Wunused -Woverloaded-virtual -Wpedantic -Wconversion \
#            -Wsign-conversion -Wmisleading-indentation -Wduplicated-cond \
#            -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast


VERSION=$(shell cat VERSION)
README=README.md
SHELLSCRIPT=shell/tec.sh
SHELLNAME=bash
PWDFILE=/tmp/tecpwd



all: user
.PHONY: user release debug




init:
	mkdir -p build
	mkdir -p build/debug
	mkdir -p build/release
	mkdir -p build/docs
	mkdir -p build/reports
	mkdir -p build/cppcheck
	mkdir -p build/doxygen
	mkdir -p build/flawfinder
	mkdir -p build/ldoc
	mkdir -p build/luacheck
	#ln -s ../build/gcov gcov ||:
	#ln -s ../build/gtest gtest ||:

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) -DVERSION=\"$(VERSION)\" -DPWDFILE=\"$(PWDFILE)\"

$(PROGRAM): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

generate: $(PROGRAM)
	$(shell m4 -DSHELLNAME=$(SHELLNAME) -DPWDFILE=$(PWDFILE) ./utils/genshell > $(SHELLSCRIPT))
	$(shell m4 ./utils/genreadme > $(README))

clean:
	rm -rf build
	rm -rf $(PROGRAM) $(OBJS)

re: clean init $(PROGRAM)

check:
	cppcheck --suppress=variableScope --suppress=missingIncludeSystem --std=c89 \
		--enable=all --language=c --check-level=exhaustive $(SRCS) \
		--checkers-report=build/cppcheck/report.txt

install:
	./utils/install full

install-link:
	./utils/install soft

style:
	find . -name '*.[ch]' | xargs indent -nut -nbad -bap -nbc -bbo -hnl -br -brs -c33 -cd33 -ncdb -ce -ci4 -cli0 -d0 -di1 -nfc1 -i4 -ip0 -l80 -lp -npcs -nprs -npsl -sai -saf -saw -ncs -nsc -sob -nfca -cp33 -ss -ts8 -il1
	find . -name '*.[ch]\~' | xargs	rm

memcheck:
	./tests/memleak/memcheck

# Behave BDD Tests
.PHONY: test behave test_bdd

behave: test_bdd
test_bdd: user
	@echo "Running Behave BDD tests..."
	@cd tests/usage && behave --format progress --tags=-skip
	@echo "All BDD tests passed!"

test: memcheck test_bdd
	@echo "All tests completed successfully!"


#all_release: clean init check_style test_unit test_integration test_system test_e2e check_static check_security build_release docs_build ;
#.PHONY: build_user
#build_user: clean init $(PROGRAM)
#
#.PHONY: build_debug
#build_debug: clean init style check $(PROGRAM) generate memcheck
#
#.PHONY: build_release
#build_release: clean init style check $(PROGRAM) generate memcheck



# Build project just for user without any checks
user: CFLAGS := $(CFLAGS) -O3
user: LFLAGS := $(LFLAGS)
user: init $(PROGRAM)

# Build project in release configuraiton into ./build/release
release: CFLAGS := $(CFLAGS) -O3
release: LFLAGS := $(LFLAGS)
release: clean init style check $(PROGRAM) generate memcheck test_bdd

# Build project in debug configuration into ./build/debug
debug: CFLAGS := $(CFLAGS) -O0 -g3 -ggdb
debug: LFLAGS := $(LFLAGS)
debug: clean init $(PROGRAM)





#build:
#	#gcc -g -Wall lib/*.h lib/*.c -o tec
#	gcc -g  lib/*.h lib/*.c -o $(PROGRAM)
#
#prof:
#	# clean up
#	rm -rf tec_prof gmon.out prof.txt
#	gcc -g -Wall -pg lib/*.h lib/*.c -o tec_prof
#	./tec_prof
#	gprof ./tec_prof > prof.txt
#
#
