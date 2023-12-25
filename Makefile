include ../Makefile.inc

CPPS = clch.cpp
OBJS = clch.o
all: $(OBJS)

CPPFLAGS += -fPIC 

#================================================================
.PHONY:release
release:
	${CC} ${CPPFLAGS} ${CPPS}
	${CP} *.o ../Lib
	@echo "*****************************Lch:Release************************"


#================================================================
clean :
	${RM} *.o
	@echo "*****************************Lch:Clean************************"
