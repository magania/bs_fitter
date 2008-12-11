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
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran

# Macros
PLATFORM=GNU-Linux-x86

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/Release/${PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/fit_et.o \
	${OBJECTDIR}/src/RooBsTimeAngle.o \
	${OBJECTDIR}/src/bs.o \
	${OBJECTDIR}/src/BsFitter.o \
	${OBJECTDIR}/src/TransAnglesPhis.o \
	${OBJECTDIR}/src/TransAnglesEfficiency.o \
	${OBJECTDIR}/src/TransAngles.o

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
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/${PLATFORM}/bsfitter

dist/Release/${PLATFORM}/bsfitter: ${OBJECTFILES}
	${MKDIR} -p dist/Release/${PLATFORM}
	${LINK.cc} -o dist/Release/${PLATFORM}/bsfitter ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/fit_et.o: fit_et.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/fit_et.o fit_et.cpp

${OBJECTDIR}/src/RooBsTimeAngle.o: src/RooBsTimeAngle.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/src/RooBsTimeAngle.o src/RooBsTimeAngle.cc

${OBJECTDIR}/src/bs.o: src/bs.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/src/bs.o src/bs.cc

${OBJECTDIR}/src/BsFitter.o: src/BsFitter.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/src/BsFitter.o src/BsFitter.cc

${OBJECTDIR}/src/TransAnglesPhis.o: src/TransAnglesPhis.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/src/TransAnglesPhis.o src/TransAnglesPhis.cc

${OBJECTDIR}/src/TransAnglesEfficiency.o: src/TransAnglesEfficiency.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/src/TransAnglesEfficiency.o src/TransAnglesEfficiency.cc

${OBJECTDIR}/src/TransAngles.o: src/TransAngles.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/src/TransAngles.o src/TransAngles.cc

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} dist/Release/${PLATFORM}/bsfitter

# Subprojects
.clean-subprojects:
