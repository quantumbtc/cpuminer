/*
 * Copyright (c) 2024-present The Bitquantum Core developers
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 */

#include "randomq_hash.h"
#include "miner.h"
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

void randomq_hash_init(CRandomQHash* ctx)
{
    if (!ctx) return;
    
    sha256_init(ctx->sha256_first);
    randomq_init(&ctx->randomq);
    sha256_init(ctx->sha256_second);
}

void randomq_hash_cleanup(CRandomQHash* ctx)
{
    if (!ctx) return;
    
    randomq_cleanup(&ctx->randomq);
}

void randomq_hash_write(CRandomQHash* ctx, const unsigned char* input, size_t len)
{
    if (!ctx || !input || len == 0) return;
    
    // For simplicity, we'll use the existing sha256d function
    // This is a simplified implementation for testing
    uint32_t data[20];
    uint32_t hash[8];
    
    // Copy input to data array (assuming 80 bytes max)
    memset(data, 0, sizeof(data));
    size_t copy_len = (len > 80) ? 80 : len;
    memcpy(data, input, copy_len);
    
    // Use sha256d to process the data
    sha256d((unsigned char*)ctx->sha256_first, (unsigned char*)data, 80);
}

void randomq_hash_finalize(CRandomQHash* ctx, unsigned char hash[RANDOMQ_HASH_OUTPUT_SIZE])
{
    if (!ctx || !hash) return;
    
    // First SHA256 - get the current state
    unsigned char* first_hash = (unsigned char*)ctx->sha256_first;
    
    // RandomQ
    unsigned char randomq_hash[RANDOMQ_OUTPUT_SIZE];
    randomq_reset(&ctx->randomq);
    randomq_write(&ctx->randomq, (unsigned char*)first_hash, 32);
    randomq_finalize(&ctx->randomq, randomq_hash);
    
    // Second SHA256 - use sha256d for simplicity
    sha256d(hash, randomq_hash, RANDOMQ_OUTPUT_SIZE);
}

void randomq_hash_reset(CRandomQHash* ctx)
{
    if (!ctx) return;
    
    sha256_init(ctx->sha256_first);
    randomq_reset(&ctx->randomq);
    sha256_init(ctx->sha256_second);
}

void randomq_hash_set_rounds(CRandomQHash* ctx, uint64_t rounds)
{
    if (!ctx) return;
    randomq_set_rounds(&ctx->randomq, rounds);
}

void randomq_hash_set_nonce(CRandomQHash* ctx, uint64_t nonce)
{
    if (!ctx) return;
    randomq_set_nonce(&ctx->randomq, nonce);
}
