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
PLUS_TWO=plustwoc

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}
TWOOBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/${TWO_CHOICE}
PLUSOBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/${PLUS_TWO}


# Object Files
TWOOBJECTFILES= \
	${OBJECTDIR}/${TWO_CHOICE}/AES.o \
	${OBJECTDIR}/${TWO_CHOICE}/Amortized.o \
	${OBJECTDIR}/${TWO_CHOICE}/TwoChoiceClient.o \
	${OBJECTDIR}/${TWO_CHOICE}/TwoChoiceServer.o \
	${OBJECTDIR}/${TWO_CHOICE}/TwoChoiceStorage.o \
	${OBJECTDIR}/${TWO_CHOICE}/Server.o \
	${OBJECTDIR}/${TWO_CHOICE}/Storage.o \
	${OBJECTDIR}/${TWO_CHOICE}/Utilities.o \
	${OBJECTDIR}/${TWO_CHOICE}/logger.o \
	${OBJECTDIR}/${TWO_CHOICE}/utils.o \
	${OBJECTDIR}/twomain.o 

#plus2 object files
PLUSOBJECTFILES= \
	${OBJECTDIR}/${PLUS_TWO}/AES.o \
	${OBJECTDIR}/${PLUS_TWO}/Amortized.o \
	${OBJECTDIR}/${PLUS_TWO}/TwoChoiceClient.o \
	${OBJECTDIR}/${PLUS_TWO}/TwoChoiceServer.o \
	${OBJECTDIR}/${PLUS_TWO}/TwoChoiceStorage.o \
	${OBJECTDIR}/${PLUS_TWO}/Server.o \
	${OBJECTDIR}/${PLUS_TWO}/Storage.o \
	${OBJECTDIR}/${PLUS_TWO}/Utilities.o \
	${OBJECTDIR}/${PLUS_TWO}/logger.o \
	${OBJECTDIR}/${PLUS_TWO}/utils.o \
	${OBJECTDIR}/plusmain.o 


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${PLUS_TWO}/plustwo-sda

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWO_CHOICE}/twochoice-sda: ${TWOOBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWO_CHOICE}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWO_CHOICE}/twochoice-sda ${TWOOBJECTFILES} ${LDLIBSOPTIONS} -lpthread

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${PLUS_TWO}/plustwo-sda: ${PLUSOBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${PLUS_TWO}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${PLUS_TWO}/plustwo-sda ${PLUSOBJECTFILES} ${LDLIBSOPTIONS} -lpthread

${OBJECTDIR}/${TWO_CHOICE}/AES.o: ${TWO_CHOICE}/AES.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWO_CHOICE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWO_CHOICE}/AES.o ${TWO_CHOICE}/AES.cpp


${OBJECTDIR}/${TWO_CHOICE}/Amortized.o: ${TWO_CHOICE}/Amortized.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWO_CHOICE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWO_CHOICE}/Amortized.o ${TWO_CHOICE}/Amortized.cpp

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

${OBJECTDIR}/twomain.o: twomain.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/twomain.o twomain.cpp

${OBJECTDIR}/${TWO_CHOICE}/utils.o: ${TWO_CHOICE}/utils.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWO_CHOICE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWO_CHOICE}/utils.o ${TWO_CHOICE}/utils.cpp

#####plustwo######################################################

${PLUSOBJECTDIR}/AES.o: ${PLUS_TWO}/AES.cpp
	${MKDIR} -p ${PLUSOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${PLUSOBJECTDIR}/AES.o ${PLUS_TWO}/AES.cpp


${PLUSOBJECTDIR}/Amortized.o: ${PLUS_TWO}/Amortized.cpp
	${MKDIR} -p ${PLUSOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${PLUSOBJECTDIR}/Amortized.o ${PLUS_TWO}/Amortized.cpp

${PLUSOBJECTDIR}/TwoChoiceClient.o: ${PLUS_TWO}/TwoChoiceClient.cpp
	${MKDIR} -p ${OBJECTDIR}/${PLUS_TWO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${PLUSOBJECTDIR}/TwoChoiceClient.o ${PLUS_TWO}/TwoChoiceClient.cpp

${PLUSOBJECTDIR}/TwoChoiceServer.o: ${PLUS_TWO}/TwoChoiceServer.cpp
	${MKDIR} -p ${OBJECTDIR}/${PLUS_TWO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${PLUS_TWO}/TwoChoiceServer.o ${PLUS_TWO}/TwoChoiceServer.cpp

${PLUSOBJECTDIR}/TwoChoiceStorage.o: ${PLUS_TWO}/TwoChoiceStorage.cpp
	${MKDIR} -p ${OBJECTDIR}/${PLUS_TWO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${PLUS_TWO}/TwoChoiceStorage.o ${PLUS_TWO}/TwoChoiceStorage.cpp

${PLUSOBJECTDIR}/Server.o: ${TWO_CHOICE}/Server.cpp
	${MKDIR} -p ${OBJECTDIR}/${PLUS_TWO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${PLUS_TWO}/Server.o ${PLUS_TWO}/Server.cpp

${PLUSOBJECTDIR}/Storage.o: ${PLUS_TWO}/Storage.cpp
	${MKDIR} -p ${OBJECTDIR}/${PLUS_TWO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${PLUS_TWO}/Storage.o ${PLUS_TWO}/Storage.cpp

${PLUSOBJECTDIR}/Utilities.o: ${PLUS_TWO}/Utilities.cpp
	${MKDIR} -p ${OBJECTDIR}/${PLUS_TWO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${PLUS_TWO}/Utilities.o ${PLUS_TWO}/Utilities.cpp

${PLUSOBJECTDIR}/logger.o: ${PLUS_TWO}/logger.cpp
	${MKDIR} -p ${OBJECTDIR}/${PLUS_TWO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${PLUS_TWO}/logger.o ${PLUS_TWO}/logger.cpp

${OBJECTDIR}/plusmain.o: plusmain.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/plusmain.o plusmain.cpp

${OBJECTDIR}/${PLUS_TWO}/utils.o: ${PLUS_TWO}/utils.cpp
	${MKDIR} -p ${OBJECTDIR}/${PLUS_TWO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${PLUS_TWO}/utils.o ${PLUS_TWO}/utils.cpp



#${OBJECTDIR}/RAMStore.o: RAMStore.cpp
#	${MKDIR} -p ${OBJECTDIR}
#	${RM} "$@.d"
#	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/RAMStore.o RAMStore.cpp
#
#${OBJECTDIR}/AVLTree.o: ${TWO_CHOICE}/AVLTree.cpp
#	${MKDIR} -p ${OBJECTDIR}
#	${RM} "$@.d"
#	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AVLTree.o ${TWO_CHOICE}/AVLTree.cpp
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
