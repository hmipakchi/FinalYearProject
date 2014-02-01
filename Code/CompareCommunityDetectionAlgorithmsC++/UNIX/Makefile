#
# Makefile
#

CC = g++
CPPFLAGS = -c
LFLAGS = -o $@
DEPENDENCIES = utility.o compare_results.o
PROGRAM = compare_results

# FINAL PROGRAM

${PROGRAM} : ${DEPENDENCIES}
	${CC} ${LFLAGS} ${DEPENDENCIES}

# MODULES

utility.o : utility.cpp utility.h
	${CC} ${CPPFLAGS} utility.cpp

compare_results.o : compare_results.cpp
	${CC} ${CPPFLAGS} compare_results.cpp

# CLEAN

clean:
	-rm main ${DEPENDENCIES}
	-rm main ${PROGRAM}