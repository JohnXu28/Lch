include ../Makefile.inc

CPPS = clch.cpp

#OBJS : replace *.cpp to *.o
OBJS=$(CPPS:.cpp=.o)

all: $(OBJS)

#================================================================
.PHONY:test
%.o : %.cpp
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o $@ $<
	${CP} *.o ../Lib
	@echo "*****************************Lch:test************************"


#================================================================
.PHONY:release
release:
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o $@ $<
	${CP} *.o ../Lib
	@echo "*****************************Lch:Release************************"


#================================================================
clean :
	${RM} *.o
	@echo "*****************************Lch:Clean************************"
