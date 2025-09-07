# CPUMiner - Bitquantum RandomQ CPU Miner

A high-performance CPU miner for Bitquantum Core using the RandomQ post-quantum algorithm.

## Features

- **RandomQ Algorithm**: Implements the RandomQ post-quantum hashing algorithm
- **Multi-threading**: Supports multiple CPU cores for parallel mining
- **RPC Integration**: Communicates with Bitquantum Core via JSON-RPC
- **Optimized Performance**: AVX2, SSE4, and other CPU optimizations
- **Real-time Statistics**: Live mining statistics and hash rate monitoring
- **Long Polling**: Efficient work updates using long polling
- **Configurable**: Extensive configuration options via file or command line

## Requirements

- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
- CMake 3.16+
- libcurl
- nlohmann/json
- Bitquantum Core node running with RPC enabled

## Building

### Prerequisites

Install required dependencies:

```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake libcurl4-openssl-dev nlohmann-json3-dev

# CentOS/RHEL
sudo yum install gcc-c++ cmake libcurl-devel nlohmann-json3-devel

# macOS
brew install cmake curl nlohmann-json
```

### Linux Compilation

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ../cpuminer

# Build
make -j$(nproc)
```


## Usage

### Basic Usage

```bash
# Using command line arguments
./cpuminer --rpc-user bitquantum --rpc-password bitquantum123 --threads 4

# Using configuration file
./cpuminer --config config.conf
```

### Configuration

Create a configuration file (e.g., `config.conf`):

```ini
# RPC Connection
rpc_host=127.0.0.1
rpc_port=18332
rpc_user=bitquantum
rpc_password=bitquantum123

# Mining
num_threads=4
randomq_rounds=8192

# Performance
enable_avx2=true
enable_sse4=true
enable_optimized=true

# Logging
log_level=2
show_stats=true
stats_interval=10
```

### Command Line Options

```
Options:
  -h, --help              Show help message
  -v, --version           Show version information
  --config FILE           Load configuration from file

RPC Connection:
  --rpc-host HOST         RPC server host (default: 127.0.0.1)
  --rpc-port PORT         RPC server port (default: 18332)
  --rpc-user USER         RPC username
  --rpc-password PASS     RPC password

Mining:
  --threads N             Number of mining threads (0=auto)
  --nonce-start N         Starting nonce value
  --nonce-end N           Ending nonce value
  --randomq-rounds N      RandomQ algorithm rounds (default: 8192)

Performance:
  --no-avx2               Disable AVX2 optimizations
  --no-sse4               Disable SSE4 optimizations
  --no-optimized          Disable optimized algorithms

Logging:
  --log-level N           Log level (0=error, 1=warning, 2=info, 3=debug)
  --no-stats              Disable statistics display
  --stats-interval N      Statistics update interval in seconds

Advanced:
  --no-long-polling       Disable long polling
  --work-timeout N        Work timeout in seconds (default: 300)
  --retry-interval N      Retry interval in seconds (default: 5)
  --no-submit             Don't submit found blocks
```

## RandomQ Algorithm

The RandomQ algorithm is a post-quantum proof-of-work algorithm that provides:

- **Quantum Resistance**: Resistant to Shor's and Grover's quantum attacks
- **Lattice-based Security**: Uses lattice cryptography principles
- **Optimized Performance**: Efficient CPU implementation
- **Configurable Rounds**: Adjustable security/performance trade-off

### Algorithm Pipeline

```
Block Header -> SHA256 -> RandomQ -> SHA256 -> Final Hash
```

- **SHA256**: Standard SHA-256 hashing
- **RandomQ**: Post-quantum algorithm with 8192 rounds (configurable)
- **Final SHA256**: Additional security layer

## Performance

### Optimization Features

- **AVX2 Support**: Vectorized operations for modern CPUs
- **SSE4 Support**: SIMD instructions for older CPUs
- **Multi-threading**: Parallel nonce searching
- **Optimized Algorithms**: Hand-tuned RandomQ implementation

### Expected Performance

Performance varies by CPU architecture:

- **Intel Core i7-8700K**: ~50-100 KH/s (8 threads)
- **AMD Ryzen 7 3700X**: ~60-120 KH/s (8 threads)
- **Intel Xeon E5-2680 v4**: ~40-80 KH/s (14 threads)

*Performance depends on RandomQ rounds, CPU architecture, and optimization settings.*

## Troubleshooting

### Common Issues

1. **RPC Connection Failed**
   - Verify Bitquantum Core is running
   - Check RPC credentials and port
   - Ensure RPC is enabled in bitquantum.conf

2. **Low Hash Rate**
   - Enable optimizations (AVX2, SSE4)
   - Increase thread count
   - Check CPU temperature and throttling

3. **No Work Received**
   - Verify network connection
   - Check RPC client logs
   - Ensure node is synced

### Debug Mode

Enable debug logging for detailed information:

```bash
./cpuminer --log-level 3 --rpc-user user --rpc-password pass
```

## Development

### Project Structure

```
cpuminer/
├── CMakeLists.txt                 # CMake构建配置
├── CMakePresets.json              # CMake预设配置
├── build.sh                       # Linux构建脚本
├── build_windows.sh               # Windows构建脚本
├── build_windows.bat              # Windows批处理构建脚本
├── install_dependencies_linux.sh  # Linux依赖安装脚本
├── install_dependencies_windows.bat # Windows依赖安装脚本
├── main.cpp                       # 主程序入口
├── miner.h/cpp                    # 主挖矿类
├── randomq_miner.h/cpp            # RandomQ挖矿逻辑
├── rpc_client.h/cpp               # RPC通信
├── config.h/cpp                   # 配置管理
├── config.conf                    # 默认配置文件
├── cmake/                         # CMake工具链文件
├── README.md                      # 项目说明文档
└── README_CMAKE.md                # CMake构建指南
```

### Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## License

This project is licensed under the MIT License. See the COPYING file for details.

## Support

For support and questions:

- GitHub Issues: [Create an issue](https://github.com/bitquantum/cpuminer/issues)
- Documentation: [Read the docs](https://docs.bitquantum.org/mining)
- Community: [Join our Discord](https://discord.gg/bitquantum)

## Changelog

### v1.0.0
- Initial release
- RandomQ algorithm implementation
- Multi-threading support
- RPC integration
- Performance optimizations
- Configuration system
- Statistics monitoring
