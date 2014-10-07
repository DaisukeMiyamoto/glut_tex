# Makefile

CC     = gcc
CFLAGS = -Wall -O2
LDLIBS = -lglut -lGLU -lGL -lm
INCLUDES = test_glut.h
OBJS   = test_glut.o
PROG   = a.out


all: $(PROG)

$(PROG): $(OBJS)
	cc -o $@ $(OBJS) $(LDLIBS)

*.o : $(INCLUDES)

.c.o:

clean:
	rm -fv $(OBJ) *~ *.o

