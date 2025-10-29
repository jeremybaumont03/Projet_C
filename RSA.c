#include "RSA.h"
#include "modexp.h"
#include "pgcd.h"
#include <stdint.h>
#include <stdio.h>

// Extended Euclid for modular inverse (returns inverse of a mod m, or 0 if none)
static uint64_t modinv(uint64_t a, uint64_t m) {
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)m, newr = (int64_t)(a % m);
    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp = newt;
        newt = t - q * newt;
        t = tmp;
        tmp = newr;
        newr = r - q * newr;
        r = tmp;
    }
    if (r > 1) return 0; // not invertible
    if (t < 0) t += m;
    return (uint64_t)t;
}

RSAKey rsa_generate(uint64_t p, uint64_t q, uint64_t e_candidate) {
    RSAKey k = {0,0,0};
    if (p < 2 || q < 2) return k;
    uint64_t n = p * q;
    uint64_t phi = (p - 1) * (q - 1);

    // choose e: use e_candidate if coprime to phi, otherwise find small odd e
    uint64_t e = e_candidate;
    if (gcd_euclid(e, phi) != 1) {
        // find small e starting at 3
        e = 3;
        while (e < phi && gcd_euclid(e, phi) != 1) e += 2;
    }

    uint64_t d = modinv(e, phi);
    if (d == 0) {
        // failed to find inverse
        k.n = n; k.e = e; k.d = 0; return k;
    }
    k.n = n; k.e = e; k.d = d;
    return k;
}

uint64_t rsa_encrypt(uint64_t message, const RSAKey *k) {
    return modexp(message, k->e, k->n);
}

uint64_t rsa_decrypt(uint64_t cipher, const RSAKey *k) {
    return modexp(cipher, k->d, k->n);
}
