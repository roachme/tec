MAKE=make
PROGRAM=build/_tec
VERSION=$(shell cat VERSION)
README=README.md
SHELLSCRIPT=shell/tec.sh
SHELLNAME=bash
CC=gcc
SRCS=$(wildcard lib/*.c cli/*.c cli/aux/*.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))
CFLAGS=-I cli -Wall -O3 -fbounds-check -Wpedantic -Wextra
LFLAGS=-lconfig -g
PWDFILE=/tmp/tecpwd

# TODO: add debug mode
# TODO: add user and developer builds

all: init $(PROGRAM)
.PHONY: clean debug generate release

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
	cppcheck --std=c89 --enable=all --language=c $(SRCS)

valgrind:
	valgrind  --track-origins=yes --leak-check=full --show-leak-kinds=all ./$(PROGRAM)

install:
	./utils/install full

install-link:
	./utils/install soft

style:
	find . -name '*.[ch]' | xargs indent -nut -nbad -bap -nbc -bbo -hnl -br -brs -c33 -cd33 -ncdb -ce -ci4 -cli0 -d0 -di1 -nfc1 -i4 -ip0 -l80 -lp -npcs -nprs -npsl -sai -saf -saw -ncs -nsc -sob -nfca -cp33 -ss -ts8 -il1
	find . -name '*.[ch]\~' | xargs	rm

memcheck:
	./tests/memleak/memcheck

# Build project in debug configuration into ./build/debug
.PHONY: build_debug
build_debug:
	make debug


# Build project in release configuraiton into ./build/release
.PHONY: build_release
build_release:
	make


#all_release: clean init check_style test_unit test_integration test_system test_e2e check_static check_security build_release docs_build ;
release: clean init style check $(PROGRAM) generate memcheck


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
