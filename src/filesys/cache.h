#include <stdbool.h>
#include "devices/block.h"
/* Maximum of 64 sectors */
#define MAX_CACHE_SECTORS_SIZE 64
/* Automatically flush cache to disk */
#define AUTO_WRITE_FREQ 8

/* One entry in the stored in cache */
struct cache_entry;

void cache_init (void);

bool cache_is_cached (block_sector_t sector);
struct cache_entry* cache_get_cache_entry (block_sector_t sector);
struct cache_entry* cache_add (block_sector_t sector);
/* Evicts the block from cache */
void cache_evict (void);
/* Write all dirty blocks to disk */
void cache_evict_all_dirty (void);
/* Flush cache and write all dirty blocks to disk. Used when exiting */
void cache_flush (void);
/* Returns the least recently used block */
struct cache_entry* cache_get_lru (void);
/* Thread stuff */
/* Writes all dirty blocks to disk periodically */
void cache_task_evict_dirty (void);
void cache_start_task_fetch_next_block (block_sector_t sector);
void cache_task_fetch_next_block (void *to_read);
