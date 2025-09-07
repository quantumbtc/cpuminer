// Copyright (c) 2024-present The Bitquantum Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CPUMINER_RANDOMQ_HASH_H
#define CPUMINER_RANDOMQ_HASH_H

#include <cstdint>
#include <vector>
#include <string>
#include "../src/uint256.h"
#include "../src/crypto/randomq.h"

/**
 * RandomQ hash implementation for mining
 */
class CRandomQHash {
public:
    static const size_t OUTPUT_SIZE = 32; // 256-bit output
    
    CRandomQHash();
    ~CRandomQHash();
    
    // Initialize the hasher
    void Initialize();
    
    // Write data to the hasher
    void Write(const unsigned char* data, size_t len);
    
    // Write data from a vector
    void Write(const std::vector<unsigned char>& data);
    
    // Write data from a string
    void Write(const std::string& data);
    
    // Finalize and get the hash
    void Finalize(unsigned char hash[OUTPUT_SIZE]);
    
    // Get hash as uint256
    uint256 GetHash();
    
    // Reset the hasher
    void Reset();
    
    // Set RandomQ parameters
    void SetRandomQRounds(uint64_t rounds);
    void SetRandomQNonce(uint64_t nonce);
    
    // Get current state
    const uint64_t* GetState() const { return m_randomq.GetState(); }
    size_t GetStateSize() const { return 25; } // 25 x 64-bit words
    
private:
    CRandomQ m_randomq;
    bool m_initialized;
    uint64_t m_rounds;
    uint64_t m_nonce;
};

// Utility functions for RandomQ mining
namespace RandomQMining {
    // Calculate RandomQ hash for a block header
    uint256 CalculateRandomQHash(const std::vector<unsigned char>& header);
    
    // Calculate RandomQ hash with optimizations
    uint256 CalculateRandomQHashOptimized(const std::vector<unsigned char>& header, uint32_t nonce);
    
    // Check if a hash meets the target
    bool CheckTarget(const uint256& hash, const uint256& target);
    
    // Convert difficulty to target
    uint256 DifficultyToTarget(uint32_t difficulty);
    
    // Convert target to difficulty
    uint32_t TargetToDifficulty(const uint256& target);
}

#endif // CPUMINER_RANDOMQ_HASH_H
