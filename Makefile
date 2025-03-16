CFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter -fsanitize=address
CC = gcc

PROGRAM = test_graphes

all: $(PROGRAM)

$(PROGRAM): main.o graphes.o Pcolors.o
	$(CC) -o $(PROGRAM) $(CFLAGS) main.o graphes.o Pcolors.o

main.o: main.c graphes.h Pcolors.h
	$(CC) $(CFLAGS) -c main.c

graphes.o: graphes.c graphes.h
	$(CC) $(CFLAGS) -c graphes.c

Pcolors.o: Pcolors.c Pcolors.h
	$(CC) $(CFLAGS) -c Pcolors.c

clean:
	rm -f *.o *~ $(PROGRAM)

.PHONY: all clean run

run: $(PROGRAM)
	./$(PROGRAM)

total: clean
	make
	./$(PROGRAM)


