CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -g

SRC = counter.c main.c
EXEC = counter

all: clean link

objetos:
	$(CC) $(CFLAGS) -c $(SRC)

link: objetos
	$(CC) $(CFLAGS) *.o -o $(EXEC)
        
run: link
	./$(EXEC) input/balanced.in

memory_check:
	valgrind ./$(EXEC) input/balanced.in

clean:
	@rm -f *.o $(EXEC)