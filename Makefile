DEBUG = 1

CC  = armcc
CXX = armcpp
AS  = armasm
LD  = armlink
AR  = armlib
RM  = rm

## Flag definitions ##
# -bigend   : Compiles code for an ARM operating with big-endian memory. The most
#             significant byte has lowest address.
# -za0      : LDR is not restricted to accessing word-aligned addresses.
# -zps0     :
# -zpv1     :
# -zi4      : The compiler selects a value for the maximum number of instructions
#             allowed to generate an integer literal inline before using LDR rx,=value
# -fa       : Checks for certain types of data flow anomalies.
# -fh       : Checks "all external objects are declared before use",
#             "all file-scoped static objects are used",
#             "all predeclarations of static functions are used between
#              their declaration and their definition".
# -fv       : Reports on all unused declarations, including those from standard headers.
# -fx       : Enables all warnings that are suppressed by default.
# -fpu none : No FPU. Use software floating point library.
# -arch 3   : Compile using ARM3 architecture.
# -apcs     : nofp - Do not use a frame-pointer register. This is the default.
#             softfp - Use software floating-point library functions for floating-point code.
#             noswstackcheck - No software stack-checking PCS variant. This is the default.
OPT      = -O2
CFLAGS   = $(OPT) -bigend -za1 -zps0 -zi4 -fa -fh -fx -fpu none -arch 3 -apcs '/softfp/nofp/swstackcheck'
CXXFLAGS = $(CFLAGS)
ASFLAGS	 = -BI -i ./include/3do
INCPATH	 = -I ./include/3do -I ./include/ttl
LIBPATH  = ./lib/3do
LDFLAGS	 = -match 0x1 -nodebug -noscanlib -verbose -remove -aif -reloc -dupok -ro-base 0 -sym $(EXENAME).sym -libpath $(LIBPATH)
STARTUP	 = $(LIBPATH)/cstartup.o

SRC_S   = $(wildcard src/*.s)
SRC_C   = $(wildcard src/*.c)
SRC_CXX = $(wildcard src/*.cpp)

OBJ += $(SRC_S:src/%.s=build/%.s.o)
OBJ += $(SRC_C:src/%.c=build/%.c.o)
OBJ += $(SRC_CXX:src/%.cpp=build/%.cpp.o)

all: cpplib

cpplib: builddir $(OBJ)
	$(AR) -c build/cpplib.lib $(OBJ) 

builddir:
	mkdir -p build/

build/%.s.o: src/%.s
	$(AS) $(INCPATH) $(ASFLAGS) $< -o $@

build/%.c.o: src/%.c
	$(CC) $(INCPATH) $(CFLAGS) -c $< -o $@

build/%.cpp.o: src/%.cpp
	$(CXX) $(INCPATH) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) -vf $(OBJ) cpplib

.PHONY: clean
