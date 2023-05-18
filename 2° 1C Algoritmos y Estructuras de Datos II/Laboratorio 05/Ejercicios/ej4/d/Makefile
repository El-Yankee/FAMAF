CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -g

SRC = clone.c
EXEC = clone

all: clean link

objetos:
	$(CC) $(CFLAGS) -c $(SRC)

link: objetos
	$(CC) $(CFLAGS) *.o -o $(EXEC)
        
run: link
	./$(EXEC)

memory_check:
	valgrind ./$(EXEC)

clean:
	@rm -f *.o $(EXEC)









	