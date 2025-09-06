// Simple diagnostic to test RandomQ compilation
#include <stdio.h>
#include <stdint.h>

// Test if we can include the headers
#include "miner.h"
#include "randomq.h"
#include "randomq_hash.h"

int main() {
    printf("Headers included successfully\n");
    
    // Test basic functionality
    CRandomQ ctx;
    randomq_init(&ctx);
    printf("CRandomQ initialized\n");
    
    CRandomQHash hasher;
    randomq_hash_init(&hasher);
    printf("CRandomQHash initialized\n");
    
    printf("All tests passed\n");
    return 0;
}
