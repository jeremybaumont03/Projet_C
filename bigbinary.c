#include "bigbinary.h"
#include <stdio.h>
#include <string.h>

#define WORD_BASE ((uint64_t)0x100000000ULL) // 2^32

// Internal: allocate words and set to zero
static uint32_t *alloc_words(size_t n) {
    uint32_t *w = (uint32_t*)calloc(n, sizeof(uint32_t));
    return w;
}

BigBinary bb_zero() {
    BigBinary z;
    z.words = alloc_words(1);
    z.nwords = 1;
    z.sign = 0;
    z.words[0] = 0;
    return z;
}

BigBinary bb_from_uint64(uint64_t v) {
    BigBinary r;
    if (v == 0) return bb_zero();
    r.words = alloc_words(2);
    r.words[0] = (uint32_t)(v & 0xFFFFFFFFULL);
    r.words[1] = (uint32_t)((v >> 32) & 0xFFFFFFFFULL);
    r.nwords = (r.words[1] ? 2 : 1);
    r.sign = 1;
    return r;
}

BigBinary bb_from_binstring(const char *s) {
    // s is sequence of '0'/'1', MSB first e.g. "1011"
    BigBinary r = bb_zero();
    size_t len = strlen(s);
    for (size_t i = 0; i < len; ++i) {
        char c = s[i];
        if (c != '0' && c != '1') continue;
        // left shift r by 1 and add bit
        BigBinary tmp = bb_shiftl1(&r);
        if (!bb_is_zero(&r)) bb_free(&r);
        r = tmp;
        if (c == '1') {
            BigBinary one = bb_from_uint64(1);
            BigBinary sum = bb_add(&r, &one);
            bb_free(&r);
            bb_free(&one);
            r = sum;
        }
    }
    if (bb_is_zero(&r)) r.sign = 0;
    else r.sign = 1;
    return r;
}

void bb_free(BigBinary *a) {
    if (!a) return;
    if (a->words) free(a->words);
    a->words = NULL;
    a->nwords = 0;
    a->sign = 0;
}

BigBinary bb_copy(const BigBinary *a) {
    BigBinary r;
    r.nwords = a->nwords;
    r.words = alloc_words(r.nwords);
    memcpy(r.words, a->words, r.nwords * sizeof(uint32_t));
    r.sign = a->sign;
    return r;
}

void bb_trim(BigBinary *a) {
    if (!a) return;
    size_t n = a->nwords;
    while (n > 1 && a->words[n-1] == 0) n--;
    if (n == 0) n = 1;
    if (n != a->nwords) {
        a->words = (uint32_t*)realloc(a->words, n * sizeof(uint32_t));
        a->nwords = n;
    }
    if (n == 1 && a->words[0] == 0) a->sign = 0;
}

bool bb_is_zero(const BigBinary *a) {
    if (!a) return true;
    for (size_t i = 0; i < a->nwords; ++i) if (a->words[i] != 0) return false;
    return true;
}

bool bb_is_even(const BigBinary *a) {
    if (!a) return true;
    return (a->words[0] & 1) == 0;
}

int bb_cmp(const BigBinary *a, const BigBinary *b) {
    // compare absolute values (we only use non-negative numbers here)
    if (a->nwords > b->nwords) return 1;
    if (a->nwords < b->nwords) return -1;
    for (size_t i = a->nwords; i-- > 0;) {
        if (a->words[i] > b->words[i]) return 1;
        if (a->words[i] < b->words[i]) return -1;
    }
    return 0;
}

void bb_print_binary(const BigBinary *a) {
    if (bb_is_zero(a)) { printf("0"); return; }
    // convert to binary string by repeated shifting (non-destructive copy)
    BigBinary tmp = bb_copy(a);
    bool first = true;
    // collect bits in dynamic buffer (since size unknown)
    char *buf = NULL;
    size_t cap = 0, len = 0;
    while (!bb_is_zero(&tmp)) {
        int bit = tmp.words[0] & 1;
        // append char
        if (len + 1 >= cap) {
            cap = cap ? cap*2 : 64;
            buf = (char*)realloc(buf, cap);
        }
        buf[len++] = (bit ? '1' : '0');
        // shift right by 1
        BigBinary shr = bb_shiftr1(&tmp);
        bb_free(&tmp);
        tmp = shr;
    }
    // print reversed
    for (size_t i = 0; i < len; ++i) putchar(buf[len-1-i]);
    free(buf);
    bb_free(&tmp);
}

void bb_print_hex(const BigBinary *a) {
    if (bb_is_zero(a)) { printf("0x0"); return; }
    for (size_t i = a->nwords; i-- > 0;) {
        if (i == a->nwords - 1) printf("%X", a->words[i]);
        else printf("%08X", a->words[i]);
    }
}

// Addition
BigBinary bb_add(const BigBinary *a, const BigBinary *b) {
    size_t n = (a->nwords > b->nwords) ? a->nwords : b->nwords;
    BigBinary r;
    r.words = alloc_words(n + 1);
    r.nwords = n + 1;
    uint64_t carry = 0;
    for (size_t i = 0; i < r.nwords; ++i) {
        uint64_t aw = (i < a->nwords) ? a->words[i] : 0;
        uint64_t bw = (i < b->nwords) ? b->words[i] : 0;
        uint64_t sum = aw + bw + carry;
        r.words[i] = (uint32_t)(sum & 0xFFFFFFFFULL);
        carry = sum >> 32;
    }
    r.sign = 1;
    bb_trim(&r);
    return r;
}

// Subtraction a - b, assume a >= b
BigBinary bb_sub(const BigBinary *a, const BigBinary *b) {
    BigBinary r;
    r.words = alloc_words(a->nwords);
    r.nwords = a->nwords;
    int64_t borrow = 0;
    for (size_t i = 0; i < r.nwords; ++i) {
        int64_t aw = (i < a->nwords) ? a->words[i] : 0;
        int64_t bw = (i < b->nwords) ? b->words[i] : 0;
        int64_t diff = aw - bw - borrow;
        if (diff < 0) {
            diff += (int64_t)WORD_BASE;
            borrow = 1;
        } else borrow = 0;
        r.words[i] = (uint32_t)(diff & 0xFFFFFFFFLL);
    }
    r.sign = 1;
    bb_trim(&r);
    return r;
}

// shift right by 1 (floor division by 2)
BigBinary bb_shiftr1(const BigBinary *a) {
    BigBinary r;
    r.words = alloc_words(a->nwords);
    r.nwords = a->nwords;
    uint32_t carry = 0;
    for (size_t i = a->nwords; i-- > 0;) {
        uint64_t cur = ((uint64_t)carry << 32) | a->words[i];
        r.words[i] = (uint32_t)(cur >> 1);
        carry = (uint32_t)(cur & 1);
    }
    r.sign = (bb_is_zero(&r) ? 0 : 1);
    bb_trim(&r);
    return r;
}

// shift left by 1 (multiply by 2)
BigBinary bb_shiftl1(const BigBinary *a) {
    BigBinary r;
    r.words = alloc_words(a->nwords + 1);
    r.nwords = a->nwords + 1;
    uint64_t carry = 0;
    for (size_t i = 0; i < a->nwords; ++i) {
        uint64_t cur = ((uint64_t)a->words[i] << 1) + carry;
        r.words[i] = (uint32_t)(cur & 0xFFFFFFFFULL);
        carry = cur >> 32;
    }
    r.words[a->nwords] = (uint32_t)carry;
    r.sign = 1;
    bb_trim(&r);
    return r;
}

// multiplication naive O(n*m)
BigBinary bb_mul(const BigBinary *a, const BigBinary *b) {
    BigBinary r;
    size_t n = a->nwords, m = b->nwords;
    r.words = alloc_words(n + m);
    r.nwords = n + m;
    for (size_t i = 0; i < n; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < m; ++j) {
            uint64_t cur = (uint64_t)a->words[i] * (uint64_t)b->words[j] + r.words[i+j] + carry;
            r.words[i+j] = (uint32_t)(cur & 0xFFFFFFFFULL);
            carry = cur >> 32;
        }
        r.words[i + m] = (uint32_t)carry;
    }
    r.sign = 1;
    bb_trim(&r);
    return r;
}

// Division algorithm (long division) returns quotient, remainder through pointer
// Works for positive values. If b==0 behavior undefined.
BigBinary bb_divmod(const BigBinary *a, const BigBinary *b, BigBinary *remainder) {
    // Simple cases
    BigBinary zero = bb_zero();
    if (bb_is_zero(b)) {
        // undefined: return zero quotient
        if (remainder) *remainder = zero;
        else bb_free(&zero);
        return zero;
    }
    if (bb_cmp(a, b) < 0) {
        // quotient = 0, remainder = a
        if (remainder) *remainder = bb_copy(a);
        return bb_zero();
    }
    // We'll implement binary long division: shift divisor left until just <= dividend
    BigBinary dividend = bb_copy(a);
    BigBinary divisor = bb_copy(b);
    BigBinary quotient = bb_zero();
    // determine shift
    size_t shift = 0;
    // while divisor << shift <= dividend, increase shift
    BigBinary cur = divisor;
    while (bb_cmp(&cur, &dividend) <= 0) {
        BigBinary next = bb_shiftl1(&cur);
        bb_free(&cur);
        cur = next;
        shift++;
    }
    // cur > dividend now, step back one
    if (shift > 0) {
        BigBinary prev = bb_shiftr1(&cur);
        bb_free(&cur);
        cur = prev;
        shift--;
    }
    // quotient will have enough bits to represent
    // initialize quotient to zero
    quotient = bb_zero();
    // perform subtractive algorithm
    while (true) {
        if (bb_cmp(&cur, &dividend) <= 0) {
            BigBinary newdiv = bb_sub(&dividend, &cur);
            bb_free(&dividend);
            dividend = newdiv;
            // set bit at position 'shift' in quotient: quotient += (1 << shift)
            // create temp = 1 << shift
            BigBinary one = bb_from_uint64(1);
            for (size_t i = 0; i < shift; ++i) {
                BigBinary tmp = bb_shiftl1(&one);
                bb_free(&one);
                one = tmp;
            }
            BigBinary newq = bb_add(&quotient, &one);
            bb_free(&quotient);
            bb_free(&one);
            quotient = newq;
        }
        if (shift == 0) break;
        // shift cur right by 1
        BigBinary prev = bb_shiftr1(&cur);
        bb_free(&cur);
        cur = prev;
        shift--;
    }
    if (remainder) *remainder = bb_copy(&dividend);
    bb_free(&dividend);
    bb_free(&divisor);
    bb_free(&cur);
    // trim quotient
    bb_trim(&quotient);
    return quotient;
}

uint64_t bb_to_u64(const BigBinary *a, int *ok) {
    uint64_t v = 0;
    if (a->nwords > 2) { if (ok) *ok = 0; return 0; }
    if (a->nwords >= 1) v |= a->words[0];
    if (a->nwords == 2) v |= ((uint64_t)a->words[1] << 32);
    if (ok) *ok = 1;
    return v;
}
    