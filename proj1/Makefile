CC=gcc
CFLAG=-DDEBUG -Wall -std=c99 -Wextra
SRC=main.c scheduler.c process.c
OBJ=$(SRC:.c=.o)
EXE_NAME=main

all: $(EXE_NAME)
$(EXE_NAME): $(OBJ)
	$(CC) $(CFLAG) $(OBJ) -o $@
%.o: %.c

.PHONY:clean
clean:
	@rm -f $(EXE_NAME) $(OBJ)
