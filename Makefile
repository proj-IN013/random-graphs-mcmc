CFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter #-fsanitize=address #-Wno-unused-variable
CC = gcc

PROGRAM = test_graphes

all: $(PROGRAM)

$(PROGRAM): main.o analyse.o graphes.o Pcolors.o
	$(CC) -o $(PROGRAM) $(CFLAGS) main.o analyse.o graphes.o Pcolors.o

main.o: main.c analyse.h graphes.h Pcolors.h
	$(CC) $(CFLAGS) -c main.c

analyse.o: analyse.c analyse.h
	$(CC) $(CFLAGS) -c analyse.c

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


