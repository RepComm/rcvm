
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

// #include "./rcvm.c"
#include "./fileutils.c"
#include "./rcvm.asm.c"

bool doLoop;

// rcvmp vm;

void init() {
  doLoop = true;
  
  // vm = rcvm_create();

  // rcvm_parse(vm, "");

}
void cleanup() {
  // rcvm_destroy(vm);
}

bool streq(const char *a, const char *b) { return strcmp(a, b) == 0; }

int main(int argc, char **argv) {
  // allocation and setup
  init();

  struct TextFile f;
  f.fname = "./src/main.rcvm.asm";
  if (!TextFile_read(&f)) {
    printf("couldn't read assembly file");
    TextFile_clear(&f);
    return -1;
  }
  // printf("read file: %s", f.data);
  struct asm_to_bin_info info;
  info.src = f.data;

  assemble_begin(&info);

  for (int i=0; i<info.resultByteLength;) {
    for (int j=0; j<16; j++) {
      if (i > info.resultByteLength-1) break;
      printf(
        "%02hhX ",
        info.result[i]
      );
      i++;
    }
    printf("\n");
  }

  struct BinFile outFile;
  outFile.fname = "./main.rcvm.o";
  outFile.data = info.result;
  outFile.dataLength = info.resultByteLength;
  BinFile_write(&outFile);

  assemble_end(&info);

  TextFile_clear(&f);


  // while (doLoop) {
  //   rcvm_step(vm);
  // }

  // clear out any devices and free memory
  cleanup();
}
