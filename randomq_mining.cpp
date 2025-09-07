// Copyright (c) 2024-present The Bitquantum Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "randomq_mining.h"
#include "randomq_hash.h"
#include <cstring>
#include <algorithm>
#include <iomanip>
#include <sstream>

namespace RandomQMining {

uint256 CalculateRandomQHash(const CBlockHeader& header) {
    // Serialize block header
    std::vector<unsigned char> header_data;
    header_data.reserve(80); // Standard block header size
    
    // Serialize header fields
    uint32_t version = header.nVersion;
    uint256 prevBlock = header.hashPrevBlock;
    uint256 merkleRoot = header.hashMerkleRoot;
    uint32_t timestamp = header.nTime;
    uint32_t bits = header.nBits;
    uint32_t nonce = header.nNonce;
    
    // Convert to little-endian byte order
    for (int i = 0; i < 4; i++) {
        header_data.push_back(version & 0xFF);
        version >>= 8;
    }
    
    // Add previous block hash
    for (int i = 0; i < 32; i++) {
        header_data.push_back(prevBlock.begin()[i]);
    }
    
    // Add merkle root
    for (int i = 0; i < 32; i++) {
        header_data.push_back(merkleRoot.begin()[i]);
    }
    
    // Add timestamp
    for (int i = 0; i < 4; i++) {
        header_data.push_back(timestamp & 0xFF);
        timestamp >>= 8;
    }
    
    // Add bits
    for (int i = 0; i < 4; i++) {
        header_data.push_back(bits & 0xFF);
        bits >>= 8;
    }
    
    // Add nonce
    for (int i = 0; i < 4; i++) {
        header_data.push_back(nonce & 0xFF);
        nonce >>= 8;
    }
    
    // Calculate RandomQ hash
    return CalculateRandomQHash(header_data);
}

uint256 CalculateRandomQHashOptimized(const CBlockHeader& header, uint32_t nonce) {
    // Create a copy of the header with the new nonce
    CBlockHeader header_copy = header;
    header_copy.nNonce = nonce;
    
    return CalculateRandomQHash(header_copy);
}

bool CheckTarget(const uint256& hash, const uint256& target) {
    return hash < target;
}

bool CheckRandomQProofOfWork(const CBlockHeader& header, uint32_t nBits, const uint256& powLimit) {
    // Calculate target from nBits
    uint256 target = uint256().SetCompact(nBits);
    
    // Check if target is valid
    if (target == uint256() || target > powLimit) {
        return false;
    }
    
    // Calculate hash
    uint256 hash = CalculateRandomQHash(header);
    
    // Check if hash meets target
    return CheckTarget(hash, target);
}

uint256 DifficultyToTarget(uint32_t difficulty) {
    if (difficulty == 0) {
        return uint256();
    }
    
    // Convert difficulty to target
    // Target = 2^256 / difficulty
    uint256 max_target = uint256("0x00000000ffff0000000000000000000000000000000000000000000000000000");
    return max_target / difficulty;
}

uint32_t TargetToDifficulty(const uint256& target) {
    if (target == uint256()) {
        return 0;
    }
    
    uint256 max_target = uint256("0x00000000ffff0000000000000000000000000000000000000000000000000000");
    return static_cast<uint32_t>((max_target / target).GetLow64());
}

// MiningUtils implementation
double MiningUtils::CalculateHashRate(uint64_t hashes, double elapsed_time) {
    if (elapsed_time <= 0) {
        return 0.0;
    }
    return static_cast<double>(hashes) / elapsed_time;
}

std::string MiningUtils::FormatHashRate(double hash_rate) {
    std::ostringstream oss;
    
    if (hash_rate >= 1e9) {
        oss << std::fixed << std::setprecision(2) << (hash_rate / 1e9) << " GH/s";
    } else if (hash_rate >= 1e6) {
        oss << std::fixed << std::setprecision(2) << (hash_rate / 1e6) << " MH/s";
    } else if (hash_rate >= 1e3) {
        oss << std::fixed << std::setprecision(2) << (hash_rate / 1e3) << " kH/s";
    } else {
        oss << std::fixed << std::setprecision(2) << hash_rate << " H/s";
    }
    
    return oss.str();
}

double MiningUtils::EstimateMiningTime(double hash_rate, const uint256& target) {
    if (hash_rate <= 0 || target == uint256()) {
        return -1.0;
    }
    
    // Calculate expected number of hashes needed
    uint256 max_target = uint256("0x00000000ffff0000000000000000000000000000000000000000000000000000");
    double expected_hashes = static_cast<double>(max_target.GetLow64()) / static_cast<double>(target.GetLow64());
    
    return expected_hashes / hash_rate;
}

bool MiningUtils::ValidateBlockHeader(const CBlockHeader& header) {
    // Basic validation
    if (header.nVersion == 0) {
        return false;
    }
    
    if (header.hashPrevBlock == uint256()) {
        return false;
    }
    
    if (header.hashMerkleRoot == uint256()) {
        return false;
    }
    
    if (header.nTime == 0) {
        return false;
    }
    
    if (header.nBits == 0) {
        return false;
    }
    
    return true;
}

} // namespace RandomQMining
