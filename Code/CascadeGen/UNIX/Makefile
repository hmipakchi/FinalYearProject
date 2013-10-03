#
# Makefile
#

CC = g++
CPPFLAGS = -c
LFLAGS = -o $@
DEPENDENCIES = tweet_analyser.o tweet_parser.o tweet.o main.o

# FINAL PROGRAM

cascadeGen : ${DEPENDENCIES}
	${CC} ${LFLAGS} ${DEPENDENCIES}

# MODULES

tweet_analyser.o : tweet_analyser.cpp tweet_analyser.h
	${CC} ${CPPFLAGS} tweet_analyser.cpp

tweet_parser.o : tweet_parser.cpp tweet_parser.h
	${CC} ${CPPFLAGS} tweet_parser.cpp

tweet.o : tweet.cpp tweet.h
	${CC} ${CPPFLAGS} tweet.cpp

main.o : main.cpp tweet_analyser.h tweet_parser.h tweet.h
	${CC} ${CPPFLAGS} main.cpp

# CLEAN

clean:
	-rm main ${DEPENDENCIES}