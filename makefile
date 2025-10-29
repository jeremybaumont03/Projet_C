CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

SRC = main.c bigbinary.c pgcd.c modexp.c RSA.c
OBJ = $(SRC:.c=.o)
EXEC = projet

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
