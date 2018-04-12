LIBS		= -lev3c-cross
INCLUDES	= -Iinclude/
LFLAGS		= -Llib/
CC		= gcc
SRCS 		= $(wildcard *.c)
PROGS 		= $(patsubst %.c,%,$(SRCS))



all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) -o $@ $< $(LIBS) 

clean:
	rm -f $(PROGS)

send:
	scp $(PROGS) robot@ev3dev.local:/home/robot/

conn:
	ssh robot@ev3dev.local
