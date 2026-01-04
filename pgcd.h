#ifndef PGCD_H
#define PGCD_H

// Inclusion nécessaire pour utiliser le type BigBinary dans les paramètres
#include "bigbinary.h"

// --- PHASE 2 : ARITHMÉTIQUE AVANCÉE ---

// Calcule le Plus Grand Commun Diviseur (PGCD) de deux grands entiers A et B.
// Méthode : Algorithme d'Euclide Binaire.
// Intérêt : Contrairement à Euclide classique, cette méthode n'utilise PAS de division complexe.
//           Elle utilise uniquement des soustractions, des tests de parité (pair/impair)
//           et des décalages (divisions par 2), ce qui est ultra-rapide pour les ordinateurs.
BigBinary BigBinary_PGCD(BigBinary A, BigBinary B);

#endif