// Copyright (c) 2024-present The Bitquantum Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "randomq_hash.h"
#include <cstring>
#include <algorithm>

CRandomQHash::CRandomQHash()
    : m_initialized(false)
    , m_rounds(8192)
    , m_nonce(0)
{
}

CRandomQHash::~CRandomQHash() {
}

void CRandomQHash::Initialize() {
    m_randomq.Initialize(nullptr, 0);
    m_randomq.SetRounds(m_rounds);
    m_randomq.SetNonce(m_nonce);
    m_initialized = true;
}

void CRandomQHash::Write(const unsigned char* data, size_t len) {
    if (!m_initialized) {
        Initialize();
    }
    
    std::span<const unsigned char> span(data, len);
    m_randomq.Write(span);
}

void CRandomQHash::Write(const std::vector<unsigned char>& data) {
    if (!data.empty()) {
        Write(data.data(), data.size());
    }
}

void CRandomQHash::Write(const std::string& data) {
    Write(reinterpret_cast<const unsigned char*>(data.c_str()), data.length());
}

void CRandomQHash::Finalize(unsigned char hash[OUTPUT_SIZE]) {
    if (!m_initialized) {
        Initialize();
    }
    
    m_randomq.Finalize(hash);
}

uint256 CRandomQHash::GetHash() {
    unsigned char hash[OUTPUT_SIZE];
    Finalize(hash);
    
    uint256 result;
    std::memcpy(result.begin(), hash, OUTPUT_SIZE);
    return result;
}

void CRandomQHash::Reset() {
    m_randomq.Reset();
    m_initialized = false;
}

void CRandomQHash::SetRandomQRounds(uint64_t rounds) {
    m_rounds = rounds;
    if (m_initialized) {
        m_randomq.SetRounds(rounds);
    }
}

void CRandomQHash::SetRandomQNonce(uint64_t nonce) {
    m_nonce = nonce;
    if (m_initialized) {
        m_randomq.SetNonce(nonce);
    }
}

// RandomQMining namespace implementation
namespace RandomQMining {

uint256 CalculateRandomQHash(const std::vector<unsigned char>& header) {
    CRandomQHash hasher;
    hasher.Write(header);
    return hasher.GetHash();
}

uint256 CalculateRandomQHashOptimized(const std::vector<unsigned char>& header, uint32_t nonce) {
    CRandomQHash hasher;
    hasher.SetRandomQNonce(nonce);
    hasher.Write(header);
    return hasher.GetHash();
}

bool CheckTarget(const uint256& hash, const uint256& target) {
    return hash < target;
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

} // namespace RandomQMining
