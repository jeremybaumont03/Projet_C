#ifndef MULTIPLICATION_H
#define MULTIPLICATION_H

// Inclusion nécessaire car on utilise le type BigBinary dans les prototypes
#include "bigbinary.h"

// --- PHASE 2 : OPÉRATIONS AVANCÉES ---

// Déclaration de la fonction de Multiplication Égyptienne.
// Cette méthode permet de multiplier deux grands entiers uniquement
// avec des additions et des décalages (multiplication par 2, division par 2).
// Entrées : A et B (les deux nombres à multiplier)
// Sortie  : Un nouveau BigBinary contenant le résultat (A * B)
BigBinary Multiplication_Egyptienne(BigBinary A, BigBinary B);

#endif