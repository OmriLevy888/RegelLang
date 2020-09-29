LLVM_DIR=~/Installations/llvm/llvm-8.0.0-prebuilt/
CPPC=$(LLVM_DIR)bin/clang++
CPPFLAGS=-I. -Ideps/include/ -std=c++17 -c
LDFLAGS=$(shell $(LLVM_DIR)bin/llvm-config --ldflags --libs) -lpthread -lncurses
OBJDIR=obj/
OUTDIR=bin/

_SRCDIRS=codegen/ common/ lexer/ parser/
_OBJS=$(shell find $(_SRCDIRS) -type f -name "*.cpp")
OBJS=$(patsubst %.cpp, %.o, $(_OBJS))

rglc: $(OBJS)
	$(eval _CXXFLAGS := $(shell $(LLVM_DIR)bin/llvm-config --cxxflags))
	$(eval MAINOBJ := $(OBJDIR)$@.o)
	$(eval __OBJS := $(subst /,-,$(OBJS)))
	$(eval _OBJS := $(patsubst %,$(OBJDIR)%,$(__OBJS)))

	$(CPPC) $(_CXXFLAGS) $(CPPFLAGS) -o $(MAINOBJ) $@.cpp 
	$(CPPC) $(LDFLAGS) -o $(OUTDIR)$@ $(_OBJS) $(MAINOBJ)

%.o: %.cpp
	$(eval OUTOBJ := $(subst /,-,$@))
	$(CPPC) -c -o $(OBJDIR)$(OUTOBJ) $(CPPFLAGS) $<

.PHONY: init clean again

init:
	mkdir bin obj

clean:
	rm -rf $(OBJDIR)*
	rm -rf $(OUTDIR)*

again:
	make clean
	make rglc