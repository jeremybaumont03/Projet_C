#ifndef BIGBINARY_H
#define BIGBINARY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// --- STRUCTURE DE DONNÉES ---
// Représente un Grand Entier de taille arbitraire
typedef struct {
    int *Tdigits;  // Tableau dynamique contenant les bits (0 ou 1)
                   // Convention : Tdigits[0] = Bit de poids fort (MSB - Gauche)
    int Taille;    // Nombre de bits actuellement stockés
    int Signe;     // +1 (Positif), -1 (Négatif), 0 (Nul)
} BigBinary;

// --- GESTION DE LA MÉMOIRE (Création / Destruction) ---

// Alloue un BigBinary vide de la taille demandée (initialisé à 0)
BigBinary initBigBinary(int taille, int signe);

// Parse une chaine de caractères binaire (ex: "1011") vers un BigBinary
BigBinary creerBigBinaryDepuisChaine(const char *chaine);

// Convertit un entier standard (ex: 14) en BigBinary (ex: 1110)
// Utile pour simplifier la saisie utilisateur
BigBinary creerBigBinaryDepuisEntier(unsigned long long n); 

// Libère la mémoire du tableau Tdigits pour éviter les fuites mémoire
void libereBigBinary(BigBinary *nb);

// Affiche le nombre dans la console (ex: "1011")
void afficheBigBinary(BigBinary nb);

// Crée une copie profonde (indépendante) d'un nombre
// (Permet de modifier la copie sans toucher à l'original)
BigBinary copieBigBinary(BigBinary A);

// --- COMPARAISONS ---

// Renvoie true si A est strictement égal à B
bool Egal(BigBinary A, BigBinary B);

// Renvoie true si A est strictement inférieur à B (A < B)
bool Inferieur(BigBinary A, BigBinary B);

// Renvoie true si A est strictement supérieur à B (A > B)
bool Superieur(BigBinary A, BigBinary B);

// --- UTILITAIRES BINAIRES ---

// Renvoie true si le nombre est impair (dernier bit à 1)
bool isOdd(BigBinary nb);

// Division entière par 2 (Décalage vers la droite : supprime le dernier bit)
void divisePar2(BigBinary *nb);

// Multiplication par 2 (Décalage vers la gauche : ajoute un 0 à la fin)
BigBinary multipliePar2(BigBinary A);

// --- OPÉRATIONS ARITHMÉTIQUES (PHASE 1) ---

// Calcule A + B (supporte les retenues)
BigBinary Addition(BigBinary A, BigBinary B);

// Calcule A - B (Suppose A >= B pour ce projet)
BigBinary Soustraction(BigBinary A, BigBinary B);

#endif