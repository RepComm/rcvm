
#ifndef RCVM_C
#define RCVM_C 1

#include "./defs.c"
#include "./rcvm.asm.c"

#define FLAG_ERROR_NONE 0

struct rcvm_flags {

  str error;

  enum asm_compare compare_result;
};

struct rcvm {
  struct rcvm_flags flags;
  int programCounter;
  datap programData;
  datap programMemory;
  datap programStack;
  datap programHeap;
};

#define rcvmp struct rcvm *

rcvmp rcvm_create () {
  rcvmp result = (rcvmp) malloc (sizeof(struct rcvm));

  return result;
}

void rcvm_destroy (rcvmp vm) {

  free(vm);
}

struct asm_datum rcvm_step_datum;

void rcvm_step (rcvmp vm) {
  fetch(vm->programData, vm->programCounter, &rcvm_step_datum);
  
  switch (rcvm_step_datum.cmd) {
    case m_noop:
      break; //no operation

    case m_push:
      break;
    case m_pop:
      break;

    case m_set:
      break;
      
    case m_add:
      break;
    case m_sub:
      break;
    case m_mul:
      break;
    case m_div:
      break;
    
    case m_compare:
      break;

    case m_load:
      break;
    case m_store:
      break;
    
    
    case m_jump:
      break;
  }

  vm->programData[vm->programCounter];
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
