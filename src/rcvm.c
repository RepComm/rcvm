
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
  int registers[16];
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

int rcvm_register_get (rcvmp vm, int reg) {
  return vm->registers[reg];
}
void rcvm_register_set(rcvmp vm, int reg, int value) {
  vm->registers[reg] = value;
}

int rcvm_ram_get(rcvmp vm, int ramAddress) {
  return vm->programMemory[ramAddress];
}
void rcvm_ram_set(rcvmp vm, int ramAddress, int value) {
  vm->programMemory[ramAddress] = value;
}

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
      vm->registers[
        rcvm_step_datum.registerA
      ] += vm->registers[
        rcvm_step_datum.registerB
      ];
      break;
    case m_sub:
      vm->registers[
        rcvm_step_datum.registerA
      ] -= vm->registers[
        rcvm_step_datum.registerB
      ];
      break;
    case m_mul:
      vm->registers[
        rcvm_step_datum.registerA
      ] *= vm->registers[
        rcvm_step_datum.registerB
      ];
      break;
    case m_div:
      vm->registers[
        rcvm_step_datum.registerA
      ] /= vm->registers[
        rcvm_step_datum.registerB
      ];
      break;
    
    case m_compare:
      int rAv = rcvm_register_get(
        vm, rcvm_step_datum.registerA
      );
      int rBv = rcvm_register_get(
        vm, rcvm_step_datum.registerB
      );
      
      if (rAv < rBv) {
        vm->flags.compare_result = compare_lessthan; 
      } else if (rAv > rBv) {
        vm->flags.compare_result = compare_greaterthan;
      } else {
        vm->flags.compare_result = compare_equal;
      }
      break;

    case m_load:
      rcvm_register_set(
        vm,
        rcvm_step_datum.registerA,
        rcvm_ram_get(vm, rcvm_step_datum.ramAddress)
      );
      break;
    case m_store:
      break;
      rcvm_ram_set(
        vm,
        rcvm_step_datum.ramAddress,
        rcvm_register_get(vm, rcvm_step_datum.registerA)
      );
    case m_jump:
      vm->programCounter = rcvm_step_datum.jump_landingspot;
      break;
  }

  vm->programData[vm->programCounter];
}

void rcvm_load_src (rcvmp vm, str src) {

}

void rcvm_load_asm (rcvmp vm, str src) {
  assemble_begin(src);

  // rcvm_load_asmbin(vm, bin);
}

void rcvm_load_asmbin (rcvmp vm, datap src) {

}

#endif
