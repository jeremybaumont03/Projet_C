# --- CONFIGURATION DU COMPILATEUR ---

# Compilateur utilisé (gcc est le standard pour le C)
CC = gcc

# Options de compilation :
# -Wall -Wextra : Active tous les avertissements (pour coder proprement)
# -g            : Ajoute des infos de débogage (utile pour gdb ou valgrind)
# -I.           : Cherche les fichiers .h dans le dossier courant
CFLAGS = -Wall -Wextra -g -I.

# --- FICHIERS DU PROJET ---

# Nom de l'exécutable final qui sera créé
TARGET = BigDigitsApp

# Liste de tous les fichiers sources (.c) du projet
SRCS = main.c bigbinary.c multiplication.c pgcd.c modexp.c RSA.c

# Liste des fichiers objets (.o)
# Cette ligne transforme automatiquement la liste des .c en .o
OBJS = $(SRCS:.c=.o)

# --- RÈGLES DE COMPILATION ---

# Règle par défaut (celle qui se lance quand on tape juste 'make')
# Elle dépend des fichiers objets $(OBJS)
$(TARGET): $(OBJS)
	@echo "--- Édition de liens (Création de l'exécutable) ---"
	$(CC) $(OBJS) -o $(TARGET)
	@echo "Succès ! Lancez le programme avec ./$(TARGET) ou 'make run'"

# Règle générique : Comment transformer n'importe quel .c en .o
# $< désigne le fichier source (.c)
# $@ désigne le fichier cible (.o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# --- UTILITAIRES ---

# Nettoyage : Supprime tous les fichiers générés (.o et l'exécutable)
# Utile pour forcer une recompilation propre
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Nettoyage terminé."

# Raccourci : Compile (si nécessaire) et lance le programme directement
run: $(TARGET)
	./$(TARGET)