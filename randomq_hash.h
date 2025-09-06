/*
 * Copyright (c) 2024-present The Bitquantum Core developers
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 */

#ifndef RANDOMQ_HASH_H
#define RANDOMQ_HASH_H

#include "randomq.h"
#include "miner.h"
#include <stdint.h>

/** A hasher class for RandomQ hash (SHA256->RandomQ->SHA256). */
typedef struct {
    uint32_t sha256_first[8];
    CRandomQ randomq;
    uint32_t sha256_second[8];
} CRandomQHash;

#define RANDOMQ_HASH_OUTPUT_SIZE 32

/** Initialize RandomQ hash */
void randomq_hash_init(CRandomQHash* ctx);

/** Cleanup RandomQ hash */
void randomq_hash_cleanup(CRandomQHash* ctx);

/** Process input data */
void randomq_hash_write(CRandomQHash* ctx, const unsigned char* input, size_t len);

/** Finalize and get the hash result */
void randomq_hash_finalize(CRandomQHash* ctx, unsigned char hash[RANDOMQ_HASH_OUTPUT_SIZE]);

/** Reset the hasher state */
void randomq_hash_reset(CRandomQHash* ctx);

/** Set RandomQ parameters */
void randomq_hash_set_rounds(CRandomQHash* ctx, uint64_t rounds);
void randomq_hash_set_nonce(CRandomQHash* ctx, uint64_t nonce);

#endif // RANDOMQ_HASH_H
