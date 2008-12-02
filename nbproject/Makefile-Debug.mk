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
OBJECTDIR=build/Debug/GNU-Linux-x86

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/RooBsTimeAngle.o \
	${OBJECTDIR}/src/fitter.o \
	${OBJECTDIR}/src/Phis.o \
	${OBJECTDIR}/src/bs.o \
	${OBJECTDIR}/src/RooBkgAngle.o \
	${OBJECTDIR}/src/TransAnglesPhis.o \
	${OBJECTDIR}/src/TimeAngle.o \
	${OBJECTDIR}/src/TransAnglesEfficiency.o \
	${OBJECTDIR}/src/F.o \
	${OBJECTDIR}/efficiency.o \
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
.build-conf: ${BUILD_SUBPROJECTS} bin/bs

bin/bs: ${OBJECTFILES}
	${MKDIR} -p bin
	${LINK.cc} -o bin/bs ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/src/RooBsTimeAngle.o: src/RooBsTimeAngle.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -g -I/usr/include/root -Iinclude -o ${OBJECTDIR}/src/RooBsTimeAngle.o src/RooBsTimeAngle.cc

${OBJECTDIR}/src/fitter.o: src/fitter.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -g -I/usr/include/root -Iinclude -o ${OBJECTDIR}/src/fitter.o src/fitter.cc

${OBJECTDIR}/src/Phis.o: src/Phis.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -g -I/usr/include/root -Iinclude -o ${OBJECTDIR}/src/Phis.o src/Phis.cc

${OBJECTDIR}/src/bs.o: src/bs.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -g -I/usr/include/root -Iinclude -o ${OBJECTDIR}/src/bs.o src/bs.cc

${OBJECTDIR}/src/RooBkgAngle.o: src/RooBkgAngle.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -g -I/usr/include/root -Iinclude -o ${OBJECTDIR}/src/RooBkgAngle.o src/RooBkgAngle.cc

${OBJECTDIR}/src/TransAnglesPhis.o: src/TransAnglesPhis.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -g -I/usr/include/root -Iinclude -o ${OBJECTDIR}/src/TransAnglesPhis.o src/TransAnglesPhis.cc

${OBJECTDIR}/src/TimeAngle.o: src/TimeAngle.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -g -I/usr/include/root -Iinclude -o ${OBJECTDIR}/src/TimeAngle.o src/TimeAngle.cc

${OBJECTDIR}/src/TransAnglesEfficiency.o: src/TransAnglesEfficiency.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -g -I/usr/include/root -Iinclude -o ${OBJECTDIR}/src/TransAnglesEfficiency.o src/TransAnglesEfficiency.cc

${OBJECTDIR}/src/F.o: src/F.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	$(COMPILE.cc) -g -I/usr/include/root -Iinclude -o ${OBJECTDIR}/src/F.o src/F.cc

${OBJECTDIR}/efficiency.o: efficiency.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I/usr/include/root -Iinclude -o ${OBJECTDIR}/efficiency.o efficiency.cc

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
