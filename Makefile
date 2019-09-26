#CROSS_COMPILE ?= arm-linux-gnueabihf-
CC      = $(CROSS_COMPILE)g++
AR      = $(CROSS_COMPILE)ar
RM 		= rm -f
AR		= ar crsv
MV		= mv
SYS_AP_NAME = Lch

CPPS = 	cLch.cpp 
		
#CPPFLAGS = -c -g -v -Wall -O2 -Wno-reorder -Wno-unused-result -finline-functions -fopenmp 
CPPFLAGS = -c -g -v -Wall -O2 -fPIC -Wno-reorder -Wno-unused-result -finline-functions -fopenmp 
#CPPFLAGS += -DLiux 
#SYS_INCLUDE =../../ 
#CPPFLAGS += -iquote $(SYS_INCLUDE)

#LINK_LIB = -lrt -lm -ldl -lpthread -fopenmp
#LINK_LIB = -shared -lm -lpthread -fopenmp

allgen : ${SYS_AP_NAME}

${SYS_AP_NAME} : Lch
	${CC} ${CPPFLAGS} ${CPPS}
	#${AR} ${SYS_AP_NAME}.a *.o	
	${MV} *.o ../Lib
clean :
	${RM} ../Lib/*.o

