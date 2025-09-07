# Bitquantum RandomQ CPU Miner

A high-performance CPU miner for Bitquantum Core using the RandomQ post-quantum algorithm.

## Features

- **Post-Quantum Security**: Uses RandomQ algorithm resistant to quantum attacks
- **Multi-threading**: Supports multiple CPU cores for maximum performance
- **Optimized**: AVX2, SSE4, and other CPU optimizations
- **RPC Integration**: Connects to Bitquantum Core via JSON-RPC
- **Configurable**: Extensive configuration options
- **Cross-platform**: Supports Linux, Windows, and macOS

## RandomQ Algorithm

RandomQ is a post-quantum cryptographic algorithm designed to resist quantum computer attacks:

- **Lattice-based**: Uses mathematical problems that are hard for quantum computers
- **Randomized**: Randomized transitions increase attack complexity
- **Large state**: 200-byte internal state provides sufficient entropy
- **Configurable rounds**: Adjustable number of rounds for security/performance trade-off

## Building

### Linux/macOS

```bash
# Install dependencies
sudo apt-get install build-essential cmake libcurl4-openssl-dev nlohmann-json3-dev

# Build
./build.sh

# Run tests
python3 test_randomq.py
```

### Windows

```cmd
REM Install Visual Studio 2022 with C++ development tools
REM Install vcpkg and install dependencies:
REM   vcpkg install curl nlohmann-json

REM Build
build_windows.bat
```

## Usage

### Basic Usage

```bash
# Start mining with default settings
./cpuminer

# Start mining with custom settings
./cpuminer --rpc-host localhost --rpc-port 8332 --threads 4

# Use configuration file
./cpuminer --config config.conf
```

### Command Line Options

- `--rpc-host <host>`: RPC server host (default: localhost)
- `--rpc-port <port>`: RPC server port (default: 8332)
- `--rpc-user <user>`: RPC username
- `--rpc-password <pass>`: RPC password
- `--threads <count>`: Number of mining threads (default: auto)
- `--randomq-rounds <num>`: RandomQ rounds (default: 8192)
- `--enable-avx2`: Enable AVX2 optimizations
- `--enable-sse4`: Enable SSE4 optimizations
- `--enable-optimized`: Enable optimized algorithms
- `--no-submit`: Don't submit work to pool
- `--log-level <level>`: Log level 0-3 (default: 2)
- `--no-stats`: Don't show statistics
- `--stats-interval <sec>`: Statistics update interval (default: 10)
- `--config <file>`: Load configuration from file
- `--help, -h`: Show help message

### Configuration File

Create a `config.conf` file with the following format:

```ini
# RPC connection settings
rpc_host=localhost
rpc_port=8332
rpc_user=your_username
rpc_password=your_password

# Mining settings
num_threads=4
randomq_rounds=8192
enable_avx2=true
enable_sse4=true
enable_optimized=true
submit_work=true

# Logging settings
log_level=2
show_stats=true
stats_interval=10
```

## Performance

### Hash Rate Expectations

- **SHA256D**: ~1000 MB/s (reference)
- **RandomQ**: ~10-50 MB/s (depends on rounds)

### Optimization Tips

1. **Enable CPU optimizations**: Use `--enable-avx2` and `--enable-sse4`
2. **Adjust thread count**: Use all available CPU cores
3. **Tune RandomQ rounds**: Balance security vs performance
4. **Use optimized build**: Compile with `-O3` and `-march=native`

## Security Considerations

### Post-Quantum Properties

1. **Lattice-based**: Quantum-resistant mathematical foundations
2. **Randomization**: Randomized transitions increase attack complexity
3. **Large state**: 200-byte state provides sufficient entropy

### Attack Mitigations

- **Precomputation**: Mitigated by random seed and nonce
- **Collision**: Dual SHA256 provides extra protection
- **Quantum**: Designed to resist quantum attacks

## Architecture

### Components

- **RandomQMiner**: Core mining logic and thread management
- **RPCClient**: Communication with Bitquantum Core
- **ConfigManager**: Configuration loading and validation
- **RandomQHash**: RandomQ algorithm implementation
- **RandomQMining**: Mining utilities and optimizations

### Threading Model

- **Main Thread**: Orchestrates mining and RPC communication
- **Mining Threads**: Perform actual hash calculations
- **RPC Thread**: Handles work updates and solution submission
- **Stats Thread**: Updates and displays statistics

## Development

### Project Structure

```
cpuminer/
├── main.cpp              # Main entry point
├── miner.h/cpp           # Main miner class
├── randomq_miner.h/cpp   # RandomQ mining implementation
├── randomq_hash.h/cpp    # RandomQ hash implementation
├── randomq_mining.h/cpp  # Mining utilities
├── rpc_client.h/cpp      # RPC communication
├── config.h/cpp          # Configuration management
├── CMakeLists.txt        # Build configuration
├── build.sh              # Linux build script
├── build_windows.bat     # Windows build script
├── config.conf           # Example configuration
├── test_randomq.py       # Test suite
└── README_RANDOMQ.md     # This file
```

### Building from Source

1. Clone the repository
2. Install dependencies
3. Run build script
4. Run tests
5. Configure and run

### Testing

```bash
# Run all tests
python3 test_randomq.py

# Run specific test
python3 -c "import test_randomq; test_randomq.test_build()"
```

## Troubleshooting

### Common Issues

1. **Build errors**: Check dependencies and compiler version
2. **RPC connection failed**: Verify RPC settings and network
3. **Low hash rate**: Enable optimizations and check CPU usage
4. **Memory issues**: Reduce thread count or RandomQ rounds

### Debug Mode

```bash
# Enable debug logging
./cpuminer --log-level 3

# Disable statistics for cleaner output
./cpuminer --no-stats
```

## License

Copyright (c) 2024-present The Bitquantum Core developers

Distributed under the MIT software license, see the accompanying file COPYING or http://www.opensource.org/licenses/mit-license.php.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests
5. Submit a pull request

## Support

For issues and questions:
- GitHub Issues: [Create an issue](https://github.com/bitquantum/bitquantum/issues)
- Documentation: [Read the docs](https://bitquantumcore.org/docs)
- Community: [Join the forum](https://bitquantumcore.org/forum)
