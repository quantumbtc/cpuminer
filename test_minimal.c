#include "miner.h"
#include "randomq_hash.h"

// Minimal test for compilation
int test_randomq() {
    CRandomQHash hasher;
    randomq_hash_init(&hasher);
    randomq_hash_cleanup(&hasher);
    return 0;
}
