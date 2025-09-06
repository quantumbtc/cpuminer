/*
 * Copyright (c) 2024-present The Bitquantum Core developers
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 */

#include "randomq.h"
#include "miner.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

// RandomQ constants
static const uint64_t RANDOMQ_CONSTANTS[25] = {
    0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b,
    0xa54ff53a5f1d36f1, 0x510e527fade682d1, 0x9b05688c2b3e6c1f,
    0x1f83d9abfb41bd6b, 0x5be0cd19137e2179, 0x428a2f98d728ae22,
    0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
    0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b,
    0xab1c5ed5da6d8118, 0xd807aa98a3030242, 0x12835b0145706fbe,
    0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 0x72be5d74f27b896f,
    0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
    0xe49b69c19ef14ad2
};

void randomq_init(CRandomQ* ctx)
{
    if (!ctx) return;
    
    memset(ctx, 0, sizeof(CRandomQ));
    ctx->rounds = 8192;
}

void randomq_cleanup(CRandomQ* ctx)
{
    if (!ctx) return;
    
    // Clear sensitive data
    memset(ctx->state, 0, sizeof(ctx->state));
    ctx->nonce = 0;
    ctx->rounds = 0;
}

void randomq_initialize(CRandomQ* ctx, const uint8_t* seed, size_t seed_len)
{
    if (!ctx) return;
    
    randomq_reset(ctx);
    
    // Initialize state with constants
    memcpy(ctx->state, RANDOMQ_CONSTANTS, sizeof(RANDOMQ_CONSTANTS));
    
    // Mix in the seed
    if (seed && seed_len > 0) {
        size_t offset = 0;
        for (size_t i = 0; i < 25 && offset < seed_len; i++) {
            uint64_t seed_chunk = 0;
            for (size_t j = 0; j < 8 && offset + j < seed_len; j++) {
                seed_chunk |= (uint64_t)seed[offset + j] << (j * 8);
            }
            ctx->state[i] ^= seed_chunk;
            offset += 8;
        }
    }
}

static void randomq_round(CRandomQ* ctx)
{
    // RandomQ round function - simplified version based on Monero's RandomX
    // This is a simplified implementation for demonstration purposes
    
    // Rotate and mix state elements
    for (int i = 0; i < 25; i++) {
        // Rotate left by 13 bits
        uint64_t rotated = (ctx->state[i] << 13) | (ctx->state[i] >> 51);
        
        // Mix with next element (wrapping around)
        uint64_t next = ctx->state[(i + 1) % 25];
        ctx->state[i] = rotated ^ next ^ (ctx->state[i] + next);
        
        // Add constant
        ctx->state[i] += RANDOMQ_CONSTANTS[i];
    }
    
    // Additional mixing step
    for (int i = 0; i < 25; i += 2) {
        uint64_t temp = ctx->state[i];
        ctx->state[i] = ctx->state[i] ^ ctx->state[(i + 1) % 25];
        ctx->state[(i + 1) % 25] = ctx->state[(i + 1) % 25] ^ temp;
    }
}

void randomq_write(CRandomQ* ctx, const unsigned char* input, size_t len)
{
    if (!ctx || !input || len == 0) return;
    
    // Process input in 64-byte chunks
    size_t offset = 0;
    while (offset < len) {
        size_t chunk_size = (len - offset > 64) ? 64 : (len - offset);
        
        // Mix input chunk into state
        for (size_t i = 0; i < chunk_size / 8 && i < 8; i++) {
            uint64_t chunk = 0;
            for (size_t j = 0; j < 8 && offset + i * 8 + j < len; j++) {
                chunk |= (uint64_t)input[offset + i * 8 + j] << (j * 8);
            }
            ctx->state[i] ^= chunk;
        }
        
        // Run one round of RandomQ
        randomq_round(ctx);
        
        offset += chunk_size;
    }
}

void randomq_finalize(CRandomQ* ctx, unsigned char hash[RANDOMQ_OUTPUT_SIZE])
{
    if (!ctx || !hash) return;
    
    // Mix in the nonce
    ctx->state[0] ^= ctx->nonce;
    
    // Run final rounds
    for (uint64_t i = 0; i < ctx->rounds; i++) {
        randomq_round(ctx);
    }
    
    // Use SHA256 to finalize the RandomQ state
    // For simplicity, we'll use sha256d
    uint32_t state_data[50]; // 25 * 2 = 50 words
    
    // Convert 64-bit state to 32-bit words
    for (int i = 0; i < 25; i++) {
        state_data[i * 2] = (uint32_t)(ctx->state[i] & 0xFFFFFFFF);
        state_data[i * 2 + 1] = (uint32_t)(ctx->state[i] >> 32);
    }
    
    // Use sha256d to hash the state
    sha256d(hash, (unsigned char*)state_data, 200);
}

void randomq_reset(CRandomQ* ctx)
{
    if (!ctx) return;
    
    memset(ctx->state, 0, sizeof(ctx->state));
    ctx->nonce = 0;
}

void randomq_set_rounds(CRandomQ* ctx, uint64_t rounds)
{
    if (!ctx) return;
    ctx->rounds = rounds;
}

void randomq_set_nonce(CRandomQ* ctx, uint64_t nonce)
{
    if (!ctx) return;
    ctx->nonce = nonce;
}
