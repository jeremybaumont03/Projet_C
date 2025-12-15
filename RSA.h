#ifndef RSA_H
#define RSA_H
#include "modexp.h"

// Phase 3 : RSA
BigBinary Chiffrement_RSA(BigBinary M, unsigned int e, BigBinary N);
BigBinary Dechiffrement_RSA(BigBinary C, unsigned int d, BigBinary N);
BigBinary ExponentiationRapideModulaire_Maison(BigBinary M, BigBinary E, BigBinary N);

#endif