
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

// #include "./rcvm.c"
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

  asmToBin("set 0 0xDEADBEEF\nset 1 0\nset 2 1\nlabel loopstart\nadd 1 2\ncompare 0 1\njump lessthan loopstart\nlabel loopend");

  // while (doLoop) {
  //   rcvm_step(vm);
  // }

  // clear out any devices and free memory
  cleanup();
}
