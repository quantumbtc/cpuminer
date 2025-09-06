/*
 * Copyright (c) 2024-present The Bitquantum Core developers
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 */

#ifndef RANDOMQ_H
#define RANDOMQ_H

#include <stdint.h>
#include <stddef.h>

/** A hasher class for RandomQ (Monero's anti-quantum algorithm). */
typedef struct {
    // RandomQ internal state
    uint64_t state[25]; // 200-byte internal state
    uint64_t nonce;
    uint64_t rounds;
} CRandomQ;

#define RANDOMQ_OUTPUT_SIZE 32 // 256-bit output

/** Initialize RandomQ */
void randomq_init(CRandomQ* ctx);

/** Cleanup RandomQ */
void randomq_cleanup(CRandomQ* ctx);

/** Initialize RandomQ with a seed */
void randomq_initialize(CRandomQ* ctx, const uint8_t* seed, size_t seed_len);

/** Process input data */
void randomq_write(CRandomQ* ctx, const unsigned char* input, size_t len);

/** Finalize and get the hash result */
void randomq_finalize(CRandomQ* ctx, unsigned char hash[RANDOMQ_OUTPUT_SIZE]);

/** Reset the hasher state */
void randomq_reset(CRandomQ* ctx);

/** Set the number of rounds for RandomQ */
void randomq_set_rounds(CRandomQ* ctx, uint64_t rounds);

/** Set the nonce value */
void randomq_set_nonce(CRandomQ* ctx, uint64_t nonce);

#endif // RANDOMQ_H
