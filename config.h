// Copyright (c) 2024-present The Bitquantum Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CPUMINER_CONFIG_H
#define CPUMINER_CONFIG_H

#include <string>
#include <vector>
#include <cstdint>

// Configuration structure for the miner
struct MinerConfig {
    // RPC connection settings
    std::string rpc_host;
    int rpc_port;
    std::string rpc_user;
    std::string rpc_password;
    
    // Mining settings
    int num_threads;
    uint64_t randomq_rounds;
    bool enable_avx2;
    bool enable_sse4;
    bool enable_optimized;
    bool submit_work;
    
    // Logging settings
    int log_level;
    bool show_stats;
    uint32_t stats_interval;
    
    // Default constructor
    MinerConfig();
    
    // Validation
    bool validate() const;
    
    // Print configuration
    void print() const;
};

// Configuration manager
class ConfigManager {
public:
    // Load configuration from command line arguments
    static bool loadFromArgs(int argc, char* argv[], MinerConfig& config);
    
    // Load configuration from file
    static bool loadFromFile(const std::string& filename, MinerConfig& config);
    
    // Save configuration to file
    static bool saveToFile(const std::string& filename, const MinerConfig& config);
    
    // Print help
    static void printHelp();
    
private:
    // Parse command line argument
    static bool parseArg(const std::string& arg, MinerConfig& config);
    
    // Set default values
    static void setDefaults(MinerConfig& config);
};

#endif // CPUMINER_CONFIG_H