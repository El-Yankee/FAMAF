CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -g

SRC = array_helpers.c main.c
EXEC = reader

all: clean link

objetos:
	$(CC) $(CFLAGS) -c $(SRC)

link: objetos
	$(CC) $(CFLAGS) *.o -o $(EXEC)
        
run: link
	./$(EXEC) ../input/example-easy.in

memory_check:
	valgrind ./$(EXEC) ../input/example-easy.in

clean:
	@rm -f *.o $(EXEC)











	