CC = gcc 
CFLAGS = -Wall -Wextra -std=c11 

##release and debug flags 

#optimize and disables debug prints. 
RELEASE_CFLAGS = -O2 -DNDEBUG  

#include debug symbol and enables DEBUG_PRINT.
DEBUG_CFLAGS = -g -DDEBUG

#target library 
#statuc library we will build (libprocmon.a)
TARGET = libprocmon.a 

#file to complile into the library
SOURCES = proc_stauts.c 

#same files but compiled into .o (objects) files.
OBJECTS = $(SOURCES:.c=.o)

#header files that .c files depends on.
HEADER = proc_status.h

#default: release build

#ALL: default target when we type make
all: CFLAGS += $(RELEASE_CFLAGS)
all: $(TARGET)


#buld the same library but with debug flags
debug: CFLAGS += $(DEBUG_CFLAGS)
debug: $(TARGET)



#create static library from the object files 

## $@ -> the target name (libprocmon.a)
# $^ -> all dependencies (proc_status.o)
# ar rcs -> command to create a static library 
$(TARGET): $(OBJECTS)
	ar rcs $@ $^
	
	
# step: 4 compile object files 	

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@
	
#BUild the test program 

# -L -> look in the current directory for libraries 
# -lprocmon -> link with libprocmon.a
test: test_proc.c $(TARGET)	
	$(CC) $(CFLAGS) -o test_proc test_proc.c -L. -lprocmon

#BUild process monitor 
monitor: proc_mon.c socket_server.c socket_header.h $(TARGET)
	$(CC) $(CFLAGS) -o proc_mon proc_mon.c socket_server.c -L. -lprocmon


#A 'run ' target for convience

run: test 
	./test_proc



#step :6 	
#clean generated  files 



clean: 

#.phony tells make these are not actual files
#clean -> remove compiled filed so we can rebuild fresh
	rm -f $(TARGET) $(OBJECTS) test_proc proc_mon /tmp/proc_mon.sock
.PHONY: all debug clean test 
	
