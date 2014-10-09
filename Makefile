# Makefile

CC       = gcc
CFLAGS   = -Wall -O2
LDLIBS   = -lglut -lGLU -lGL -lm
INCLUDES = glut_wrapper.h
SRC      = glut_wrapper.c test_glut.c
OBJS     = glut_wrapper.o test_glut.o
PROG     = test_glut


all: $(PROG)

$(PROG): $(OBJS)
	cc -o $@ $(OBJS) $(LDLIBS)

*.o : $(INCLUDES) $(SRC)

.c.o:

clean:
	rm -fv $(OBJ) *~ *.o

