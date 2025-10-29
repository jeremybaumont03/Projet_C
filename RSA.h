#ifndef RSA_H
#define RSA_H

#include <stdint.h>

// Cle RSA simple (valeurs sur 64-bit pour l'exemple)
typedef struct {
    uint64_t n;
    uint64_t e;
    uint64_t d;
} RSAKey;

// Générer une paire de clés simples à partir de p et q (petits premiers)
// Retourne RSAKey (n, e, d). Si erreur, d == 0.
RSAKey rsa_generate(uint64_t p, uint64_t q, uint64_t e_candidate);

// Chiffrement / déchiffrement (message < n)
uint64_t rsa_encrypt(uint64_t message, const RSAKey *k);
uint64_t rsa_decrypt(uint64_t cipher, const RSAKey *k);

#endif // RSA_H
