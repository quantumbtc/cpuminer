/*
 * Simple compilation test for RandomQ integration
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Include the necessary headers
#include "miner.h"
#include "randomq_hash.h"

int main()
{
    printf("Testing RandomQ compilation...\n");
    
    // Test basic RandomQ functionality
    CRandomQHash hasher;
    randomq_hash_init(&hasher);
    
    const char* test_data = "test";
    unsigned char hash[32];
    
    randomq_hash_write(&hasher, (const unsigned char*)test_data, strlen(test_data));
    randomq_hash_set_nonce(&hasher, 12345);
    randomq_hash_set_rounds(&hasher, 8192);
    randomq_hash_finalize(&hasher, hash);
    
    printf("RandomQ hash calculated successfully!\n");
    printf("Hash: ");
    for (int i = 0; i < 32; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
    
    randomq_hash_cleanup(&hasher);
    
    printf("Compilation test passed!\n");
    return 0;
}