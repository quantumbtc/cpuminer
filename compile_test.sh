#!/bin/bash

# Simple compilation test for RandomQ files
echo "Testing RandomQ compilation..."

# Test randomq.c
echo "Compiling randomq.c..."
x86_64-w64-mingw32-gcc -DHAVE_CONFIG_H -I. -DCURL_STATICLIB -I/usr/local/include -I./compat/jansson -O3 -I/root/2/deps-install/include -static -c randomq.c -o randomq_test.o
if [ $? -eq 0 ]; then
    echo "✓ randomq.c compiled successfully"
    rm -f randomq_test.o
else
    echo "✗ randomq.c compilation failed"
fi

# Test randomq_hash.c
echo "Compiling randomq_hash.c..."
x86_64-w64-mingw32-gcc -DHAVE_CONFIG_H -I. -DCURL_STATICLIB -I/usr/local/include -I./compat/jansson -O3 -I/root/2/deps-install/include -static -c randomq_hash.c -o randomq_hash_test.o
if [ $? -eq 0 ]; then
    echo "✓ randomq_hash.c compiled successfully"
    rm -f randomq_hash_test.o
else
    echo "✗ randomq_hash.c compilation failed"
fi

# Test randomq_mining.c
echo "Compiling randomq_mining.c..."
x86_64-w64-mingw32-gcc -DHAVE_CONFIG_H -I. -DCURL_STATICLIB -I/usr/local/include -I./compat/jansson -O3 -I/root/2/deps-install/include -static -c randomq_mining.c -o randomq_mining_test.o
if [ $? -eq 0 ]; then
    echo "✓ randomq_mining.c compiled successfully"
    rm -f randomq_mining_test.o
else
    echo "✗ randomq_mining.c compilation failed"
fi

echo "Compilation test completed."
