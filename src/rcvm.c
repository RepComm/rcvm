
#ifndef RCVM_C
#define RCVM_C 1

#include "./defs.c"
#include "./rcvm.asm.c"

#define FLAG_ERROR_NONE 0

struct rcvm_flags {

  str error;
};

struct rcvm {
  struct rcvm_flags flags;
};

#define rcvmp struct rcvm *

rcvmp rcvm_create () {
  rcvmp result = (rcvmp) malloc (sizeof(struct rcvm));

  return result;
}

void rcvm_destroy (rcvmp vm) {

  free(vm);
}

void rcvm_step (rcvmp vm) {

}

void rcvm_load_src (rcvmp vm, str src) {

}

void rcvm_load_asm (rcvmp vm, str src) {
  asmToBin(src);

  rcvm_load_asmbin(vm, bin);
}

void rcvm_load_asmbin (rcvmp vm, datap src) {

}

#endif
