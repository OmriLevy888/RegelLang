LLVM_DIR=~/Installations/llvm/llvm-8.0.0-prebuilt/
LLVM_DIR=/usr/
CPPC=$(LLVM_DIR)bin/clang++
LLVM_PROFDATA=$(LLVM_DIR)bin/llvm-profdata
LLVM_COV=$(LLVM_DIR)bin/llvm-cov
RELEASE_OBJDIR=obj/release/
DEBUG_OBJDIR=obj/debug/
TEST_OBJDIR=obj/tests/
OUTDIR=bin/

RELEASE_SRCDIRS=codegen/ common/ lexer/ parser/ cli/
DEBUG_SRCDIRS=codegen/ common/ lexer/ parser/ cli/
TEST_SRCDIRS=codegen/ common/ lexer/ parser/ cli/ tests/
RELEASE_OBJS=$(patsubst %.cpp,$(RELEASE_OBJDIR)%.o,$(shell find $(RELEASE_SRCDIRS) -type f -name "*.cpp"))
DEBUG_OBJS=$(patsubst %.cpp,$(DEBUG_OBJDIR)%.o,$(shell find $(DEBUG_SRCDIRS) -type f -name "*.cpp"))
TEST_OBJS=$(patsubst %.cpp,$(TEST_OBJDIR)%.o,$(shell find $(TEST_SRCDIRS) -type f -name "*.cpp"))

CPPFLAGS=-I. -Ideps/include/ -std=c++17 -c
_CXXFLAGS=$(shell $(LLVM_DIR)bin/llvm-config --cxxflags)
CXXFLAGS=$(patsubst -fno-exceptions,,$(_CXXFLAGS))
LDFLAGS=$(shell $(LLVM_DIR)bin/llvm-config --ldflags --libs) -lpthread -lncurses

DEBUG_CPPFLAGS=-g
DEBUG_POSTFIX=-debug 
TEST_CPPFLAGS=-fprofile-instr-generate -fcoverage-mapping -DRGL_TESTS -Itests/deps/include/ -g
TEST_POSTFIX=-tests 
TEST_LDFLAGS=-fprofile-instr-generate -fcoverage-mapping -Ldeps/lib/gtest/ -lgtest

.PHONY: clean rglc debug tests cloc coverage

rglc: $(RELEASE_OBJS)
	$(eval MAINOBJ := $(RELEASE_OBJDIR)rglc.o)
	mkdir -p $(RELEASE_OBJDIR)
	mkdir -p $(OUTDIR)

	@$(CPPC) $(CXXFLAGS) $(CPPFLAGS) $(DEBUG_CPPFLAGS) -o $(MAINOBJ) rglc.cpp 
	@$(CPPC) $(LDFLAGS) -o $(OUTDIR)rglc$(POSTFIX) $(RELEASE_OBJS) $(MAINOBJ)

debug: $(DEBUG_OBJS)
	$(eval MAINOBJ := $(DEBUG_OBJDIR)rglc.o)
	mkdir -p $(DEBUG_OBJDIR)
	mkdir -p $(OUTDIR)

	@$(CPPC) $(CXXFLAGS) $(CPPFLAGS) $(DEBUG_CPPFLAGS) -o $(MAINOBJ) rglc.cpp 
	@$(CPPC) $(LDFLAGS) -o $(OUTDIR)rglc$(DEBUG_POSTFIX) $(DEBUG_OBJS) $(MAINOBJ)

tests: $(TEST_OBJS)
	$(eval MAINOBJ := $(TEST_OBJDIR)rglc.o)
	mkdir -p $(TEST_OBJDIR)
	mkdir -p $(OUTDIR)

	@$(CPPC) $(CXXFLAGS) $(CPPFLAGS) $(TEST_CPPFLAGS) -o $(MAINOBJ) rglc.cpp 
	@$(CPPC) $(LDFLAGS) -o $(OUTDIR)rglc$(TEST_POSTFIX) $(TEST_OBJS) $(MAINOBJ) $(TEST_LDFLAGS)

$(RELEASE_OBJDIR)%.o: %.cpp
	@mkdir -p $(@D)
	@echo Compiling $@
	@$(CPPC) -c -o $@ $(CPPFLAGS) $<

$(DEBUG_OBJDIR)%.o: %.cpp
	@mkdir -p $(@D)
	@echo Compiling $@
	@$(CPPC) -c -o $@ $(CPPFLAGS) $(DEBUG_CPPFLAGS) $<

$(TEST_OBJDIR)%.o: %.cpp
	@mkdir -p $(@D)
	@echo Compiling $@
	@$(CPPC) -c -o $@ $(CPPFLAGS) $(TEST_CPPFLAGS) $<

clean:
	rm -rf $(RELEASE_OBJDIR)*
	rm -rf $(DEBUG_OBJDIR)*
	rm -rf $(TEST_OBJDIR)*
	rm -rf $(OUTDIR)*

cloc:
	cloc cli codegen common lang-spec lexer parser source tests

coverage: tests
	@./$(OUTDIR)rglc$(TEST_POSTFIX) >/dev/null 2>&1

	@$(LLVM_PROFDATA) merge -output=merged.profraw default.profraw
	@$(LLVM_COV) report ./$(OUTDIR)rglc$(TEST_POSTFIX) -instr-profile=merged.profraw $(RELEASE_SRCDIRS) -use-color
	@$(LLVM_COV) show ./$(OUTDIR)rglc$(TEST_POSTFIX) -instr-profile=merged.profraw $(RELEASE_SRCDIRS) -use-color -format html > coverage.html
