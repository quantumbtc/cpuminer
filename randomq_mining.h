// Copyright (c) 2024-present The Bitquantum Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CPUMINER_RANDOMQ_MINING_H
#define CPUMINER_RANDOMQ_MINING_H

#include <cstdint>
#include <vector>
#include <string>
#include "uint256.h"
#include "block.h"

// Forward declarations
class CBlockHeader;

namespace RandomQMining {
    // Calculate RandomQ hash for a block header
    uint256 CalculateRandomQHash(const CBlockHeader& header);
    
    // Calculate RandomQ hash with optimizations
    uint256 CalculateRandomQHashOptimized(const CBlockHeader& header, uint32_t nonce);
    
    // Check if a hash meets the target
    bool CheckTarget(const uint256& hash, const uint256& target);
    
    // Check proof of work
    bool CheckRandomQProofOfWork(const CBlockHeader& header, uint32_t nBits, const uint256& powLimit);
    
    // Convert difficulty to target
    uint256 DifficultyToTarget(uint32_t difficulty);
    
    // Convert target to difficulty
    uint32_t TargetToDifficulty(const uint256& target);
    
    // Mining utilities
    class MiningUtils {
    public:
        // Calculate hash rate
        static double CalculateHashRate(uint64_t hashes, double elapsed_time);
        
        // Format hash rate
        static std::string FormatHashRate(double hash_rate);
        
        // Calculate mining time estimate
        static double EstimateMiningTime(double hash_rate, const uint256& target);
        
        // Validate block header
        static bool ValidateBlockHeader(const CBlockHeader& header);
    };
}

#endif // CPUMINER_RANDOMQ_MINING_H
