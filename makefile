LLVM_DIR=~/Installations/llvm/llvm-8.0.0-prebuilt/
CPPC=$(LLVM_DIR)bin/clang++
OBJDIR=obj/
OUTDIR=bin/

rglc : _SRCDIRS=codegen/ common/ lexer/ parser/
tests : _SRCDIRS=codegen/ common/ lexer/ parser/ tests/
OBJS=$(patsubst %.cpp,$(OBJDIR)%.o,$(shell find $(_SRCDIRS) -type f -name "*.cpp"))

CPPFLAGS=-I. -Ideps/include/ -std=c++17 -c
LDFLAGS=$(shell $(LLVM_DIR)bin/llvm-config --ldflags --libs) -lpthread -lncurses

tests : TESTS_CPPFLAGS=-DRGL_TESTS -Itests/deps/include/
tests : POSTFIX=-tests 

.PHONY: clean again all rglc tests

rglc: all

tests: all

all: $(OBJS)
	$(eval _CXXFLAGS := $(shell $(LLVM_DIR)bin/llvm-config --cxxflags))
	$(eval MAINOBJ := $(OBJDIR)rglc.o)

	$(CPPC) $(_CXXFLAGS) $(CPPFLAGS) $(TESTS_CPPFLAGS) -o $(MAINOBJ) rglc.cpp 
	$(CPPC) $(LDFLAGS) -o $(OUTDIR)rglc$(POSTFIX) $(OBJS) $(MAINOBJ)

$(OBJDIR)%.o: %.cpp
	mkdir -p $(@D)
	echo compiling object file
	$(CPPC) -c -o $@ $(CPPFLAGS) $(TESTS_CPPFLAGS) $<

clean:
	rm -rf $(OBJDIR)*
	rm -rf $(OUTDIR)*

again:
	make clean
	make rglc