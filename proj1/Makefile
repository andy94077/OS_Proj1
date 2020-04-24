CC=gcc
CFLAGS=-DDEBUG -Wall -std=c11 -Wextra
SRC=main.c scheduler.c process.c syscall.c
OBJ=$(SRC:.c=.o)
EXE_NAME=main

all:$(EXE_NAME)
$(EXE_NAME):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY:clean
clean:
	@rm -f $(EXE_NAME) $(OBJ)
