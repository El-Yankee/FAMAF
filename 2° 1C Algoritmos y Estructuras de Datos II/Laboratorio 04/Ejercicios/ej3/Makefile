CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -g

SRC = list.c array_helpers.c main.c
EXEC = average

all: clean link

objetos:
	$(CC) $(CFLAGS) -c $(SRC)

link: objetos
	$(CC) $(CFLAGS) *.o -o $(EXEC)
        
run: link
	./$(EXEC) input/example-sorted.in

memory_check:
	valgrind ./$(EXEC) input/example-sorted.in

clean:
	@rm -f *.o $(EXEC)









	