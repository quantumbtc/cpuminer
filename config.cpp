// Copyright (c) 2024-present The Bitquantum Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>

MinerConfig::MinerConfig() {
    // Set default values
    ConfigManager::setDefaults(*this);
}

bool MinerConfig::validate() const {
    // Validate RPC settings
    if (rpc_host.empty()) {
        std::cerr << "Error: RPC host not specified" << std::endl;
        return false;
    }
    
    if (rpc_port <= 0 || rpc_port > 65535) {
        std::cerr << "Error: Invalid RPC port: " << rpc_port << std::endl;
        return false;
    }
    
    // Validate mining settings
    if (num_threads <= 0) {
        std::cerr << "Error: Invalid number of threads: " << num_threads << std::endl;
        return false;
    }
    
    if (randomq_rounds == 0) {
        std::cerr << "Error: RandomQ rounds must be greater than 0" << std::endl;
        return false;
    }
    
    // Validate logging settings
    if (log_level < 0 || log_level > 3) {
        std::cerr << "Error: Invalid log level: " << log_level << std::endl;
        return false;
    }
    
    if (stats_interval == 0) {
        std::cerr << "Error: Stats interval must be greater than 0" << std::endl;
        return false;
    }
    
    return true;
}

void MinerConfig::print() const {
    std::cout << "\n=== Miner Configuration ===" << std::endl;
    std::cout << "RPC Host: " << rpc_host << std::endl;
    std::cout << "RPC Port: " << rpc_port << std::endl;
    std::cout << "RPC User: " << rpc_user << std::endl;
    std::cout << "RPC Password: " << (rpc_password.empty() ? "not set" : "***") << std::endl;
    std::cout << "Threads: " << num_threads << std::endl;
    std::cout << "RandomQ Rounds: " << randomq_rounds << std::endl;
    std::cout << "AVX2: " << (enable_avx2 ? "enabled" : "disabled") << std::endl;
    std::cout << "SSE4: " << (enable_sse4 ? "enabled" : "disabled") << std::endl;
    std::cout << "Optimized: " << (enable_optimized ? "enabled" : "disabled") << std::endl;
    std::cout << "Submit Work: " << (submit_work ? "enabled" : "disabled") << std::endl;
    std::cout << "Log Level: " << log_level << std::endl;
    std::cout << "Show Stats: " << (show_stats ? "enabled" : "disabled") << std::endl;
    std::cout << "Stats Interval: " << stats_interval << " seconds" << std::endl;
    std::cout << "=========================" << std::endl;
}

bool ConfigManager::loadFromArgs(int argc, char* argv[], MinerConfig& config) {
    // Set default values
    setDefaults(config);
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--help" || arg == "-h") {
            printHelp();
            return false;
        }
        
        if (!parseArg(arg, config)) {
            std::cerr << "Error: Invalid argument: " << arg << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ConfigManager::loadFromFile(const std::string& filename, MinerConfig& config) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open config file: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Parse key=value pairs
        size_t pos = line.find('=');
        if (pos == std::string::npos) {
            continue;
        }
        
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        
        // Trim whitespace
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        
        // Set configuration values
        if (key == "rpc_host") {
            config.rpc_host = value;
        } else if (key == "rpc_port") {
            config.rpc_port = std::stoi(value);
        } else if (key == "rpc_user") {
            config.rpc_user = value;
        } else if (key == "rpc_password") {
            config.rpc_password = value;
        } else if (key == "num_threads") {
            config.num_threads = std::stoi(value);
        } else if (key == "randomq_rounds") {
            config.randomq_rounds = std::stoull(value);
        } else if (key == "enable_avx2") {
            config.enable_avx2 = (value == "true" || value == "1");
        } else if (key == "enable_sse4") {
            config.enable_sse4 = (value == "true" || value == "1");
        } else if (key == "enable_optimized") {
            config.enable_optimized = (value == "true" || value == "1");
        } else if (key == "submit_work") {
            config.submit_work = (value == "true" || value == "1");
        } else if (key == "log_level") {
            config.log_level = std::stoi(value);
        } else if (key == "show_stats") {
            config.show_stats = (value == "true" || value == "1");
        } else if (key == "stats_interval") {
            config.stats_interval = std::stoi(value);
        }
    }
    
    return true;
}

bool ConfigManager::saveToFile(const std::string& filename, const MinerConfig& config) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot create config file: " << filename << std::endl;
        return false;
    }
    
    file << "# Bitquantum RandomQ CPU Miner Configuration" << std::endl;
    file << "# Generated on " << __DATE__ << " " << __TIME__ << std::endl;
    file << std::endl;
    
    file << "# RPC connection settings" << std::endl;
    file << "rpc_host=" << config.rpc_host << std::endl;
    file << "rpc_port=" << config.rpc_port << std::endl;
    file << "rpc_user=" << config.rpc_user << std::endl;
    file << "rpc_password=" << config.rpc_password << std::endl;
    file << std::endl;
    
    file << "# Mining settings" << std::endl;
    file << "num_threads=" << config.num_threads << std::endl;
    file << "randomq_rounds=" << config.randomq_rounds << std::endl;
    file << "enable_avx2=" << (config.enable_avx2 ? "true" : "false") << std::endl;
    file << "enable_sse4=" << (config.enable_sse4 ? "true" : "false") << std::endl;
    file << "enable_optimized=" << (config.enable_optimized ? "true" : "false") << std::endl;
    file << "submit_work=" << (config.submit_work ? "true" : "false") << std::endl;
    file << std::endl;
    
    file << "# Logging settings" << std::endl;
    file << "log_level=" << config.log_level << std::endl;
    file << "show_stats=" << (config.show_stats ? "true" : "false") << std::endl;
    file << "stats_interval=" << config.stats_interval << std::endl;
    
    return true;
}

void ConfigManager::printHelp() {
    std::cout << "Bitquantum RandomQ CPU Miner v1.0.0" << std::endl;
    std::cout << "Usage: cpuminer [options]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --rpc-host <host>        RPC server host (default: localhost)" << std::endl;
    std::cout << "  --rpc-port <port>        RPC server port (default: 8332)" << std::endl;
    std::cout << "  --rpc-user <user>        RPC username" << std::endl;
    std::cout << "  --rpc-password <pass>    RPC password" << std::endl;
    std::cout << "  --threads <count>        Number of mining threads (default: auto)" << std::endl;
    std::cout << "  --randomq-rounds <num>   RandomQ rounds (default: 8192)" << std::endl;
    std::cout << "  --enable-avx2            Enable AVX2 optimizations" << std::endl;
    std::cout << "  --enable-sse4            Enable SSE4 optimizations" << std::endl;
    std::cout << "  --enable-optimized       Enable optimized algorithms" << std::endl;
    std::cout << "  --no-submit              Don't submit work to pool" << std::endl;
    std::cout << "  --log-level <level>      Log level 0-3 (default: 2)" << std::endl;
    std::cout << "  --no-stats               Don't show statistics" << std::endl;
    std::cout << "  --stats-interval <sec>   Statistics update interval (default: 10)" << std::endl;
    std::cout << "  --config <file>          Load configuration from file" << std::endl;
    std::cout << "  --help, -h               Show this help message" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  cpuminer --rpc-host localhost --rpc-port 8332 --threads 4" << std::endl;
    std::cout << "  cpuminer --config miner.conf" << std::endl;
}

bool ConfigManager::parseArg(const std::string& arg, MinerConfig& config) {
    if (arg.substr(0, 2) != "--") {
        return false;
    }
    
    std::string key = arg.substr(2);
    size_t pos = key.find('=');
    std::string value;
    
    if (pos != std::string::npos) {
        value = key.substr(pos + 1);
        key = key.substr(0, pos);
    }
    
    if (key == "rpc-host") {
        if (value.empty()) return false;
        config.rpc_host = value;
    } else if (key == "rpc-port") {
        if (value.empty()) return false;
        config.rpc_port = std::stoi(value);
    } else if (key == "rpc-user") {
        if (value.empty()) return false;
        config.rpc_user = value;
    } else if (key == "rpc-password") {
        if (value.empty()) return false;
        config.rpc_password = value;
    } else if (key == "threads") {
        if (value.empty()) return false;
        config.num_threads = std::stoi(value);
    } else if (key == "randomq-rounds") {
        if (value.empty()) return false;
        config.randomq_rounds = std::stoull(value);
    } else if (key == "enable-avx2") {
        config.enable_avx2 = true;
    } else if (key == "enable-sse4") {
        config.enable_sse4 = true;
    } else if (key == "enable-optimized") {
        config.enable_optimized = true;
    } else if (key == "no-submit") {
        config.submit_work = false;
    } else if (key == "log-level") {
        if (value.empty()) return false;
        config.log_level = std::stoi(value);
    } else if (key == "no-stats") {
        config.show_stats = false;
    } else if (key == "stats-interval") {
        if (value.empty()) return false;
        config.stats_interval = std::stoi(value);
    } else if (key == "config") {
        if (value.empty()) return false;
        return loadFromFile(value, config);
    } else {
        return false;
    }
    
    return true;
}

void ConfigManager::setDefaults(MinerConfig& config) {
    // RPC settings
    config.rpc_host = "localhost";
    config.rpc_port = 8332;
    config.rpc_user = "";
    config.rpc_password = "";
    
    // Mining settings
    config.num_threads = std::thread::hardware_concurrency();
    config.randomq_rounds = 8192;
    config.enable_avx2 = true;
    config.enable_sse4 = true;
    config.enable_optimized = true;
    config.submit_work = true;
    
    // Logging settings
    config.log_level = 2;
    config.show_stats = true;
    config.stats_interval = 10;
}