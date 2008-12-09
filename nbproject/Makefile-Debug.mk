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
OBJECTDIR=build/Debug/${PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/RooBsTimeAngle.o \
	${OBJECTDIR}/src/bs.o \
	${OBJECTDIR}/src/TransAnglesPhis.o \
	${OBJECTDIR}/src/TransAnglesEfficiency.o \
	${OBJECTDIR}/src/BsFitter.o \
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
	${MAKE}  -f nbproject/Makefile-Debug.mk bin/bs

bin/bs: ${OBJECTFILES}
	${MKDIR} -p bin
	${LINK.cc} -o bin/bs ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/src/RooBsTimeAngle.o: src/RooBsTimeAngle.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -g -I/usr/include/root -Iinclude -o ${OBJECTDIR}/src/RooBsTimeAngle.o src/RooBsTimeAngle.cc

${OBJECTDIR}/src/bs.o: src/bs.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -g -I/usr/include/root -Iinclude -o ${OBJECTDIR}/src/bs.o src/bs.cc

${OBJECTDIR}/src/TransAnglesPhis.o: src/TransAnglesPhis.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -g -I/usr/include/root -Iinclude -o ${OBJECTDIR}/src/TransAnglesPhis.o src/TransAnglesPhis.cc

${OBJECTDIR}/src/TransAnglesEfficiency.o: src/TransAnglesEfficiency.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -g -I/usr/include/root -Iinclude -o ${OBJECTDIR}/src/TransAnglesEfficiency.o src/TransAnglesEfficiency.cc

${OBJECTDIR}/src/BsFitter.o: src/BsFitter.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -g -I/usr/include/root -Iinclude -o ${OBJECTDIR}/src/BsFitter.o src/BsFitter.cc

${OBJECTDIR}/src/TransAngles.o: src/TransAngles.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -g -I/usr/include/root -Iinclude -o ${OBJECTDIR}/src/TransAngles.o src/TransAngles.cc

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} bin/bs

# Subprojects
.clean-subprojects:
