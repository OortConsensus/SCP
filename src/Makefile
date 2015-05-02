# define the C compiler to use
CC = gcc
CXX = g++
CFLAGS = -Wall -g -std=c++11
# define any directories containing header files other than /usr/include, e.g. -I
INCLUDES =
# define library paths in addition to /usr/lib, e.g. -L
LFLAGS =
# define any libraries to link into executable:, e.g. -l
LIBS =
# define the C source files
SRCS = message.cpp transport.cpp transportDummyImpl.cpp slot.cpp node.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
MAIN = main

.PHONY: clean

all:$(MAIN)
	@echo  Stellar has been compiled

$(MAIN): $(OBJS) 
	$(CXX) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

