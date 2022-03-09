
#ifndef DEFS_C
#define DEFS_C

#define str char *
#define datap unsigned char *
#include <stdbool.h>

//https://stackoverflow.com/a/8979027/8112809
bool IsLittleEndian () {
  int i=1;
  return (int)*((unsigned char *)&i)==1;
}

//https://stackoverflow.com/a/3784478/8112809
void int_to_bytes (int n, unsigned char * bytes, int bytesOffset) {
  bytes[bytesOffset+0] = (n >> 24) & 0xFF;
  bytes[bytesOffset+1] = (n >> 16) & 0xFF;
  bytes[bytesOffset+2] = (n >> 8) & 0xFF;
  bytes[bytesOffset+3] = n & 0xFF;
}

/* Write an int to a buffer, specifying the int, buffer, offset witin the buffer
 * and endianness
 *
 */
void data_write_int(datap data, int offset, int i, bool littleEndian) {
  if (IsLittleEndian() == littleEndian) {
    memcpy(data+offset, &i, sizeof(int));
  } else {
    int_to_bytes(i, data, offset);
  }
}

#endif
