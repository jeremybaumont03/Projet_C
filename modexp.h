#ifndef MODEXP_H
#define MODEXP_H
#include "bigbinary.h"
#include "multiplication.h" // Nécessaire pour le modulo et l'exp

// Phase 2 : Arithmétique Modulaire
BigBinary Modulo(BigBinary A, BigBinary N);
BigBinary ExponentiationRapideModulaire(BigBinary A, unsigned int e, BigBinary N);

#endif