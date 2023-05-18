CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -g

SRC = strfuncs.c main.c
EXEC = main

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









	