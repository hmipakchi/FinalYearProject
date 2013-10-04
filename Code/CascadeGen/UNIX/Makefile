#
# Makefile
#

CC = g++
CPPFLAGS = -c
LFLAGS = -o $@
DEPENDENCIES = cascade.o tweet_analyser.o tweet_parser.o tweet.o main.o
PROGRAM = cascadeGen

# FINAL PROGRAM

${PROGRAM} : ${DEPENDENCIES}
	${CC} ${LFLAGS} ${DEPENDENCIES}

# MODULES

cascade.o : cascade.cpp cascade.h tweet.h
	${CC} ${CPPFLAGS} cascade.cpp

tweet_analyser.o : tweet_analyser.cpp tweet_analyser.h tweet.h cascade.h
	${CC} ${CPPFLAGS} tweet_analyser.cpp

tweet_parser.o : tweet_parser.cpp tweet_parser.h tweet.h
	${CC} ${CPPFLAGS} tweet_parser.cpp

tweet.o : tweet.cpp tweet.h
	${CC} ${CPPFLAGS} tweet.cpp

main.o : main.cpp tweet_analyser.h tweet_parser.h tweet.h cascade.h
	${CC} ${CPPFLAGS} main.cpp

# CLEAN

clean:
	-rm main ${DEPENDENCIES}
	-rm main ${PROGRAM}