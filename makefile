CPPC=~/Installations/prebuild-llvm-10.0.0/llvm/bin/clang++
CPPFLAGS=-I. -Ideps/include/
#LDFLAGS=$(shell ~/Installations/prebuild-llvm-10.0.0/llvm/bin/llvm-config --cxxflags --ldflags --system-libs --libs core mcjit native)
LDFLAGS=$(shell ~/Installations/prebuild-llvm-10.0.0/llvm/bin/llvm-config --cxxflags --ldflags --libs core)
OBJDIR=obj/
OUTDIR=bin/
LLVMLIBS=	LLVMX86Disassembler LLVMX86AsmParser LLVMX86CodeGen LLVMCFGuard LLVMGlobalISel \
			LLVMSelectionDAG LLVMAsmPrinter LLVMDebugInfoDWARF LLVMCodeGen LLVMScalarOpts \
			LLVMInstCombine LLVMAggressiveInstCombine LLVMTransformUtils LLVMBitWriter LLVMX86Desc \
			LLVMMCDisassembler LLVMX86Utils LLVMX86Info LLVMMCJIT LLVMExecutionEngine LLVMTarget \
			LLVMAnalysis LLVMProfileData LLVMRuntimeDyld LLVMObject LLVMTextAPI LLVMMCParser \
			LLVMBitReader LLVMMC LLVMDebugInfoCodeView LLVMDebugInfoMSF LLVMCore LLVMRemarks \
			LLVMBitstreamReader LLVMBinaryFormat LLVMSupport LLVMDemangle 
STDLIBS=z rt dl pthread m
LIBS=$(patsubst %,-l%,$(LLVMLIBS)) $(patsubst %,-l%,$(STDLIBS))
#LDFLAGS=-L/home/user/Installations/llvm/build/lib $(LIBS) -Ldeps/lib/llvm
#-I/home/user/Installations/llvm/llvm/include 
#-I/home/user/Installations/llvm/build/include 
#-std=c++14   -fno-exceptions -fno-rtti  

_SRCDIRS=codegen/ common/ lexer/ parser/
_OBJS=$(shell find $(_SRCDIRS) -type f -name "*.cpp")
OBJS=$(patsubst %.cpp, %.o, $(_OBJS))

toy:
	$(eval _CXXFLAGS := $(shell ~/Installations/prebuild-llvm-10.0.0/llvm/bin/llvm-config --cxxflags))
	$(eval _LDFLAGS := $(shell ~/Installations/prebuild-llvm-10.0.0/llvm/bin/llvm-config --ldflags --libs --system-libs core))
	~/Installations/prebuild-llvm-10.0.0/llvm/bin/clang++ -c $(_CXXFLAGS) rglc.cpp -o toy.o
	~/Installations/prebuild-llvm-10.0.0/llvm/bin/clang++ toy.o $(_LDFLAGS)

rglc: $(OBJS)
	$(eval _OBJFILES := $(subst /,-,$^))
	$(eval OBJFILES := $(patsubst %,$(OBJDIR)%,$(_OBJFILES)))
	$(CPPC) -o $(OUTDIR)$@ $(OBJFILES) $(CPPFLAGS) $(LDFLAGS) $@.cpp

%.o: %.cpp
	$(eval OUTOBJ := $(subst /,-,$@))
	$(CPPC) -c -o $(OBJDIR)$(OUTOBJ) $(CPPFLAGS) $<

.PHONY: clean again

clean:
	rm -rf $(OBJDIR)*
	rm -rf $(OUTDIR)*

again:
	make clean
	make rglc