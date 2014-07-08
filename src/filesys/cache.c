#include "cache.h"
#include <debug.h>
#include <list.h>
#include "threads/synch.h"
#include "threads/thread.h"
#include "threads/malloc.h"
#include "devices/timer.h"
#include "filesys.h"

/* Cache with space for 64 blocks */
static struct list cache_list;
static struct lock cache_lock;

struct cache_entry
  {
    uint8_t block[BLOCK_SECTOR_SIZE];
    block_sector_t sector;

    struct list_elem cache_elem;
    bool is_dirty;
    /* For LRU cache replacement */
    int64_t last_access;
  };

void cache_init (void)
{
  list_init (&cache_list);
  lock_init (&cache_lock);

  /* Periodically write dirty blocks */
  thread_create ("write_task", PRI_DEFAULT, (void*)cache_task_evict_dirty, NULL);
}

bool cache_is_cached (block_sector_t sector)
{
  return cache_get_cache_entry (sector) != NULL;
}

/* Returns the corresponding cached entry for the given SECTOR.
 * Returns null if no cache is found. */
struct cache_entry* cache_get_cache_entry (block_sector_t sector)
{
  struct list_elem *e;
  for (e = list_begin (&cache_list); e != list_end (&cache_list); e = list_next (e))
    {
      struct cache_entry *entry = list_entry (e, struct cache_entry, cache_elem);
      if (entry->sector == sector)
        return entry;
    }
  return NULL;
}

/* Adds the given SECTOR to the cache and returns the cached entry */
struct cache_entry* cache_add (block_sector_t sector)
{
  // Cache has no space left
  uint32_t cache_size = list_size (&cache_list);
  if (cache_size >= MAX_CACHE_SECTORS_SIZE)
    cache_evict ();

  struct cache_entry *entry = (struct cache_entry*)malloc (sizeof (struct cache_entry));

  // Populate the new cache entry
  block_read (fs_device, sector, &entry->block);
  entry->sector = sector;
  entry->is_dirty = false;
  // Set last access time to current time
  entry->last_access = timer_ticks ();

  list_push_back (&cache_list, &entry->cache_elem);

  return entry;
}

/* Evicts one block from cache */
void cache_evict (void)
{
  // Get a dirty sector to evict
  struct cache_entry *entry;
  do {
    entry = cache_get_lru ();
  } while (!entry->is_dirty);

  block_write (fs_device, entry->sector, &entry->block);
}

/* Evicts all dirty cache */
void cache_evict_all_dirty (void)
{
  struct list_elem *e;
  for (e = list_begin (&cache_list); e != list_end (&cache_list); e = list_next (e))
    {
      struct cache_entry *entry = list_entry (e, struct cache_entry, cache_elem);
      if (entry && entry->is_dirty)
        {
          block_write (fs_device, entry->sector, &entry->block);
          entry->is_dirty = false;
        }
    }
}

void cache_flush (void)
{
  // Save dirty data to disk
  cache_evict_all_dirty ();

  // Free
  struct list_elem *e;
  for (e = list_begin (&cache_list); e != list_end (&cache_list); e = list_next (e))
    {
      struct cache_entry *entry = list_entry (e, struct cache_entry, cache_elem);

      list_remove (e);
      free (entry);
    }
}

/* Writes all dirty blocks to disk periodically */
void cache_task_evict_dirty (void)
{
  while (1)
    {
      timer_sleep (AUTO_WRITE_FREQ);

      cache_evict_all_dirty ();
    }
}

void cache_start_task_fetch_next_block (block_sector_t sector)
{
  block_sector_t *to_read = (block_sector_t*)malloc (sizeof (block_sector_t));

  ASSERT (to_read);

  *to_read = sector + 1;
  thread_create ("fetch_next_block_task", PRI_DEFAULT,
      cache_task_fetch_next_block, to_read);
}

void cache_task_fetch_next_block (void *to_read)
{
  block_sector_t sector = *( (block_sector_t*)to_read );
  lock_acquire (&cache_lock);

  // Cache if it's not cached yet
  if (cache_is_cached (sector))
    cache_add (sector);

  lock_release (&cache_lock);
  free (to_read);
}

/* Returns the least recently used block */
struct cache_entry* cache_get_lru (void)
{
  struct list_elem *e = list_begin (&cache_list);
  struct cache_entry *c = list_entry (e, struct cache_entry, cache_elem);

  struct cache_entry *min_entry = c;
  uint64_t min = c->last_access;

  // Find the entry that has been around the longest
  while (e != list_end (&cache_list))
    {
      c = list_entry (e, struct cache_entry, cache_elem);

      if (c->last_access < min)
      {
        min = c->last_access;
        min_entry = c;
      }

      e = list_next (e);
    }

  return c;
}
