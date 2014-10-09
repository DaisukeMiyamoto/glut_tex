# Makefile

CC     = gcc
CFLAGS = -Wall -O2
LDLIBS = -lglut -lGLU -lGL -lm
INCLUDES = glut_wrapper.h
OBJS   = glut_wrapper.o test_glut.o
PROG   = test_texture


all: $(PROG)

$(PROG): $(OBJS)
	cc -o $@ $(OBJS) $(LDLIBS)

*.o : $(INCLUDES)

.c.o:

clean:
	rm -fv $(OBJ) *~ *.o

