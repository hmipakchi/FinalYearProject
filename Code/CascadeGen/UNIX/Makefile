#
# Makefile
#

CC = g++
CPPFLAGS = -c
LFLAGS = -o $@
DEPENDENCIES = utility.o cascade.o tweet_analyser.o tweet_parser.o tweet.o main.o
PROGRAM = cascadeGen

# FINAL PROGRAM

${PROGRAM} : ${DEPENDENCIES}
	${CC} ${LFLAGS} ${DEPENDENCIES}

# MODULES

utility.o : utility.cpp utility.h
	${CC} ${CPPFLAGS} utility.cpp

cascade.o : cascade.cpp cascade.h
	${CC} ${CPPFLAGS} cascade.cpp

tweet_analyser.o : tweet_analyser.cpp tweet_analyser.h
	
	${CC} ${CPPFLAGS} tweet_analyser.cpp

tweet_parser.o : tweet_parser.cpp tweet_parser.h
	${CC} ${CPPFLAGS} tweet_parser.cpp

tweet.o : tweet.cpp tweet.h
	${CC} ${CPPFLAGS} tweet.cpp

main.o : main.cpp
	${CC} ${CPPFLAGS} main.cpp

# CLEAN

clean:
	-rm main ${DEPENDENCIES}
	-rm main ${PROGRAM}