#ifndef MODEXP_H
#define MODEXP_H

#include "bigbinary.h"
#include "multiplication.h" // Nécessaire car l'exponentiation utilise la multiplication

// --- PHASE 2 : CALCULS MODULAIRES ---

// Calcule le reste de la division entière (A % N).
// Fonction essentielle pour empêcher les nombres de devenir infinis.
// Elle réduit le résultat à une taille inférieure à N.
BigBinary Modulo(BigBinary A, BigBinary N);

// Calcule l'exponentiation modulaire : (A^e) % N.
// Algorithme "Square and Multiply".
// C'est le moteur du RSA : permet de calculer des puissances géantes très rapidement.
BigBinary ExponentiationRapideModulaire(BigBinary A, unsigned int e, BigBinary N);

#endif