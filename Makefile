OBJS    = words.o
SOURCE    = words.c
HEADER    = 
OUT    = words
CC     = gcc
FLAGS     = -g -c -Wall
LFLAGS     = 

all: $(OBJS)
    $(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

words.o: words.c
    $(CC) $(FLAGS) words.c 


clean:
    rm -f $(OBJS) $(OUT)
