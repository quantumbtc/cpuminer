// Copyright (c) 2024-present The Bitquantum Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "rpc_client.h"
#include "randomq_miner.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstring>

RPCClient::RPCClient()
    : m_port(0)
    , m_connected(false)
    , m_running(false)
    , m_should_stop(false)
    , m_has_work(false)
    , m_miner(nullptr)
    , m_log_level(2)
{
    m_stats.reset();
}

RPCClient::~RPCClient() {
    stop();
}

bool RPCClient::initialize(const std::string& host, int port, 
                          const std::string& user, const std::string& password) {
    m_host = host;
    m_port = port;
    m_user = user;
    m_password = password;
    
    // Construct RPC URL
    std::ostringstream oss;
    oss << "http://" << m_user << ":" << m_password << "@" << m_host << ":" << m_port;
    m_url = oss.str();
    
    log(2, "RPC client initialized for " + m_host + ":" + std::to_string(m_port));
    return true;
}

void RPCClient::start() {
    if (m_running) {
        log(1, "RPC client is already running");
        return;
    }
    
    m_running = true;
    m_should_stop = false;
    
    // Start work update thread
    m_work_thread = std::thread(&RPCClient::workThread, this);
    
    log(2, "RPC client started");
}

void RPCClient::stop() {
    if (!m_running) {
        return;
    }
    
    log(2, "Stopping RPC client...");
    m_should_stop = true;
    
    // Wait for work thread to finish
    if (m_work_thread.joinable()) {
        m_work_thread.join();
    }
    
    m_running = false;
    m_connected = false;
    
    log(2, "RPC client stopped");
}

WorkData RPCClient::getCurrentWork() const {
    std::lock_guard<std::mutex> lock(m_work_mutex);
    return m_current_work;
}

bool RPCClient::submitSolution(const WorkData& work, uint32_t nonce, const std::string& hash) {
    // This is a placeholder implementation
    // In a real implementation, this would submit the solution via RPC
    log(2, "Submitting solution (placeholder):");
    log(2, "  Nonce: " + std::to_string(nonce));
    log(2, "  Hash: " + hash);
    log(2, "  Target: " + work.target);
    
    // Update statistics
    {
        std::lock_guard<std::mutex> lock(m_stats_mutex);
        m_stats.valid_blocks++;
    }
    
    return true;
}

void RPCClient::setMiner(RandomQMiner* miner) {
    m_miner = miner;
}

MiningStats RPCClient::getStats() const {
    std::lock_guard<std::mutex> lock(m_stats_mutex);
    return m_stats;
}

void RPCClient::connect() {
    // This is a placeholder implementation
    // In a real implementation, this would establish HTTP connection
    m_connected = true;
    log(2, "Connected to RPC server");
}

void RPCClient::disconnect() {
    m_connected = false;
    log(2, "Disconnected from RPC server");
}

std::string RPCClient::rpcCall(const std::string& method, const std::string& params) {
    // This is a placeholder implementation
    // In a real implementation, this would make HTTP POST request
    log(3, "RPC call: " + method + " with params: " + params);
    
    // Return mock response for testing
    if (method == "getblocktemplate") {
        return R"({
            "result": {
                "version": 1,
                "previousblockhash": "0000000000000000000000000000000000000000000000000000000000000000",
                "merkleroot": "0000000000000000000000000000000000000000000000000000000000000000",
                "target": "00000000ffff0000000000000000000000000000000000000000000000000000",
                "height": 1,
                "bits": "1d00ffff",
                "time": 1234567890,
                "transactions": []
            }
        })";
    }
    
    return "{}";
}

std::string RPCClient::rpcCall(const std::string& method, const std::vector<std::string>& params) {
    // Convert vector to JSON array
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < params.size(); i++) {
        if (i > 0) oss << ",";
        oss << "\"" << params[i] << "\"";
    }
    oss << "]";
    
    return rpcCall(method, oss.str());
}

void RPCClient::updateWork() {
    // Get new work from RPC
    std::string response = rpcCall("getblocktemplate");
    
    // Parse work template
    WorkData work = parseWorkTemplate(response);
    
    if (work.isValid()) {
        std::lock_guard<std::mutex> lock(m_work_mutex);
        m_current_work = work;
        m_has_work = true;
        
        // Set work in miner
        if (m_miner) {
            m_miner->setWork(work);
        }
        
        log(2, "Work updated");
    } else {
        log(1, "Failed to get valid work");
    }
}

void RPCClient::workThread() {
    log(2, "Work thread started");
    
    while (!m_should_stop) {
        try {
            // Connect if not connected
            if (!m_connected) {
                connect();
            }
            
            // Update work
            updateWork();
            
            // Wait before next update
            std::this_thread::sleep_for(std::chrono::seconds(30));
            
        } catch (const std::exception& e) {
            log(0, "Work thread error: " + std::string(e.what()));
            disconnect();
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }
    
    log(2, "Work thread stopped");
}

std::string RPCClient::httpPost(const std::string& url, const std::string& data) {
    // This is a placeholder implementation
    // In a real implementation, this would use libcurl or similar
    log(3, "HTTP POST to " + url + " with data: " + data);
    return "{}";
}

std::string RPCClient::httpGet(const std::string& url) {
    // This is a placeholder implementation
    // In a real implementation, this would use libcurl or similar
    log(3, "HTTP GET from " + url);
    return "{}";
}

WorkData RPCClient::parseWorkTemplate(const std::string& json) {
    // This is a placeholder implementation
    // In a real implementation, this would parse JSON properly
    WorkData work;
    
    // Set mock work data for testing
    work.block_template = json;
    work.previous_block_hash = "0000000000000000000000000000000000000000000000000000000000000000";
    work.merkle_root = "0000000000000000000000000000000000000000000000000000000000000000";
    work.target = "00000000ffff0000000000000000000000000000000000000000000000000000";
    work.version = 1;
    work.timestamp = 1234567890;
    work.bits = 0x1d00ffff;
    work.height = 1;
    work.nonce_start = 0;
    work.nonce_end = 0xFFFFFFFF;
    
    return work;
}

bool RPCClient::parseSubmitResponse(const std::string& json) {
    // This is a placeholder implementation
    // In a real implementation, this would parse JSON properly
    return true;
}

void RPCClient::log(int level, const std::string& message) const {
    if (level <= m_log_level) {
        const char* level_names[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
        std::cout << "[RPC] [" << level_names[level] << "] " << message << std::endl;
    }
}

// WorkData implementation
bool WorkData::isValid() const {
    return !block_template.empty() && 
           !previous_block_hash.empty() && 
           !target.empty() && 
           version != 0 && 
           timestamp != 0 && 
           bits != 0 && 
           height != 0;
}

void WorkData::print() const {
    std::cout << "Work Data:" << std::endl;
    std::cout << "  Previous Block: " << previous_block_hash << std::endl;
    std::cout << "  Target: " << target << std::endl;
    std::cout << "  Version: " << version << std::endl;
    std::cout << "  Timestamp: " << timestamp << std::endl;
    std::cout << "  Bits: " << bits << std::endl;
    std::cout << "  Height: " << height << std::endl;
    std::cout << "  Nonce Range: " << nonce_start << " - " << nonce_end << std::endl;
    std::cout << "  Transactions: " << transactions.size() << std::endl;
}

// MiningStats implementation
void MiningStats::reset() {
    total_hashes = 0;
    valid_blocks = 0;
    invalid_blocks = 0;
    hash_rate = 0.0;
    elapsed_time = 0.0;
    current_nonce = 0;
    current_target.clear();
    best_hash.clear();
    best_nonce = 0;
}

void MiningStats::print() const {
    std::cout << "\n=== Mining Statistics ===" << std::endl;
    std::cout << "Total Hashes: " << total_hashes << std::endl;
    std::cout << "Valid Blocks: " << valid_blocks << std::endl;
    std::cout << "Invalid Blocks: " << invalid_blocks << std::endl;
    std::cout << "Hash Rate: " << std::fixed << std::setprecision(2) << hash_rate << " H/s" << std::endl;
    std::cout << "Elapsed Time: " << elapsed_time << " seconds" << std::endl;
    if (!best_hash.empty()) {
        std::cout << "Best Hash: " << best_hash << std::endl;
        std::cout << "Best Nonce: " << best_nonce << std::endl;
    }
    std::cout << "=========================" << std::endl;
}