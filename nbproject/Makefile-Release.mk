#
# Gererated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/Release/GNU-Linux-x86

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/RooBsTimeAngle.o \
	${OBJECTDIR}/src/fitter.o \
	${OBJECTDIR}/src/BsFitter.o \
	${OBJECTDIR}/src/TimeAngle.o \
	${OBJECTDIR}/src/TransAngles.o \
	${OBJECTDIR}/src/F.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} dist/Release/GNU-Linux-x86/bsfitter

dist/Release/GNU-Linux-x86/bsfitter: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.cc} -o dist/Release/GNU-Linux-x86/bsfitter ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/src/RooBsTimeAngle.o: src/RooBsTimeAngle.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/src/RooBsTimeAngle.o src/RooBsTimeAngle.cc

${OBJECTDIR}/src/fitter.o: src/fitter.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/src/fitter.o src/fitter.cc

${OBJECTDIR}/src/BsFitter.o: src/BsFitter.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/src/BsFitter.o src/BsFitter.cc

${OBJECTDIR}/src/TimeAngle.o: src/TimeAngle.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/src/TimeAngle.o src/TimeAngle.cc

${OBJECTDIR}/src/TransAngles.o: src/TransAngles.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/src/TransAngles.o src/TransAngles.cc

${OBJECTDIR}/src/F.o: src/F.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/src/F.o src/F.cc

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/bsfitter

# Subprojects
.clean-subprojects:
