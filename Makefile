CFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter -fsanitize=address
CC = gcc

PROGRAM = test_graphes

all: $(PROGRAM)

$(PROGRAM): main.o graphes.o
	$(CC) -o $(PROGRAM) $(CFLAGS) main.o graphes.o

main.o: main.c graphes.h
	$(CC) $(CFLAGS) -c main.c

graphes.o: graphes.c graphes.h
	$(CC) $(CFLAGS) -c graphes.c

clean:
	rm -f *.o *~ $(PROGRAM)

.PHONY: all clean run

run: $(PROGRAM)
	./$(PROGRAM)

