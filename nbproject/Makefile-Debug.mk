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
CND_PLATFORM=GNU_5.4-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParser.o \
	${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserCfg.o \
	${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserDiacr.o \
	${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserLoad.o \
	${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserMorphS.o \
	${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserSave.o \
	${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserSoundEx.o \
	${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserTranslit.o \
	${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserTree.o \
	${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserUtil.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-static-libstdc++
CXXFLAGS=-static-libstdc++

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rebuilder

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rebuilder: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rebuilder ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParser.o: OROSSParser/OROSSParser/COROSSParser.cpp 
	${MKDIR} -p ${OBJECTDIR}/OROSSParser/OROSSParser
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParser.o OROSSParser/OROSSParser/COROSSParser.cpp

${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserCfg.o: OROSSParser/OROSSParser/COROSSParserCfg.cpp 
	${MKDIR} -p ${OBJECTDIR}/OROSSParser/OROSSParser
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserCfg.o OROSSParser/OROSSParser/COROSSParserCfg.cpp

${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserDiacr.o: OROSSParser/OROSSParser/COROSSParserDiacr.cpp 
	${MKDIR} -p ${OBJECTDIR}/OROSSParser/OROSSParser
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserDiacr.o OROSSParser/OROSSParser/COROSSParserDiacr.cpp

${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserLoad.o: OROSSParser/OROSSParser/COROSSParserLoad.cpp 
	${MKDIR} -p ${OBJECTDIR}/OROSSParser/OROSSParser
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserLoad.o OROSSParser/OROSSParser/COROSSParserLoad.cpp

${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserMorphS.o: OROSSParser/OROSSParser/COROSSParserMorphS.cpp 
	${MKDIR} -p ${OBJECTDIR}/OROSSParser/OROSSParser
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserMorphS.o OROSSParser/OROSSParser/COROSSParserMorphS.cpp

${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserSave.o: OROSSParser/OROSSParser/COROSSParserSave.cpp 
	${MKDIR} -p ${OBJECTDIR}/OROSSParser/OROSSParser
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserSave.o OROSSParser/OROSSParser/COROSSParserSave.cpp

${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserSoundEx.o: OROSSParser/OROSSParser/COROSSParserSoundEx.cpp 
	${MKDIR} -p ${OBJECTDIR}/OROSSParser/OROSSParser
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserSoundEx.o OROSSParser/OROSSParser/COROSSParserSoundEx.cpp

${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserTranslit.o: OROSSParser/OROSSParser/COROSSParserTranslit.cpp 
	${MKDIR} -p ${OBJECTDIR}/OROSSParser/OROSSParser
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserTranslit.o OROSSParser/OROSSParser/COROSSParserTranslit.cpp

${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserTree.o: OROSSParser/OROSSParser/COROSSParserTree.cpp 
	${MKDIR} -p ${OBJECTDIR}/OROSSParser/OROSSParser
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserTree.o OROSSParser/OROSSParser/COROSSParserTree.cpp

${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserUtil.o: OROSSParser/OROSSParser/COROSSParserUtil.cpp 
	${MKDIR} -p ${OBJECTDIR}/OROSSParser/OROSSParser
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OROSSParser/OROSSParser/COROSSParserUtil.o OROSSParser/OROSSParser/COROSSParserUtil.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -IOROSSParser/OROSSParser -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rebuilder

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
