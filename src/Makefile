
#
# Generic Makefile to make one OpenGL based executable.
# Use GNU make .
#

SHELL	= /bin/sh

SOURCES = main.c calendar.c picking.c calendar_primitives.c \
	  effect1.c effect2.c effect3.c effect4.c effect5.c effect6.c \
	  effect7.c effect8.c effect9.c effect10.c effect11.c effect12.c \
	  effect13.c effect14.c effect15.c effect16.c effect17.c effect18.c \
	  effect19.c effect20.c effect21.c effect22.c effect23.c effect24.c \
	  texture.c messages.c pause.c \
	  stars.c vectormath.c util.c quit.c util/readtex.c
TARGET  = calendar
OBJECTS = $(SOURCES:.c=.o)

CC	= gcc
CFLAGS	= -O2 -g -Wall
LDFLAGS =
LDLIBS	= -lglut -lGLU -lGL -lm


all: $(TARGET)
.PHONY: all


$(OBJECTS): %.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(TARGET): $(OBJECTS) 
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

.PHONY: clean
clean:
	-rm -f core \#*\# *~ *.o $(OBJECTS) $(TARGET)
