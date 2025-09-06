/*
 * Copyright (c) 2024-present The Bitquantum Core developers
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 */

#include "miner.h"
#include "randomq_hash.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

int scanhash_randomq(int thr_id, uint32_t *pdata, const uint32_t *ptarget,
	uint32_t max_nonce, unsigned long *hashes_done)
{
	uint32_t data[20];
	uint32_t hash[8];
	uint32_t n = pdata[19] - 1;
	const uint32_t first_nonce = pdata[19];
	const uint32_t Htarg = ptarget[7];
	
	// Copy block header data
	memcpy(data, pdata, 80);
	
	do {
		data[19] = ++n;
		
		// Calculate RandomQ hash
		CRandomQHash hasher;
		randomq_hash_init(&hasher);
		randomq_hash_write(&hasher, (unsigned char*)data, 80);
		randomq_hash_set_nonce(&hasher, data[19]);
		randomq_hash_set_rounds(&hasher, 8192);
		randomq_hash_finalize(&hasher, (unsigned char*)hash);
		randomq_hash_cleanup(&hasher);
		
		// Check if hash meets target
		if (swab32(hash[7]) <= Htarg) {
			pdata[19] = data[19];
			*hashes_done = n - first_nonce + 1;
			return 1;
		}
	} while (n < max_nonce && !work_restart[thr_id].restart);
	
	*hashes_done = n - first_nonce + 1;
	pdata[19] = n;
	return 0;
}
