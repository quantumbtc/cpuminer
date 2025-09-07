// Copyright (c) 2024-present The Bitquantum Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CPUMINER_RPC_CLIENT_H
#define CPUMINER_RPC_CLIENT_H

#include <string>
#include <vector>
#include <cstdint>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

// Forward declarations
class RandomQMiner;

// Work data structure
struct WorkData {
    std::string block_template;
    std::string previous_block_hash;
    std::string merkle_root;
    std::string target;
    uint32_t version;
    uint32_t timestamp;
    uint32_t bits;
    uint32_t height;
    uint32_t nonce_start;
    uint32_t nonce_end;
    std::vector<std::string> transactions;
    
    // Validation
    bool isValid() const;
    
    // Print work data
    void print() const;
};

// Mining statistics
struct MiningStats {
    uint64_t total_hashes;
    uint64_t valid_blocks;
    uint64_t invalid_blocks;
    double hash_rate;
    double elapsed_time;
    uint32_t current_nonce;
    std::string current_target;
    std::string best_hash;
    uint32_t best_nonce;
    
    // Reset statistics
    void reset();
    
    // Print statistics
    void print() const;
};

// RPC client for communicating with Bitquantum Core
class RPCClient {
public:
    RPCClient();
    ~RPCClient();
    
    // Initialize RPC client
    bool initialize(const std::string& host, int port, 
                   const std::string& user, const std::string& password);
    
    // Start RPC client
    void start();
    
    // Stop RPC client
    void stop();
    
    // Check if connected
    bool isConnected() const { return m_connected; }
    
    // Get current work
    WorkData getCurrentWork() const;
    
    // Submit solution
    bool submitSolution(const WorkData& work, uint32_t nonce, const std::string& hash);
    
    // Set miner reference
    void setMiner(RandomQMiner* miner);
    
    // Get statistics
    MiningStats getStats() const;
    
private:
    // RPC connection
    void connect();
    void disconnect();
    
    // RPC calls
    std::string rpcCall(const std::string& method, const std::string& params = "[]");
    std::string rpcCall(const std::string& method, const std::vector<std::string>& params);
    
    // Work management
    void updateWork();
    void workThread();
    
    // HTTP client
    std::string httpPost(const std::string& url, const std::string& data);
    std::string httpGet(const std::string& url);
    
    // JSON parsing
    WorkData parseWorkTemplate(const std::string& json);
    bool parseSubmitResponse(const std::string& json);
    
    // Logging
    void log(int level, const std::string& message) const;
    
private:
    // Connection settings
    std::string m_host;
    int m_port;
    std::string m_user;
    std::string m_password;
    std::string m_url;
    
    // Connection state
    std::atomic<bool> m_connected;
    std::atomic<bool> m_running;
    std::atomic<bool> m_should_stop;
    
    // Work data
    mutable std::mutex m_work_mutex;
    WorkData m_current_work;
    bool m_has_work;
    
    // Statistics
    mutable std::mutex m_stats_mutex;
    MiningStats m_stats;
    
    // Threading
    std::thread m_work_thread;
    
    // Miner reference
    RandomQMiner* m_miner;
    
    // Logging
    int m_log_level;
};

#endif // CPUMINER_RPC_CLIENT_H