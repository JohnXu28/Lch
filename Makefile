include ../Makefile.inc

CPPS = clch.cpp

#OBJS : replace *.cpp to *.o
OBJS=$(CPPS:.cpp=.o)

all: $(OBJS)

CPPFLAGS += -fPIC 

#================================================================
.PHONY:test
%.o : %.cpp
	${CXX} ${CPPFLAGS} -o $@ $<
	${CP} *.o ../Lib
	@echo "*****************************Lch:test************************"


#================================================================
.PHONY:release
release:
	${CXX} ${CPPFLAGS} ${CPPS}
	${CP} *.o ../Lib
	@echo "*****************************Lch:Release************************"


#================================================================
clean :
	${RM} *.o
	@echo "*****************************Lch:Clean************************"
