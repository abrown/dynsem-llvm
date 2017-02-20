#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=Cygwin-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/engine.o \
	${OBJECTDIR}/src/log.o \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/test_assert.o \
	${OBJECTDIR}/src/types.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f1

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/engine.o \
	${TESTDIR}/tests/types.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dynsem-llvm.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dynsem-llvm.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dynsem-llvm ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/engine.o: src/engine.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/engine.o src/engine.c

${OBJECTDIR}/src/log.o: src/log.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/log.o src/log.c

${OBJECTDIR}/src/main.o: src/main.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main.o src/main.c

${OBJECTDIR}/src/test_assert.o: src/test_assert.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/test_assert.o src/test_assert.c

${OBJECTDIR}/src/types.o: src/types.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/types.o src/types.c

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/engine.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS}   

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/types.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS}   


${TESTDIR}/tests/engine.o: tests/engine.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I. -std=c11 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/engine.o tests/engine.c


${TESTDIR}/tests/types.o: tests/types.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I. -std=c11 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/types.o tests/types.c


${OBJECTDIR}/src/engine_nomain.o: ${OBJECTDIR}/src/engine.o src/engine.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/engine.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -std=c11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/engine_nomain.o src/engine.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/engine.o ${OBJECTDIR}/src/engine_nomain.o;\
	fi

${OBJECTDIR}/src/log_nomain.o: ${OBJECTDIR}/src/log.o src/log.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/log.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -std=c11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/log_nomain.o src/log.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/log.o ${OBJECTDIR}/src/log_nomain.o;\
	fi

${OBJECTDIR}/src/main_nomain.o: ${OBJECTDIR}/src/main.o src/main.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -std=c11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main_nomain.o src/main.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/main_nomain.o;\
	fi

${OBJECTDIR}/src/test_assert_nomain.o: ${OBJECTDIR}/src/test_assert.o src/test_assert.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/test_assert.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -std=c11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/test_assert_nomain.o src/test_assert.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/test_assert.o ${OBJECTDIR}/src/test_assert_nomain.o;\
	fi

${OBJECTDIR}/src/types_nomain.o: ${OBJECTDIR}/src/types.o src/types.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/types.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -std=c11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/types_nomain.o src/types.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/types.o ${OBJECTDIR}/src/types_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
