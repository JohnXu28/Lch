include ../Makefile.inc

CPPS = 	cLch.cpp 
		
CPPFLAGS += -fPIC 

#================================================================
.PHONY:release
release : 
	${CC} ${CPPFLAGS} -O2 ${CPPS}
	${CP} *.o ../Lib

#================================================================
.PHONY:debug
debug : 
	${CC} ${CPPFLAGS} -O0 ${CPPS}
	${CP} *.o ../Lib

#================================================================
clean :
	${RM} *.o

