/*
 * Test program for RandomQ algorithm integration
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "randomq_hash.h"

int main()
{
    printf("Testing RandomQ algorithm integration...\n");
    
    // Test data
    const char* test_data = "Hello, RandomQ!";
    unsigned char hash[RANDOMQ_HASH_OUTPUT_SIZE];
    
    // Initialize hasher
    CRandomQHash hasher;
    randomq_hash_init(&hasher);
    
    // Write test data
    randomq_hash_write(&hasher, (const unsigned char*)test_data, strlen(test_data));
    
    // Set parameters
    randomq_hash_set_nonce(&hasher, 12345);
    randomq_hash_set_rounds(&hasher, 8192);
    
    // Finalize hash
    randomq_hash_finalize(&hasher, hash);
    
    // Print result
    printf("Input: %s\n", test_data);
    printf("RandomQ Hash: ");
    for (int i = 0; i < RANDOMQ_HASH_OUTPUT_SIZE; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
    
    // Cleanup
    randomq_hash_cleanup(&hasher);
    
    printf("RandomQ test completed successfully!\n");
    return 0;
}
