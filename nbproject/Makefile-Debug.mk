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
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build
TWO_CHOICE=twochoice

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}


# Object Files
OBJECTFILES= \
	${OBJECTDIR}/${TWO_CHOICE}/AES.o \
	${OBJECTDIR}/${TWO_CHOICE}/Amortized.o \
	${OBJECTDIR}/${TWO_CHOICE}/TwoChoiceClient.o \
	${OBJECTDIR}/${TWO_CHOICE}/TwoChoiceServer.o \
	${OBJECTDIR}/${TWO_CHOICE}/TwoChoiceStorage.o \
	${OBJECTDIR}/${TWO_CHOICE}/Server.o \
	${OBJECTDIR}/${TWO_CHOICE}/Storage.o \
	${OBJECTDIR}/${TWO_CHOICE}/Utilities.o \
	${OBJECTDIR}/${TWO_CHOICE}/logger.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/${TWO_CHOICE}/utils.o


	#${OBJECTDIR}/AmortizedBASClient.o \
	${OBJECTDIR}/AmortizedBASServer.o \
	${OBJECTDIR}/Bid.o \
	${OBJECTDIR}/OMAP.o \
	${OBJECTDIR}/ORAM.o \
	${OBJECTDIR}/OneChoiceClient.o \
	${OBJECTDIR}/OneChoiceServer.o \
	${OBJECTDIR}/OneChoiceStorage.o \
	${OBJECTDIR}/RAMStore.o \
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
LDLIBSOPTIONS=-lcrypto -lssl #-lstxxl_debug

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWO_CHOICE}/twochoice-sda

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWO_CHOICE}/twochoice-sda: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWO_CHOICE}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWO_CHOICE}/twochoice-sda ${OBJECTFILES} ${LDLIBSOPTIONS} -lpthread

${OBJECTDIR}/${TWO_CHOICE}/AES.o: ${TWO_CHOICE}/AES.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWO_CHOICE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWO_CHOICE}/AES.o ${TWO_CHOICE}/AES.cpp

#${OBJECTDIR}/AVLTree.o: ${TWO_CHOICE}/AVLTree.cpp
#	${MKDIR} -p ${OBJECTDIR}
#	${RM} "$@.d"
#	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AVLTree.o ${TWO_CHOICE}/AVLTree.cpp

${OBJECTDIR}/${TWO_CHOICE}/Amortized.o: ${TWO_CHOICE}/Amortized.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWO_CHOICE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWO_CHOICE}/Amortized.o ${TWO_CHOICE}/Amortized.cpp

#${OBJECTDIR}/AmortizedBASClient.o: AmortizedBASClient.cpp
#	${MKDIR} -p ${OBJECTDIR}
#	${RM} "$@.d"
#	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AmortizedBASClient.o AmortizedBASClient.cpp
#
#${OBJECTDIR}/AmortizedBASServer.o: AmortizedBASServer.cpp
#	${MKDIR} -p ${OBJECTDIR}
#	${RM} "$@.d"
#	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AmortizedBASServer.o AmortizedBASServer.cpp
#
#${OBJECTDIR}/Bid.o: Bid.cpp
#	${MKDIR} -p ${OBJECTDIR}
#	${RM} "$@.d"
#	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Bid.o Bid.cpp
#
#${OBJECTDIR}/OMAP.o: OMAP.cpp
#	${MKDIR} -p ${OBJECTDIR}
#	${RM} "$@.d"
#	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OMAP.o OMAP.cpp
#
#${OBJECTDIR}/ORAM.o: ORAM.cpp
#	${MKDIR} -p ${OBJECTDIR}
#	${RM} "$@.d"
#	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ORAM.o ORAM.cpp
#
#${OBJECTDIR}/OneChoiceClient.o: OneChoiceClient.cpp
#	${MKDIR} -p ${OBJECTDIR}
#	${RM} "$@.d"
#	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OneChoiceClient.o OneChoiceClient.cpp
#
#${OBJECTDIR}/OneChoiceServer.o: OneChoiceServer.cpp
#	${MKDIR} -p ${OBJECTDIR}
#	${RM} "$@.d"
#	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OneChoiceServer.o OneChoiceServer.cpp
#
#${OBJECTDIR}/OneChoiceStorage.o: OneChoiceStorage.cpp
#	${MKDIR} -p ${OBJECTDIR}
#	${RM} "$@.d"
#	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OneChoiceStorage.o OneChoiceStorage.cpp
#
${OBJECTDIR}/${TWO_CHOICE}/TwoChoiceClient.o: ${TWO_CHOICE}/TwoChoiceClient.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWO_CHOICE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWO_CHOICE}/TwoChoiceClient.o ${TWO_CHOICE}/TwoChoiceClient.cpp

${OBJECTDIR}/${TWO_CHOICE}/TwoChoiceServer.o: ${TWO_CHOICE}/TwoChoiceServer.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWO_CHOICE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWO_CHOICE}/TwoChoiceServer.o ${TWO_CHOICE}/TwoChoiceServer.cpp

${OBJECTDIR}/${TWO_CHOICE}/TwoChoiceStorage.o: ${TWO_CHOICE}/TwoChoiceStorage.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWO_CHOICE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWO_CHOICE}/TwoChoiceStorage.o ${TWO_CHOICE}/TwoChoiceStorage.cpp

#${OBJECTDIR}/RAMStore.o: RAMStore.cpp
#	${MKDIR} -p ${OBJECTDIR}
#	${RM} "$@.d"
#	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/RAMStore.o RAMStore.cpp

${OBJECTDIR}/${TWO_CHOICE}/Server.o: ${TWO_CHOICE}/Server.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWO_CHOICE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWO_CHOICE}/Server.o ${TWO_CHOICE}/Server.cpp

${OBJECTDIR}/${TWO_CHOICE}/Storage.o: ${TWO_CHOICE}/Storage.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWO_CHOICE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWO_CHOICE}/Storage.o ${TWO_CHOICE}/Storage.cpp

${OBJECTDIR}/${TWO_CHOICE}/Utilities.o: ${TWO_CHOICE}/Utilities.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWO_CHOICE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWO_CHOICE}/Utilities.o ${TWO_CHOICE}/Utilities.cpp

${OBJECTDIR}/${TWO_CHOICE}/logger.o: ${TWO_CHOICE}/logger.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWO_CHOICE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWO_CHOICE}/logger.o ${TWO_CHOICE}/logger.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/${TWO_CHOICE}/utils.o: ${TWO_CHOICE}/utils.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWO_CHOICE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWO_CHOICE}/utils.o ${TWO_CHOICE}/utils.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
