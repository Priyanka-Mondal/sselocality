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
SDDO=sddomap
DYN=dynStashplus

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}
TWOOBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/${TWO_CHOICE}
PLUSOBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/${PLUS_TWO}
SDDOOBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/${SDDO}
DYNOBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/${DYN}


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

#2choice++ with dynamic stash
DYNOBJECTFILES= \
	${OBJECTDIR}/${DYN}/AES.o \
	${OBJECTDIR}/${DYN}/Amortized.o \
	${OBJECTDIR}/${DYN}/TwoChoiceClient.o \
	${OBJECTDIR}/${DYN}/TwoChoiceServer.o \
	${OBJECTDIR}/${DYN}/TwoChoiceStorage.o \
	${OBJECTDIR}/${DYN}/Server.o \
	${OBJECTDIR}/${DYN}/Storage.o \
	${OBJECTDIR}/${DYN}/Utilities.o \
	${OBJECTDIR}/${DYN}/logger.o \
	${OBJECTDIR}/${DYN}/utils.o \
	${OBJECTDIR}/dynmain.o 

#plus2 object files
PLUSOBJECTFILES= \
	${OBJECTDIR}/${PLUS_TWO}/AES.o \
	${OBJECTDIR}/${PLUS_TWO}/Amortized.o \
	${PLUSOBJECTDIR}/OneChoiceServer.o \
	${PLUSOBJECTDIR}/OneChoiceStorage.o \
	${OBJECTDIR}/${PLUS_TWO}/TwoChoiceClient.o \
	${OBJECTDIR}/${PLUS_TWO}/TwoChoiceServer.o \
	${OBJECTDIR}/${PLUS_TWO}/TwoChoiceStorage.o \
	${OBJECTDIR}/${PLUS_TWO}/Storage.o \
	${OBJECTDIR}/${PLUS_TWO}/Server.o \
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


#sddomap object files
SDDOOBJECTFILES= \
	${OBJECTDIR}/${SDDO}/AES.o \
	${OBJECTDIR}/${SDDO}/DeAmortized.o \
	${OBJECTDIR}/${SDDO}/Utilities.o \
	${OBJECTDIR}/${SDDO}/utils.o \
	${OBJECTDIR}/${SDDO}/logger.o \
	${SDDOOBJECTDIR}/RAMStore.o \
	${SDDOOBJECTDIR}/Bid.o \
	${SDDOOBJECTDIR}/OMAP.o \
	${SDDOOBJECTDIR}/ORAM.o \
	${SDDOOBJECTDIR}/AVLTree.o \
	${SDDOOBJECTDIR}/OneChoiceClient.o \
	${SDDOOBJECTDIR}/OneChoiceServer.o \
	${SDDOOBJECTDIR}/OneChoiceStorage.o \
	${SDDOOBJECTDIR}/Storage.o \
	${OBJECTDIR}/sddmain.o 
	
	#${OBJECTDIR}/${SDDO}/OneChoiceClient.o \
	${OBJECTDIR}/${SDDO}/Server.o \
	${OBJECTDIR}/${SDDO}/OneChoiceServer.o \
	${OBJECTDIR}/${SDDO}/OneChoiceStorage.o \
	${OBJECTDIR}/${SDDO}/Storage.o \

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${SDDO}/onechoice-sdd
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${DYN}/dynstash

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWO_CHOICE}/twochoice-sda: ${TWOOBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWO_CHOICE}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWO_CHOICE}/twochoice-sda ${TWOOBJECTFILES} ${LDLIBSOPTIONS} -lpthread

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${PLUS_TWO}/plustwo-sda: ${PLUSOBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${PLUS_TWO}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${PLUS_TWO}/plustwo-sda ${PLUSOBJECTFILES} ${LDLIBSOPTIONS} -lpthread

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${SDDO}/onechoice-sdd: ${SDDOOBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${SDDO}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${SDDO}/onechoice-sdd ${SDDOOBJECTFILES} ${LDLIBSOPTIONS} -lpthread

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${DYN}/dynstash: ${DYNOBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${DYN}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${DYN}/dynstash ${DYNOBJECTFILES} ${LDLIBSOPTIONS} -lpthread

######twochoice++ with dynamic stash

${OBJECTDIR}/${DYN}/AES.o: ${DYN}/AES.cpp
	${MKDIR} -p ${OBJECTDIR}/${DYN}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${DYN}/AES.o ${DYN}/AES.cpp


${OBJECTDIR}/${DYN}/Amortized.o: ${DYN}/Amortized.cpp
	${MKDIR} -p ${OBJECTDIR}/${DYN}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${DYN}/Amortized.o ${DYN}/Amortized.cpp

${OBJECTDIR}/${DYN}/TwoChoiceClient.o: ${DYN}/TwoChoiceClient.cpp
	${MKDIR} -p ${OBJECTDIR}/${DYN}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${DYN}/TwoChoiceClient.o ${DYN}/TwoChoiceClient.cpp

${OBJECTDIR}/${DYN}/TwoChoiceServer.o: ${DYN}/TwoChoiceServer.cpp
	${MKDIR} -p ${OBJECTDIR}/${DYN}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${DYN}/TwoChoiceServer.o ${DYN}/TwoChoiceServer.cpp

${OBJECTDIR}/${DYN}/TwoChoiceStorage.o: ${DYN}/TwoChoiceStorage.cpp
	${MKDIR} -p ${OBJECTDIR}/${DYN}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${DYN}/TwoChoiceStorage.o ${DYN}/TwoChoiceStorage.cpp


${OBJECTDIR}/${DYN}/Server.o: ${DYN}/Server.cpp
	${MKDIR} -p ${OBJECTDIR}/${DYN}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${DYN}/Server.o ${DYN}/Server.cpp

${OBJECTDIR}/${DYN}/Storage.o: ${DYN}/Storage.cpp
	${MKDIR} -p ${OBJECTDIR}/${DYN}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${DYN}/Storage.o ${DYN}/Storage.cpp

${OBJECTDIR}/${DYN}/Utilities.o: ${DYN}/Utilities.cpp
	${MKDIR} -p ${OBJECTDIR}/${DYN}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${DYN}/Utilities.o ${DYN}/Utilities.cpp

${OBJECTDIR}/${DYN}/logger.o: ${DYN}/logger.cpp
	${MKDIR} -p ${OBJECTDIR}/${DYN}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${DYN}/logger.o ${DYN}/logger.cpp

${OBJECTDIR}/dynmain.o: dynmain.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dynmain.o dynmain.cpp

${OBJECTDIR}/${DYN}/utils.o: ${DYN}/utils.cpp
	${MKDIR} -p ${OBJECTDIR}/${DYN}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${DYN}/utils.o ${DYN}/utils.cpp

#######two choice#########################################################

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

${PLUSOBJECTDIR}/OneChoiceServer.o: ${PLUS_TWO}/OneChoiceServer.cpp
	${MKDIR} -p ${OBJECTDIR}/${PLUS_TWO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${PLUS_TWO}/OneChoiceServer.o ${PLUS_TWO}/OneChoiceServer.cpp


${PLUSOBJECTDIR}/OneChoiceStorage.o: ${PLUS_TWO}/OneChoiceStorage.cpp
	${MKDIR} -p ${OBJECTDIR}/${PLUS_TWO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${PLUS_TWO}/OneChoiceStorage.o ${PLUS_TWO}/OneChoiceStorage.cpp

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


#####SDd+OMAP+1choice######################################################

${SDDOOBJECTDIR}/AES.o: ${SDDO}/AES.cpp
	${MKDIR} -p ${SDDOOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDOOBJECTDIR}/AES.o ${SDDO}/AES.cpp

${SDDOOBJECTDIR}/DeAmortized.o: ${SDDO}/DeAmortized.cpp
	${MKDIR} -p ${SDDOOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDOOBJECTDIR}/DeAmortized.o ${SDDO}/DeAmortized.cpp

${SDDOOBJECTDIR}/OneChoiceClient.o: ${SDDO}/OneChoiceClient.cpp
	${MKDIR} -p ${OBJECTDIR}/${SDDO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDOOBJECTDIR}/OneChoiceClient.o ${SDDO}/OneChoiceClient.cpp

${SDDOOBJECTDIR}/OneChoiceServer.o: ${SDDO}/OneChoiceServer.cpp
	${MKDIR} -p ${OBJECTDIR}/${SDDO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDOOBJECTDIR}/OneChoiceServer.o ${SDDO}/OneChoiceServer.cpp

${SDDOOBJECTDIR}/OneChoiceStorage.o: ${SDDO}/OneChoiceStorage.cpp
	${MKDIR} -p ${OBJECTDIR}/${SDDO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDOOBJECTDIR}/OneChoiceStorage.o ${SDDO}/OneChoiceStorage.cpp

${SDDOOBJECTDIR}/Storage.o: ${SDDO}/Storage.cpp
	${MKDIR} -p ${OBJECTDIR}/${SDDO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDOOBJECTDIR}/Storage.o ${SDDO}/Storage.cpp

${SDDOOBJECTDIR}/Utilities.o: ${SDDO}/Utilities.cpp
	${MKDIR} -p ${OBJECTDIR}/${SDDO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDOOBJECTDIR}/Utilities.o ${SDDO}/Utilities.cpp

${SDDOOBJECTDIR}/logger.o: ${SDDO}/logger.cpp
	${MKDIR} -p ${OBJECTDIR}/${SDDO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${SDDO}/logger.o ${SDDO}/logger.cpp

${OBJECTDIR}/sddmain.o: sddmain.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sddmain.o sddmain.cpp

${OBJECTDIR}/${SDDO}/utils.o: ${SDDO}/utils.cpp
	${MKDIR} -p ${OBJECTDIR}/${SDDO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${SDDO}/utils.o ${SDDO}/utils.cpp

${SDDOOBJECTDIR}/Bid.o: ${SDDO}/Bid.cpp
	${MKDIR} -p ${SDDOOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDOOBJECTDIR}/Bid.o ${SDDO}/Bid.cpp


${SDDOOBJECTDIR}/RAMStore.o: ${SDDO}/RAMStore.cpp
	${MKDIR} -p ${SDDOOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDOOBJECTDIR}/RAMStore.o ${SDDO}/RAMStore.cpp

${SDDOOBJECTDIR}/AVLTree.o: ${SDDO}/AVLTree.cpp
	${MKDIR} -p ${SDDOOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDOOBJECTDIR}/AVLTree.o ${SDDO}/AVLTree.cpp

${SDDOOBJECTDIR}/OMAP.o: ${SDDO}/OMAP.cpp
	${MKDIR} -p ${SDDOOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDOOBJECTDIR}/OMAP.o ${SDDO}/OMAP.cpp


${SDDOOBJECTDIR}/ORAM.o: ${SDDO}/ORAM.cpp
	${MKDIR} -p ${SDDOOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDOOBJECTDIR}/ORAM.o ${SDDO}/ORAM.cpp


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
