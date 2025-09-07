#!/usr/bin/env python3
"""
Bitquantum RandomQ CPU Miner Test Script
Copyright (c) 2024-present The Bitquantum Core developers
"""

import subprocess
import sys
import time
import json
import os

def test_build():
    """Test if the miner builds successfully"""
    print("Testing build...")
    
    # Check if build directory exists
    if not os.path.exists("build"):
        print("Build directory not found. Running build script...")
        subprocess.run(["./build.sh"], check=True)
    
    # Check if executable exists
    if not os.path.exists("build/cpuminer"):
        print("Error: cpuminer executable not found")
        return False
    
    print("✓ Build test passed")
    return True

def test_help():
    """Test help command"""
    print("Testing help command...")
    
    try:
        result = subprocess.run(["./build/cpuminer", "--help"], 
                              capture_output=True, text=True, timeout=10)
        
        if result.returncode != 0:
            print(f"Error: Help command failed with return code {result.returncode}")
            print(f"Stderr: {result.stderr}")
            return False
        
        if "Bitquantum RandomQ CPU Miner" not in result.stdout:
            print("Error: Help output doesn't contain expected text")
            return False
        
        print("✓ Help test passed")
        return True
        
    except subprocess.TimeoutExpired:
        print("Error: Help command timed out")
        return False
    except Exception as e:
        print(f"Error: Help command failed: {e}")
        return False

def test_config():
    """Test configuration loading"""
    print("Testing configuration...")
    
    try:
        result = subprocess.run(["./build/cpuminer", "--config", "config.conf"], 
                              capture_output=True, text=True, timeout=5)
        
        # The miner should start and then stop (since no RPC connection)
        if "Miner initialized successfully" not in result.stdout:
            print("Error: Configuration test failed")
            print(f"Stdout: {result.stdout}")
            print(f"Stderr: {result.stderr}")
            return False
        
        print("✓ Configuration test passed")
        return True
        
    except subprocess.TimeoutExpired:
        print("Error: Configuration test timed out")
        return False
    except Exception as e:
        print(f"Error: Configuration test failed: {e}")
        return False

def test_mining_simulation():
    """Test mining simulation"""
    print("Testing mining simulation...")
    
    # Create a test configuration
    test_config = {
        "rpc_host": "localhost",
        "rpc_port": 8332,
        "rpc_user": "test",
        "rpc_password": "test",
        "num_threads": 1,
        "randomq_rounds": 1024,  # Reduced for testing
        "enable_avx2": False,
        "enable_sse4": False,
        "enable_optimized": False,
        "submit_work": False,
        "log_level": 3,
        "show_stats": True,
        "stats_interval": 5
    }
    
    # Write test config
    with open("test_config.conf", "w") as f:
        for key, value in test_config.items():
            f.write(f"{key}={value}\n")
    
    try:
        # Run miner for a short time
        result = subprocess.run(["./build/cpuminer", "--config", "test_config.conf"], 
                              capture_output=True, text=True, timeout=10)
        
        # Check for mining output
        if "Mining thread" not in result.stdout and "RandomQ" not in result.stdout:
            print("Error: Mining simulation test failed")
            print(f"Stdout: {result.stdout}")
            print(f"Stderr: {result.stderr}")
            return False
        
        print("✓ Mining simulation test passed")
        return True
        
    except subprocess.TimeoutExpired:
        print("Error: Mining simulation test timed out")
        return False
    except Exception as e:
        print(f"Error: Mining simulation test failed: {e}")
        return False
    finally:
        # Clean up test config
        if os.path.exists("test_config.conf"):
            os.remove("test_config.conf")

def main():
    """Run all tests"""
    print("Bitquantum RandomQ CPU Miner Test Suite")
    print("=" * 50)
    
    tests = [
        test_build,
        test_help,
        test_config,
        test_mining_simulation
    ]
    
    passed = 0
    total = len(tests)
    
    for test in tests:
        try:
            if test():
                passed += 1
            else:
                print(f"❌ {test.__name__} failed")
        except Exception as e:
            print(f"❌ {test.__name__} failed with exception: {e}")
        print()
    
    print("=" * 50)
    print(f"Test Results: {passed}/{total} tests passed")
    
    if passed == total:
        print("🎉 All tests passed!")
        return 0
    else:
        print("❌ Some tests failed")
        return 1

if __name__ == "__main__":
    sys.exit(main())
