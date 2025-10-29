#ifndef BIGBINARY_H
#define BIGBINARY_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// BigBinary : représentation sur "limbs" de 32 bits (base = 2^32)
typedef struct {
    uint32_t *words;   // little-endian words: words[0] = least significant 32 bits
    size_t nwords;     // nombre de mots alloués/utilisés
    int sign;          // +1 positive, 0 zero, -1 negative (on travaille surtout en positif)
} BigBinary;

// Création / libération
BigBinary bb_zero();                             // crée 0
BigBinary bb_from_uint64(uint64_t v);            // créer depuis uint64
BigBinary bb_from_binstring(const char *s);      // créer depuis chaîne de bits "10101"
void bb_free(BigBinary *a);
BigBinary bb_copy(const BigBinary *a);

// Affichage
void bb_print_binary(const BigBinary *a);
void bb_print_hex(const BigBinary *a);

// Opérations basiques
bool bb_is_zero(const BigBinary *a);
bool bb_is_even(const BigBinary *a);
int bb_cmp(const BigBinary *a, const BigBinary *b); // -1,0,1
void bb_trim(BigBinary *a);

// Addition / soustraction (positifs)
BigBinary bb_add(const BigBinary *a, const BigBinary *b); // a + b
// suppose a >= b
BigBinary bb_sub(const BigBinary *a, const BigBinary *b); // a - b

// Shift / division by 2
BigBinary bb_shiftr1(const BigBinary *a); // floor(a/2)
BigBinary bb_shiftl1(const BigBinary *a); // a*2

// Multiplication (naïf)
BigBinary bb_mul(const BigBinary *a, const BigBinary *b);

// Division with remainder: computes q = a / b and r = a % b
// returns quotient; remainder output param
BigBinary bb_divmod(const BigBinary *a, const BigBinary *b, BigBinary *remainder);

// Helpers
uint64_t bb_to_u64(const BigBinary *a, int *ok); // try to convert to uint64, ok=1 if success

#endif // BIGBINARY_H
