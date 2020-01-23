#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/RobotProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/RobotProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../PrcLib/AMB2300.c ../PrcLib/AnaIn.c ../PrcLib/clock.c ../PrcLib/DigIn.c ../PrcLib/LEDs.c ../PrcLib/Motors.c ../PrcLib/PRC.c ../PrcLib/uart1.c main.c realTimeTasks.c protocol.c interpreter.c backgroundTasks.c mission.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1811990669/AMB2300.o ${OBJECTDIR}/_ext/1811990669/AnaIn.o ${OBJECTDIR}/_ext/1811990669/clock.o ${OBJECTDIR}/_ext/1811990669/DigIn.o ${OBJECTDIR}/_ext/1811990669/LEDs.o ${OBJECTDIR}/_ext/1811990669/Motors.o ${OBJECTDIR}/_ext/1811990669/PRC.o ${OBJECTDIR}/_ext/1811990669/uart1.o ${OBJECTDIR}/main.o ${OBJECTDIR}/realTimeTasks.o ${OBJECTDIR}/protocol.o ${OBJECTDIR}/interpreter.o ${OBJECTDIR}/backgroundTasks.o ${OBJECTDIR}/mission.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1811990669/AMB2300.o.d ${OBJECTDIR}/_ext/1811990669/AnaIn.o.d ${OBJECTDIR}/_ext/1811990669/clock.o.d ${OBJECTDIR}/_ext/1811990669/DigIn.o.d ${OBJECTDIR}/_ext/1811990669/LEDs.o.d ${OBJECTDIR}/_ext/1811990669/Motors.o.d ${OBJECTDIR}/_ext/1811990669/PRC.o.d ${OBJECTDIR}/_ext/1811990669/uart1.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/realTimeTasks.o.d ${OBJECTDIR}/protocol.o.d ${OBJECTDIR}/interpreter.o.d ${OBJECTDIR}/backgroundTasks.o.d ${OBJECTDIR}/mission.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1811990669/AMB2300.o ${OBJECTDIR}/_ext/1811990669/AnaIn.o ${OBJECTDIR}/_ext/1811990669/clock.o ${OBJECTDIR}/_ext/1811990669/DigIn.o ${OBJECTDIR}/_ext/1811990669/LEDs.o ${OBJECTDIR}/_ext/1811990669/Motors.o ${OBJECTDIR}/_ext/1811990669/PRC.o ${OBJECTDIR}/_ext/1811990669/uart1.o ${OBJECTDIR}/main.o ${OBJECTDIR}/realTimeTasks.o ${OBJECTDIR}/protocol.o ${OBJECTDIR}/interpreter.o ${OBJECTDIR}/backgroundTasks.o ${OBJECTDIR}/mission.o

# Source Files
SOURCEFILES=../PrcLib/AMB2300.c ../PrcLib/AnaIn.c ../PrcLib/clock.c ../PrcLib/DigIn.c ../PrcLib/LEDs.c ../PrcLib/Motors.c ../PrcLib/PRC.c ../PrcLib/uart1.c main.c realTimeTasks.c protocol.c interpreter.c backgroundTasks.c mission.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/RobotProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ256GP710
MP_LINKER_FILE_OPTION=,--script=p33FJ256GP710.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1811990669/AMB2300.o: ../PrcLib/AMB2300.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1811990669" 
	@${RM} ${OBJECTDIR}/_ext/1811990669/AMB2300.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811990669/AMB2300.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../PrcLib/AMB2300.c  -o ${OBJECTDIR}/_ext/1811990669/AMB2300.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1811990669/AMB2300.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1811990669/AMB2300.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1811990669/AnaIn.o: ../PrcLib/AnaIn.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1811990669" 
	@${RM} ${OBJECTDIR}/_ext/1811990669/AnaIn.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811990669/AnaIn.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../PrcLib/AnaIn.c  -o ${OBJECTDIR}/_ext/1811990669/AnaIn.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1811990669/AnaIn.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1811990669/AnaIn.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1811990669/clock.o: ../PrcLib/clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1811990669" 
	@${RM} ${OBJECTDIR}/_ext/1811990669/clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811990669/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../PrcLib/clock.c  -o ${OBJECTDIR}/_ext/1811990669/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1811990669/clock.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1811990669/clock.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1811990669/DigIn.o: ../PrcLib/DigIn.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1811990669" 
	@${RM} ${OBJECTDIR}/_ext/1811990669/DigIn.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811990669/DigIn.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../PrcLib/DigIn.c  -o ${OBJECTDIR}/_ext/1811990669/DigIn.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1811990669/DigIn.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1811990669/DigIn.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1811990669/LEDs.o: ../PrcLib/LEDs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1811990669" 
	@${RM} ${OBJECTDIR}/_ext/1811990669/LEDs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811990669/LEDs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../PrcLib/LEDs.c  -o ${OBJECTDIR}/_ext/1811990669/LEDs.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1811990669/LEDs.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1811990669/LEDs.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1811990669/Motors.o: ../PrcLib/Motors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1811990669" 
	@${RM} ${OBJECTDIR}/_ext/1811990669/Motors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811990669/Motors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../PrcLib/Motors.c  -o ${OBJECTDIR}/_ext/1811990669/Motors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1811990669/Motors.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1811990669/Motors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1811990669/PRC.o: ../PrcLib/PRC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1811990669" 
	@${RM} ${OBJECTDIR}/_ext/1811990669/PRC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811990669/PRC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../PrcLib/PRC.c  -o ${OBJECTDIR}/_ext/1811990669/PRC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1811990669/PRC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1811990669/PRC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1811990669/uart1.o: ../PrcLib/uart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1811990669" 
	@${RM} ${OBJECTDIR}/_ext/1811990669/uart1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811990669/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../PrcLib/uart1.c  -o ${OBJECTDIR}/_ext/1811990669/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1811990669/uart1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1811990669/uart1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/realTimeTasks.o: realTimeTasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/realTimeTasks.o.d 
	@${RM} ${OBJECTDIR}/realTimeTasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  realTimeTasks.c  -o ${OBJECTDIR}/realTimeTasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/realTimeTasks.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/realTimeTasks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/protocol.o: protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/protocol.o.d 
	@${RM} ${OBJECTDIR}/protocol.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  protocol.c  -o ${OBJECTDIR}/protocol.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/protocol.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/protocol.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/interpreter.o: interpreter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/interpreter.o.d 
	@${RM} ${OBJECTDIR}/interpreter.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  interpreter.c  -o ${OBJECTDIR}/interpreter.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/interpreter.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/interpreter.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/backgroundTasks.o: backgroundTasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/backgroundTasks.o.d 
	@${RM} ${OBJECTDIR}/backgroundTasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  backgroundTasks.c  -o ${OBJECTDIR}/backgroundTasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/backgroundTasks.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/backgroundTasks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mission.o: mission.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mission.o.d 
	@${RM} ${OBJECTDIR}/mission.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mission.c  -o ${OBJECTDIR}/mission.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mission.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/mission.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1811990669/AMB2300.o: ../PrcLib/AMB2300.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1811990669" 
	@${RM} ${OBJECTDIR}/_ext/1811990669/AMB2300.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811990669/AMB2300.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../PrcLib/AMB2300.c  -o ${OBJECTDIR}/_ext/1811990669/AMB2300.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1811990669/AMB2300.o.d"        -g -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1811990669/AMB2300.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1811990669/AnaIn.o: ../PrcLib/AnaIn.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1811990669" 
	@${RM} ${OBJECTDIR}/_ext/1811990669/AnaIn.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811990669/AnaIn.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../PrcLib/AnaIn.c  -o ${OBJECTDIR}/_ext/1811990669/AnaIn.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1811990669/AnaIn.o.d"        -g -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1811990669/AnaIn.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1811990669/clock.o: ../PrcLib/clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1811990669" 
	@${RM} ${OBJECTDIR}/_ext/1811990669/clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811990669/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../PrcLib/clock.c  -o ${OBJECTDIR}/_ext/1811990669/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1811990669/clock.o.d"        -g -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1811990669/clock.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1811990669/DigIn.o: ../PrcLib/DigIn.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1811990669" 
	@${RM} ${OBJECTDIR}/_ext/1811990669/DigIn.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811990669/DigIn.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../PrcLib/DigIn.c  -o ${OBJECTDIR}/_ext/1811990669/DigIn.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1811990669/DigIn.o.d"        -g -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1811990669/DigIn.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1811990669/LEDs.o: ../PrcLib/LEDs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1811990669" 
	@${RM} ${OBJECTDIR}/_ext/1811990669/LEDs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811990669/LEDs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../PrcLib/LEDs.c  -o ${OBJECTDIR}/_ext/1811990669/LEDs.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1811990669/LEDs.o.d"        -g -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1811990669/LEDs.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1811990669/Motors.o: ../PrcLib/Motors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1811990669" 
	@${RM} ${OBJECTDIR}/_ext/1811990669/Motors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811990669/Motors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../PrcLib/Motors.c  -o ${OBJECTDIR}/_ext/1811990669/Motors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1811990669/Motors.o.d"        -g -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1811990669/Motors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1811990669/PRC.o: ../PrcLib/PRC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1811990669" 
	@${RM} ${OBJECTDIR}/_ext/1811990669/PRC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811990669/PRC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../PrcLib/PRC.c  -o ${OBJECTDIR}/_ext/1811990669/PRC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1811990669/PRC.o.d"        -g -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1811990669/PRC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1811990669/uart1.o: ../PrcLib/uart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1811990669" 
	@${RM} ${OBJECTDIR}/_ext/1811990669/uart1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811990669/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../PrcLib/uart1.c  -o ${OBJECTDIR}/_ext/1811990669/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1811990669/uart1.o.d"        -g -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1811990669/uart1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"        -g -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/realTimeTasks.o: realTimeTasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/realTimeTasks.o.d 
	@${RM} ${OBJECTDIR}/realTimeTasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  realTimeTasks.c  -o ${OBJECTDIR}/realTimeTasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/realTimeTasks.o.d"        -g -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/realTimeTasks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/protocol.o: protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/protocol.o.d 
	@${RM} ${OBJECTDIR}/protocol.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  protocol.c  -o ${OBJECTDIR}/protocol.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/protocol.o.d"        -g -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/protocol.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/interpreter.o: interpreter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/interpreter.o.d 
	@${RM} ${OBJECTDIR}/interpreter.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  interpreter.c  -o ${OBJECTDIR}/interpreter.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/interpreter.o.d"        -g -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/interpreter.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/backgroundTasks.o: backgroundTasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/backgroundTasks.o.d 
	@${RM} ${OBJECTDIR}/backgroundTasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  backgroundTasks.c  -o ${OBJECTDIR}/backgroundTasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/backgroundTasks.o.d"        -g -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/backgroundTasks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mission.o: mission.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mission.o.d 
	@${RM} ${OBJECTDIR}/mission.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mission.c  -o ${OBJECTDIR}/mission.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mission.o.d"        -g -omf=elf -legacy-libc -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/mission.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/RobotProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/RobotProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -legacy-libc  -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/RobotProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/RobotProject.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -legacy-libc -Wl,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/RobotProject.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
