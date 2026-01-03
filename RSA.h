#ifndef RSA_H
#define RSA_H

// Inclusion du module d'exponentiation (car RSA n'est qu'une exponentiation modulaire)
#include "modexp.h"

// --- PHASE 3 : APPLICATION CRYPTOGRAPHIQUE (RSA) ---

// Fonction de Chiffrement RSA standard.
// Transforme un message clair M en message chiffré C.
// Formule : C = M^e mod N
// Entrées : M (Message), e (Clé publique, entier standard), N (Module)
BigBinary Chiffrement_RSA(BigBinary M, unsigned int e, BigBinary N);

// Fonction de Déchiffrement RSA.
// Retrouve le message clair M à partir du message chiffré C.
// Formule : M = C^d mod N
// Entrées : C (Chiffré), d (Clé privée, entier standard), N (Module)
BigBinary Dechiffrement_RSA(BigBinary C, unsigned int d, BigBinary N);

// --- BONUS : RSA AVANCÉ ---

// Exponentiation modulaire "Maison" qui gère un EXPOSANT de type BigBinary.
// Intérêt : Permet de simuler un vrai RSA avec des clés de 1024 bits ou plus
//           (alors que 'unsigned int' est limité à 32 ou 64 bits).
// C'est cette fonction qui permet de valider le "Bonus" du sujet.
BigBinary ExponentiationRapideModulaire_Maison(BigBinary M, BigBinary E, BigBinary N);

#endif