# Aaron Clauset
# Makefile  Oct2003
# feel free to hack this to pieces

#### local macros
# remove without fussing about it
RM = /bin/rm -f

# compiler name and flags
CCC = g++
CCFLAGS = -O3 -fomit-frame-pointer -funroll-loops -fforce-addr -fexpensive-optimizations -Wno-deprecated -g3

### local program information
EXEC=run
SOURCES=communityDetection.C 

### intermediate objects
OBJECTS = $(SOURCES: .cc=.o)

### libraries
LIBS += 

### headers
HEADERS = nr3.h ran.h ludcmp.h eigen_sym.h Headers.h edge.h node.h Helper.h readInputFile.h

### targets, dependencies and actions
$(EXEC): $(OBJECTS) Makefile
	$(LINK.cc) $(CCFLAGS) -o $(EXEC) $(OBJECTS) $(LIBS)

### sort out dependencies
depend:
	makedepend $(INCLUDES) $(HEADERS) $(SOURCES) $(LIBS)

clean: 
	$(RM) $(EXEC) *.o *~