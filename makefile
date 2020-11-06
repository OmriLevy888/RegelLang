LLVM_DIR=~/Installations/llvm/llvm-8.0.0-prebuilt/
CPPC=$(LLVM_DIR)bin/clang++
RELEASE_OBJDIR=obj/release/
DEBUG_OBJDIR=obj/debug/
TEST_OBJDIR=obj/tests/
OUTDIR=bin/

RELEASE_SRCDIRS=codegen/ common/ lexer/ parser/
DEBUG_SRCDIRS=codegen/ common/ lexer/ parser/
TEST_SRCDIRS=codegen/ common/ lexer/ parser/ tests/
RELEASE_OBJS=$(patsubst %.cpp,$(RELEASE_OBJDIR)%.o,$(shell find $(RELEASE_SRCDIRS) -type f -name "*.cpp"))
DEBUG_OBJS=$(patsubst %.cpp,$(DEBUG_OBJDIR)%.o,$(shell find $(DEBUG_SRCDIRS) -type f -name "*.cpp"))
TEST_OBJS=$(patsubst %.cpp,$(TEST_OBJDIR)%.o,$(shell find $(TEST_SRCDIRS) -type f -name "*.cpp"))

CPPFLAGS=-I. -Ideps/include/ -std=c++17 -c
CXXFLAGS=$(shell $(LLVM_DIR)bin/llvm-config --cxxflags)
LDFLAGS=$(shell $(LLVM_DIR)bin/llvm-config --ldflags --libs) -lpthread -lncurses

DEBUG_CPPFLAGS=-g
DEBUG_POSTFIX=-debug 
TEST_CPPFLAGS=-g -DRGL_TESTS -Itests/deps/include/
TEST_POSTFIX=-tests 

.PHONY: clean again rglc debug tests

rglc: $(RELEASE_OBJS)
	$(eval MAINOBJ := $(RELEASE_OBJDIR)rglc.o)
	mkdir -p $(RELEASE_OBJDIR)
	mkdir -p $(OUTDIR)

	$(CPPC) $(CXXFLAGS) $(CPPFLAGS) $(DEBUG_CPPFLAGS) -o $(MAINOBJ) rglc.cpp 
	$(CPPC) $(LDFLAGS) -o $(OUTDIR)rglc$(POSTFIX) $(RELEASE_OBJS) $(MAINOBJ)

debug: $(DEBUG_OBJS)
	$(eval MAINOBJ := $(DEBUG_OBJDIR)rglc.o)
	mkdir -p $(DEBUG_OBJDIR)
	mkdir -p $(OUTDIR)

	$(CPPC) $(CXXFLAGS) $(CPPFLAGS) $(DEBUG_CPPFLAGS) -o $(MAINOBJ) rglc.cpp 
	$(CPPC) $(LDFLAGS) -o $(OUTDIR)rglc$(DEBUG_POSTFIX) $(DEBUG_OBJS) $(MAINOBJ)

tests: $(TEST_OBJS)
	$(eval MAINOBJ := $(TEST_OBJDIR)rglc.o)
	mkdir -p $(TEST_OBJDIR)
	mkdir -p $(OUTDIR)

	$(CPPC) $(CXXFLAGS) $(CPPFLAGS) $(TEST_CPPFLAGS) -o $(MAINOBJ) rglc.cpp 
	$(CPPC) $(LDFLAGS) -o $(OUTDIR)rglc$(TEST_POSTFIX) $(TEST_OBJS) $(MAINOBJ)

$(RELEASE_OBJDIR)%.o: %.cpp
	mkdir -p $(@D)
	$(CPPC) -c -o $@ $(CPPFLAGS) $<

$(DEBUG_OBJDIR)%.o: %.cpp
	mkdir -p $(@D)
	$(CPPC) -c -o $@ $(CPPFLAGS) $(DEBUG_CPPFLAGS) $<

$(TEST_OBJDIR)%.o: %.cpp
	mkdir -p $(@D)
	$(CPPC) -c -o $@ $(CPPFLAGS) $(TEST_CPPFLAGS) $<

clean:
	rm -rf $(RELEASE_OBJDIR)*
	rm -rf $(DEBUG_OBJDIR)*
	rm -rf $(OUTDIR)*

again:
	make clean
	make rglc