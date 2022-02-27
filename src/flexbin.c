
#ifndef FLEXBIN_C
#define FLEXBIN_C 1

#ifndef FLEXBIN_BLOCK_SIZE
#define FLEXBIN_BLOCK_SIZE 1024
#endif

#include <memory.h>
#include <stdbool.h>
#include <math.h>

#include "./defs.c"

struct block {
  struct block * root;
  struct block * next;
  datap data;
};
#define blockp struct block *

blockp flexbin_create_block (blockp parent) {
  blockp result = (blockp) malloc(sizeof(struct block));

  if (parent != NULL) {
    result->root = parent->root;
    parent->next = result;
  }

  result->data = (datap) malloc(sizeof(unsigned char) * FLEXBIN_BLOCK_SIZE);

  return result;
}

/**WARNING: Does not destroy subsequent children blocks!
 * This method is used internally, where the chain is iterated backwards for destruction
 */
void flexbin_destroy_block (blockp block) {
  free(block->data);
  free(block);
}

struct flexbin {
  blockp chain;
  blockp cachedBlock;
  int cachedBlockIndex;
  int byteLength;
};

#define flexbinp struct flexbin *

flexbinp flexbin_create () {
  flexbinp result = (flexbinp) malloc(sizeof(struct flexbin));
  result->byteLength = 0;
  return result;
}

bool flexbin_cache_is (flexbinp b, int index) {
  return (b->cachedBlockIndex == index && b->cachedBlock != NULL);
}

void flexbin_cache_set (flexbinp b, int index, blockp block) {
  b->cachedBlock = block;
  b->cachedBlockIndex = index;
}

int flexbin_byte_to_block (int byteIndex) {
  return (int) floorf ( (float) byteIndex / (float) FLEXBIN_BLOCK_SIZE);
}

int flexbin_byte_to_local (int byteIndex) {
  return byteIndex % FLEXBIN_BLOCK_SIZE;
}

blockp flexbin_get_block (flexbinp b, int blockIndex) {
  if (flexbin_cache_is(b, blockIndex)) return b->cachedBlock;
  blockp result = b->chain;
  for (int i=0; i<blockIndex; i++) {
    if (result->next != NULL) {
      result = result->next;
    } else {
      return NULL;
    }
  }
  return result;
}

blockp flexbin_get_block_from_byte (flexbinp b, int byteIndex) {
  int blockIndex = flexbin_byte_to_block(byteIndex);
  return flexbin_get_block(b, blockIndex);
}

unsigned char flexbin_read_uchar (flexbinp b, int byteIndex) {
  blockp block = flexbin_get_block_from_byte(b, byteIndex);
  if (block == NULL) return NULL;
  if (block->data == NULL) return NULL;
  int idx = flexbin_byte_to_local(byteIndex);
  return block->data[idx];
}

char flexbin_read_char (flexbinp b, int byteIndex) {
  unsigned char uch = flexbin_read_uchar(b, byteIndex);
  if (uch == NULL) return NULL;
  return (char) uch;
}

/**Read a C string from the binary until reaching NULL char or end of data
 * To get length without allocating 'out', simply pass a NULL for 'out'
 * 
 * Returns the length read, and reads data into 'out' if not NULL
 */
int flexbin_read_cstr (flexbinp b, int byteindex, str out) {
  char current;
  int outIdx = 0;

  for (int i=byteindex; ; i++) {
    current = flexbin_read_char(b, i);
    if (current == NULL) break;
    if (out != NULL) out[outIdx] = current;
    outIdx ++;
  }
  return outIdx+1;
}

void flexbin_copy (
  flexbinp src,
  int srcIndex,
  
  int width,

  datap dest,
  int destIndex
) {
  for (int i=0; i<width; i++) {
    dest[destIndex] = flexbin_read_uchar(src, srcIndex);
    srcIndex++;
    destIndex++;
  }
}


void flexbin_write_uchar (flexbinp b, int byteIndex, unsigned char uch) {
  blockp block = flexbin_get_block_from_byte(b, byteIndex);
  if (block == NULL) return NULL;
  if (block->data == NULL) return NULL;
  int idx = flexbin_byte_to_local(byteIndex);
  block->data[idx] = uch;
}

#endif
