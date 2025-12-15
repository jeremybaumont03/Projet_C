# Makefile pour BigDigits - ESIEA
CC = gcc
CFLAGS = -Wall -Wextra -g -I.

# Nom de l'exécutable
TARGET = BigDigitsApp

# Sources et Objets
SRCS = main.c bigbinary.c multiplication.c pgcd.c modexp.c RSA.c
OBJS = $(SRCS:.c=.o)

# Règle principale
$(TARGET): $(OBJS)
	@echo "Creating executable..."
	$(CC) $(OBJS) -o $(TARGET)
	@echo "Done! Run using ./$(TARGET)"

# Compilation des .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)