#ifndef BIGBINARY_H
#define BIGBINARY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int *Tdigits;
    int Taille;
    int Signe;
} BigBinary;

BigBinary initBigBinary(int taille, int signe);
BigBinary creerBigBinaryDepuisChaine(const char *chaine);
// NOUVEAU : Fonction pour convertir un entier normal en BigBinary
BigBinary creerBigBinaryDepuisEntier(unsigned long long n); 

void libereBigBinary(BigBinary *nb);
void afficheBigBinary(BigBinary nb);
BigBinary copieBigBinary(BigBinary A);

bool Egal(BigBinary A, BigBinary B);
bool Inferieur(BigBinary A, BigBinary B);
bool Superieur(BigBinary A, BigBinary B);
bool isOdd(BigBinary nb);

void divisePar2(BigBinary *nb);
BigBinary multipliePar2(BigBinary A);

BigBinary Addition(BigBinary A, BigBinary B);
BigBinary Soustraction(BigBinary A, BigBinary B);

#endif