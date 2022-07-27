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
SDDNO=SDDnoOMAP
DYN=dynStashplus
TWOONE=twoWithOne
V1=vone
V2=tunableTwoChoice
VP2=tunableTwoChoicePP



# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}
TWOOBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/${TWO_CHOICE}
PLUSOBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/${PLUS_TWO}
SDDOOBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/${SDDO}
SDDNOOBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/${SDDNO}
DYNOBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/${DYN}
TWOONEOBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/${TWOONE}
V1OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/${V1}
V2OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/${V2}
VP2OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/${VP2}


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

V1OBJECTFILES= \
	${OBJECTDIR}/${V1}/AES.o \
	${OBJECTDIR}/${V1}/Amortized.o \
	${OBJECTDIR}/${V1}/OneChoiceServer.o \
	${OBJECTDIR}/${V1}/OneChoiceStorage.o \
	${OBJECTDIR}/${V1}/TwoChoiceClient.o \
	${OBJECTDIR}/${V1}/TwoChoiceServer.o \
	${OBJECTDIR}/${V1}/TwoChoiceStorage.o \
	${OBJECTDIR}/${V1}/Server.o \
	${OBJECTDIR}/${V1}/Storage.o \
	${OBJECTDIR}/${V1}/Utilities.o \
	${OBJECTDIR}/${V1}/logger.o \
	${OBJECTDIR}/${V1}/utils.o \
	${OBJECTDIR}/v1main.o 

#####VP2######

VP2OBJECTFILES= \
	${OBJECTDIR}/${VP2}/AES.o \
	${OBJECTDIR}/${VP2}/Amortized.o \
	${OBJECTDIR}/${VP2}/OneChoiceServer.o \
	${OBJECTDIR}/${VP2}/OneChoiceStorage.o \
	${OBJECTDIR}/${VP2}/TwoChoicePPWithTunableLocalityClient.o \
	${OBJECTDIR}/${VP2}/TwoChoicePPWithTunableLocalityServer.o \
	${OBJECTDIR}/${VP2}/TwoChoicePPWithTunableLocalityStorage.o \
	${OBJECTDIR}/${VP2}/Server.o \
	${OBJECTDIR}/${VP2}/Storage.o \
	${OBJECTDIR}/${VP2}/Utilities.o \
	${OBJECTDIR}/${VP2}/logger.o \
	${OBJECTDIR}/${VP2}/utils.o \
	${OBJECTDIR}/vp2main.o 



#####V2
V2OBJECTFILES= \
	${OBJECTDIR}/${V2}/AES.o \
	${OBJECTDIR}/${V2}/Amortized.o \
	${OBJECTDIR}/${V2}/OneChoiceServer.o \
	${OBJECTDIR}/${V2}/OneChoiceStorage.o \
	${OBJECTDIR}/${V2}/TwoChoiceWithTunableLocalityClient.o \
	${OBJECTDIR}/${V2}/TwoChoiceWithTunableLocalityServer.o \
	${OBJECTDIR}/${V2}/TwoChoiceWithTunableLocalityStorage.o \
	${OBJECTDIR}/${V2}/Server.o \
	${OBJECTDIR}/${V2}/Storage.o \
	${OBJECTDIR}/${V2}/Utilities.o \
	${OBJECTDIR}/${V2}/logger.o \
	${OBJECTDIR}/${V2}/utils.o \
	${OBJECTDIR}/v2main.o 

#twoOne
TWOONEOBJECTFILES= \
	${OBJECTDIR}/${TWOONE}/AES.o \
	${OBJECTDIR}/${TWOONE}/Amortized.o \
	${TWOONEOBJECTDIR}/OneChoiceServer.o \
	${TWOONEOBJECTDIR}/OneChoiceStorage.o \
	${OBJECTDIR}/${TWOONE}/TwoChoiceClient.o \
	${OBJECTDIR}/${TWOONE}/TwoChoiceServer.o \
	${OBJECTDIR}/${TWOONE}/TwoChoiceStorage.o \
	${OBJECTDIR}/${TWOONE}/Server.o \
	${OBJECTDIR}/${TWOONE}/Storage.o \
	${OBJECTDIR}/${TWOONE}/Utilities.o \
	${OBJECTDIR}/${TWOONE}/logger.o \
	${OBJECTDIR}/${TWOONE}/utils.o \
	${OBJECTDIR}/twoonemain.o 


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

#######sddnoomap#######################

SDDNOOBJECTFILES= \
	${OBJECTDIR}/${SDDNO}/AES.o \
	${OBJECTDIR}/${SDDNO}/DeAmortized.o \
	${OBJECTDIR}/${SDDNO}/Utilities.o \
	${OBJECTDIR}/${SDDNO}/utils.o \
	${OBJECTDIR}/${SDDNO}/logger.o \
	${SDDNOOBJECTDIR}/RAMStore.o \
	${SDDNOOBJECTDIR}/Bid.o \
	${SDDNOOBJECTDIR}/OMAP.o \
	${SDDNOOBJECTDIR}/ORAM.o \
	${SDDNOOBJECTDIR}/AVLTree.o \
	${SDDNOOBJECTDIR}/OneChoiceClient.o \
	${SDDNOOBJECTDIR}/OneChoiceServer.o \
	${SDDNOOBJECTDIR}/OneChoiceStorage.o \
	${SDDNOOBJECTDIR}/Storage.o \
	${OBJECTDIR}/sddnmain.o 

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
	${SDDOOBJECTDIR}/Bitonic.o \
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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${SDDO}/onechoice-sdd
#	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${VP2}/vptwo
#	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${V2}/vtwo
#	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWOONE}/twoone
#	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${SDDNO}/onechoice-sddn

#	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWO_CHOICE}/twochoice-sda
#	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${PLUS_TWO}/plustwo-sda
#	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${DYN}/dynstash
#	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${V1}/vone



${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${VP2}/vptwo: ${VP2OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${VP2}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${VP2}/vptwo ${VP2OBJECTFILES} ${LDLIBSOPTIONS} -lpthread


${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${V2}/vtwo: ${V2OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${V2}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${V2}/vtwo ${V2OBJECTFILES} ${LDLIBSOPTIONS} -lpthread

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWO_CHOICE}/twochoice-sda: ${TWOOBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWO_CHOICE}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWO_CHOICE}/twochoice-sda ${TWOOBJECTFILES} ${LDLIBSOPTIONS} -lpthread

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${PLUS_TWO}/plustwo-sda: ${PLUSOBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${PLUS_TWO}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${PLUS_TWO}/plustwo-sda ${PLUSOBJECTFILES} ${LDLIBSOPTIONS} -lpthread

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${SDDO}/onechoice-sdd: ${SDDOOBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${SDDO}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${SDDO}/onechoice-sdd ${SDDOOBJECTFILES} ${LDLIBSOPTIONS} -lpthread

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${SDDNO}/onechoice-sddn: ${SDDNOOBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${SDDNO}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${SDDNO}/onechoice-sddn ${SDDNOOBJECTFILES} ${LDLIBSOPTIONS} -lpthread


${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${DYN}/dynstash: ${DYNOBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${DYN}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${DYN}/dynstash ${DYNOBJECTFILES} ${LDLIBSOPTIONS} -lpthread

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWOONE}/twoone: ${TWOONEOBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWOONE}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${TWOONE}/twoone ${TWOONEOBJECTFILES} ${LDLIBSOPTIONS} -lpthread

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${V1}/vone: ${V1OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${V1}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/${V1}/vone ${V1OBJECTFILES} ${LDLIBSOPTIONS} -lpthread



#vp2##################################################

${OBJECTDIR}/${VP2}/AES.o: ${VP2}/AES.cpp
	${MKDIR} -p ${OBJECTDIR}/${VP2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${VP2}/AES.o ${VP2}/AES.cpp

${OBJECTDIR}/${VP2}/Amortized.o: ${VP2}/Amortized.cpp
	${MKDIR} -p ${OBJECTDIR}/${VP2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${VP2}/Amortized.o ${VP2}/Amortized.cpp

${VP2OBJECTDIR}/OneChoiceServer.o: ${VP2}/OneChoiceServer.cpp
	${MKDIR} -p ${OBJECTDIR}/${VP2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${VP2}/OneChoiceServer.o ${VP2}/OneChoiceServer.cpp

${VP2OBJECTDIR}/OneChoiceStorage.o: ${VP2}/OneChoiceStorage.cpp
	${MKDIR} -p ${OBJECTDIR}/${VP2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${VP2}/OneChoiceStorage.o ${VP2}/OneChoiceStorage.cpp

${OBJECTDIR}/${VP2}/TwoChoicePPWithTunableLocalityClient.o: ${VP2}/TwoChoicePPWithTunableLocalityClient.cpp
	${MKDIR} -p ${OBJECTDIR}/${VP2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${VP2}/TwoChoicePPWithTunableLocalityClient.o ${VP2}/TwoChoicePPWithTunableLocalityClient.cpp

${OBJECTDIR}/${VP2}/TwoChoicePPWithTunableLocalityServer.o: ${VP2}/TwoChoicePPWithTunableLocalityServer.cpp
	${MKDIR} -p ${OBJECTDIR}/${VP2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${VP2}/TwoChoicePPWithTunableLocalityServer.o ${VP2}/TwoChoicePPWithTunableLocalityServer.cpp

${OBJECTDIR}/${VP2}/TwoChoicePPWithTunableLocalityStorage.o: ${VP2}/TwoChoicePPWithTunableLocalityStorage.cpp
	${MKDIR} -p ${OBJECTDIR}/${VP2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${VP2}/TwoChoicePPWithTunableLocalityStorage.o ${VP2}/TwoChoicePPWithTunableLocalityStorage.cpp

${OBJECTDIR}/${VP2}/Server.o: ${VP2}/Server.cpp
	${MKDIR} -p ${OBJECTDIR}/${VP2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${VP2}/Server.o ${VP2}/Server.cpp

${OBJECTDIR}/${VP2}/Storage.o: ${VP2}/Storage.cpp
	${MKDIR} -p ${OBJECTDIR}/${VP2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${VP2}/Storage.o ${VP2}/Storage.cpp

${OBJECTDIR}/${VP2}/Utilities.o: ${VP2}/Utilities.cpp
	${MKDIR} -p ${OBJECTDIR}/${VP2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${VP2}/Utilities.o ${VP2}/Utilities.cpp

${OBJECTDIR}/${VP2}/logger.o: ${VP2}/logger.cpp
	${MKDIR} -p ${OBJECTDIR}/${VP2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${VP2}/logger.o ${VP2}/logger.cpp

${OBJECTDIR}/${VP2}/utils.o: ${VP2}/utils.cpp
	${MKDIR} -p ${OBJECTDIR}/${VP2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${VP2}/utils.o ${VP2}/utils.cpp

${OBJECTDIR}/vp2main.o: vp2main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/vp2main.o vp2main.cpp

#v2##################################################

${OBJECTDIR}/${V2}/AES.o: ${V2}/AES.cpp
	${MKDIR} -p ${OBJECTDIR}/${V2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V2}/AES.o ${V2}/AES.cpp

${OBJECTDIR}/${V2}/Amortized.o: ${V2}/Amortized.cpp
	${MKDIR} -p ${OBJECTDIR}/${V2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V2}/Amortized.o ${V2}/Amortized.cpp

${V2OBJECTDIR}/OneChoiceServer.o: ${V2}/OneChoiceServer.cpp
	${MKDIR} -p ${OBJECTDIR}/${V2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V2}/OneChoiceServer.o ${V2}/OneChoiceServer.cpp

${V2OBJECTDIR}/OneChoiceStorage.o: ${V2}/OneChoiceStorage.cpp
	${MKDIR} -p ${OBJECTDIR}/${V2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V2}/OneChoiceStorage.o ${V2}/OneChoiceStorage.cpp

${OBJECTDIR}/${V2}/TwoChoiceWithTunableLocalityClient.o: ${V2}/TwoChoiceWithTunableLocalityClient.cpp
	${MKDIR} -p ${OBJECTDIR}/${V2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V2}/TwoChoiceWithTunableLocalityClient.o ${V2}/TwoChoiceWithTunableLocalityClient.cpp

${OBJECTDIR}/${V2}/TwoChoiceWithTunableLocalityServer.o: ${V2}/TwoChoiceWithTunableLocalityServer.cpp
	${MKDIR} -p ${OBJECTDIR}/${V2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V2}/TwoChoiceWithTunableLocalityServer.o ${V2}/TwoChoiceWithTunableLocalityServer.cpp

${OBJECTDIR}/${V2}/TwoChoiceWithTunableLocalityStorage.o: ${V2}/TwoChoiceWithTunableLocalityStorage.cpp
	${MKDIR} -p ${OBJECTDIR}/${V2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V2}/TwoChoiceWithTunableLocalityStorage.o ${V2}/TwoChoiceWithTunableLocalityStorage.cpp

${OBJECTDIR}/${V2}/Server.o: ${V2}/Server.cpp
	${MKDIR} -p ${OBJECTDIR}/${V2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V2}/Server.o ${V2}/Server.cpp

${OBJECTDIR}/${V2}/Storage.o: ${V2}/Storage.cpp
	${MKDIR} -p ${OBJECTDIR}/${V2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V2}/Storage.o ${V2}/Storage.cpp

${OBJECTDIR}/${V2}/Utilities.o: ${V2}/Utilities.cpp
	${MKDIR} -p ${OBJECTDIR}/${V2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V2}/Utilities.o ${V2}/Utilities.cpp

${OBJECTDIR}/${V2}/logger.o: ${V2}/logger.cpp
	${MKDIR} -p ${OBJECTDIR}/${V2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V2}/logger.o ${V2}/logger.cpp

${OBJECTDIR}/${V2}/utils.o: ${V2}/utils.cpp
	${MKDIR} -p ${OBJECTDIR}/${V2}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V2}/utils.o ${V2}/utils.cpp

${OBJECTDIR}/v2main.o: v2main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/v2main.o v2main.cpp






#v1##################################################

${OBJECTDIR}/${V1}/AES.o: ${V1}/AES.cpp
	${MKDIR} -p ${OBJECTDIR}/${V1}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V1}/AES.o ${V1}/AES.cpp

${OBJECTDIR}/${V1}/Amortized.o: ${V1}/Amortized.cpp
	${MKDIR} -p ${OBJECTDIR}/${V1}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V1}/Amortized.o ${V1}/Amortized.cpp

${V1OBJECTDIR}/OneChoiceServer.o: ${V1}/OneChoiceServer.cpp
	${MKDIR} -p ${OBJECTDIR}/${V1}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V1}/OneChoiceServer.o ${V1}/OneChoiceServer.cpp

${V1OBJECTDIR}/OneChoiceStorage.o: ${V1}/OneChoiceStorage.cpp
	${MKDIR} -p ${OBJECTDIR}/${V1}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V1}/OneChoiceStorage.o ${V1}/OneChoiceStorage.cpp

${OBJECTDIR}/${V1}/TwoChoiceClient.o: ${V1}/TwoChoiceClient.cpp
	${MKDIR} -p ${OBJECTDIR}/${V1}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V1}/TwoChoiceClient.o ${V1}/TwoChoiceClient.cpp

${OBJECTDIR}/${V1}/TwoChoiceServer.o: ${V1}/TwoChoiceServer.cpp
	${MKDIR} -p ${OBJECTDIR}/${V1}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V1}/TwoChoiceServer.o ${V1}/TwoChoiceServer.cpp

${OBJECTDIR}/${V1}/TwoChoiceStorage.o: ${V1}/TwoChoiceStorage.cpp
	${MKDIR} -p ${OBJECTDIR}/${V1}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V1}/TwoChoiceStorage.o ${V1}/TwoChoiceStorage.cpp

${OBJECTDIR}/${V1}/Server.o: ${V1}/Server.cpp
	${MKDIR} -p ${OBJECTDIR}/${V1}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V1}/Server.o ${V1}/Server.cpp

${OBJECTDIR}/${V1}/Storage.o: ${V1}/Storage.cpp
	${MKDIR} -p ${OBJECTDIR}/${V1}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V1}/Storage.o ${V1}/Storage.cpp

${OBJECTDIR}/${V1}/Utilities.o: ${V1}/Utilities.cpp
	${MKDIR} -p ${OBJECTDIR}/${V1}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V1}/Utilities.o ${V1}/Utilities.cpp

${OBJECTDIR}/${V1}/logger.o: ${V1}/logger.cpp
	${MKDIR} -p ${OBJECTDIR}/${V1}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V1}/logger.o ${V1}/logger.cpp

${OBJECTDIR}/${V1}/utils.o: ${V1}/utils.cpp
	${MKDIR} -p ${OBJECTDIR}/${V1}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${V1}/utils.o ${V1}/utils.cpp

${OBJECTDIR}/v1main.o: v1main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/v1main.o v1main.cpp





#two1###############################################################################

${OBJECTDIR}/${TWOONE}/AES.o: ${TWOONE}/AES.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWOONE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWOONE}/AES.o ${TWOONE}/AES.cpp

${OBJECTDIR}/${TWOONE}/Amortized.o: ${TWOONE}/Amortized.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWOONE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWOONE}/Amortized.o ${TWOONE}/Amortized.cpp

${TWOONEOBJECTDIR}/OneChoiceServer.o: ${TWOONE}/OneChoiceServer.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWOONE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWOONE}/OneChoiceServer.o ${TWOONE}/OneChoiceServer.cpp


${TWOONEOBJECTDIR}/OneChoiceStorage.o: ${TWOONE}/OneChoiceStorage.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWOONE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWOONE}/OneChoiceStorage.o ${TWOONE}/OneChoiceStorage.cpp

${OBJECTDIR}/${TWOONE}/TwoChoiceClient.o: ${TWOONE}/TwoChoiceClient.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWOONE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWOONE}/TwoChoiceClient.o ${TWOONE}/TwoChoiceClient.cpp

${OBJECTDIR}/${TWOONE}/TwoChoiceServer.o: ${TWOONE}/TwoChoiceServer.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWOONE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWOONE}/TwoChoiceServer.o ${TWOONE}/TwoChoiceServer.cpp

${OBJECTDIR}/${TWOONE}/TwoChoiceStorage.o: ${TWOONE}/TwoChoiceStorage.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWOONE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWOONE}/TwoChoiceStorage.o ${TWOONE}/TwoChoiceStorage.cpp


${OBJECTDIR}/${TWOONE}/Server.o: ${TWOONE}/Server.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWOONE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWOONE}/Server.o ${TWOONE}/Server.cpp

${OBJECTDIR}/${TWOONE}/Storage.o: ${TWOONE}/Storage.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWOONE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWOONE}/Storage.o ${TWOONE}/Storage.cpp

${OBJECTDIR}/${TWOONE}/Utilities.o: ${TWOONE}/Utilities.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWOONE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWOONE}/Utilities.o ${TWOONE}/Utilities.cpp

${OBJECTDIR}/${TWOONE}/logger.o: ${TWOONE}/logger.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWOONE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWOONE}/logger.o ${TWOONE}/logger.cpp

${OBJECTDIR}/twoonemain.o: twoonemain.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/twoonemain.o twoonemain.cpp

${OBJECTDIR}/${TWOONE}/utils.o: ${TWOONE}/utils.cpp
	${MKDIR} -p ${OBJECTDIR}/${TWOONE}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${TWOONE}/utils.o ${TWOONE}/utils.cpp



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


###########################SDd+No OMAP+1choice######################################################

${SDDNOOBJECTDIR}/AES.o: ${SDDNO}/AES.cpp
	${MKDIR} -p ${SDDNOOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDNOOBJECTDIR}/AES.o ${SDDNO}/AES.cpp

${SDDNOOBJECTDIR}/DeAmortized.o: ${SDDNO}/DeAmortized.cpp
	${MKDIR} -p ${SDDNOOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDNOOBJECTDIR}/DeAmortized.o ${SDDNO}/DeAmortized.cpp

${SDDNOOBJECTDIR}/OneChoiceClient.o: ${SDDNO}/OneChoiceClient.cpp
	${MKDIR} -p ${OBJECTDIR}/${SDDNO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDNOOBJECTDIR}/OneChoiceClient.o ${SDDNO}/OneChoiceClient.cpp

${SDDNOOBJECTDIR}/OneChoiceServer.o: ${SDDNO}/OneChoiceServer.cpp
	${MKDIR} -p ${OBJECTDIR}/${SDDNO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDNOOBJECTDIR}/OneChoiceServer.o ${SDDNO}/OneChoiceServer.cpp

${SDDNOOBJECTDIR}/OneChoiceStorage.o: ${SDDNO}/OneChoiceStorage.cpp
	${MKDIR} -p ${OBJECTDIR}/${SDDNO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDNOOBJECTDIR}/OneChoiceStorage.o ${SDDNO}/OneChoiceStorage.cpp

${SDDNOOBJECTDIR}/Storage.o: ${SDDNO}/Storage.cpp
	${MKDIR} -p ${OBJECTDIR}/${SDDNO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDNOOBJECTDIR}/Storage.o ${SDDNO}/Storage.cpp

${SDDNOOBJECTDIR}/Utilities.o: ${SDDNO}/Utilities.cpp
	${MKDIR} -p ${OBJECTDIR}/${SDDNO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDNOOBJECTDIR}/Utilities.o ${SDDNO}/Utilities.cpp

${SDDNOOBJECTDIR}/logger.o: ${SDDNO}/logger.cpp
	${MKDIR} -p ${OBJECTDIR}/${SDDNO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${SDDNO}/logger.o ${SDDNO}/logger.cpp

${OBJECTDIR}/sddnmain.o: sddnmain.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sddnmain.o sddnmain.cpp

${OBJECTDIR}/${SDDNO}/utils.o: ${SDDNO}/utils.cpp
	${MKDIR} -p ${OBJECTDIR}/${SDDNO}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/${SDDNO}/utils.o ${SDDNO}/utils.cpp

${SDDNOOBJECTDIR}/Bid.o: ${SDDNO}/Bid.cpp
	${MKDIR} -p ${SDDNOOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDNOOBJECTDIR}/Bid.o ${SDDNO}/Bid.cpp


${SDDNOOBJECTDIR}/RAMStore.o: ${SDDNO}/RAMStore.cpp
	${MKDIR} -p ${SDDNOOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDNOOBJECTDIR}/RAMStore.o ${SDDNO}/RAMStore.cpp

${SDDNOOBJECTDIR}/AVLTree.o: ${SDDNO}/AVLTree.cpp
	${MKDIR} -p ${SDDNOOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDNOOBJECTDIR}/AVLTree.o ${SDDNO}/AVLTree.cpp

${SDDNOOBJECTDIR}/OMAP.o: ${SDDNO}/OMAP.cpp
	${MKDIR} -p ${SDDNOOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDNOOBJECTDIR}/OMAP.o ${SDDNO}/OMAP.cpp


${SDDNOOBJECTDIR}/ORAM.o: ${SDDNO}/ORAM.cpp
	${MKDIR} -p ${SDDNOOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDNOOBJECTDIR}/ORAM.o ${SDDNO}/ORAM.cpp

#####################SDd+OMAP+1choice######################################################

${SDDOOBJECTDIR}/AES.o: ${SDDO}/AES.cpp
	${MKDIR} -p ${SDDOOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDOOBJECTDIR}/AES.o ${SDDO}/AES.cpp

${SDDOOBJECTDIR}/DeAmortized.o: ${SDDO}/DeAmortized.cpp
	${MKDIR} -p ${SDDOOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDOOBJECTDIR}/DeAmortized.o ${SDDO}/DeAmortized.cpp

${SDDOOBJECTDIR}/Bitonic.o: ${SDDO}/Bitonic.cpp
	${MKDIR} -p ${SDDOOBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/openssl -std=c++14 -MMD -MP -MF "$@.d" -o ${SDDOOBJECTDIR}/Bitonic.o ${SDDO}/Bitonic.cpp

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
