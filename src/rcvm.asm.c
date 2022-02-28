
#ifndef RCVM_ASM_C
#define RCVM_ASM C 1

#include <string.h>
// #include "./flexbin.c"
#include "./defs.c"
#include "./strutils.c"

enum asm_compare {
  compare_always,
  compare_equal,
  compare_unequal,
  compare_greaterthan,
  compare_lessthan,
};

enum asm_cmd {
  //no operation
  m_noop,
  
  //register commands
  m_push, //push registers to stack
  m_pop,  //pop registers from stack

  m_set,  //set register value directly
  
  m_add,  //add B register to A
  m_sub,  //sub B register from A
  m_mul,  //mul A register by B
  m_div,  //div A register by B

  m_compare, //compare A and B registers and set flags accordingly

  m_load,    //load memory from RAM address into register
  m_store,   //store memory from register into RAM address

  //control flow commands
  m_jump,    //jump to memory

};

struct asm_datum {
  //also enum asm_cmd
  unsigned char cmd;
  
  unsigned char jump_condition;
  unsigned int jump_landingspot;

  unsigned short push_pop_registers;
  
  unsigned char registerA;
  unsigned char registerB;

  int value;

  unsigned int ramAddress;
};

//https://stackoverflow.com/a/11314653/8112809
//http://gcc.gnu.org/onlinedocs/gcc/Structures-unions-enumerations-and-bit_002dfields-implementation.html
void fetch (datap data, int programCounter, struct asm_datum * datum) {
  datum->cmd = (unsigned char) data[programCounter];
  programCounter += sizeof(unsigned char);

  switch (datum->cmd) {
  case m_jump:
    datum->jump_condition = (unsigned char) data[programCounter];
    programCounter += sizeof(unsigned char);

    datum->jump_landingspot = (unsigned int) data[programCounter];
    programCounter += sizeof(unsigned int);
    break;
  case m_push:
  case m_pop:
    datum->push_pop_registers = (unsigned short) data[programCounter];
    programCounter += sizeof(unsigned short);
    break;
  case m_set:
    datum->registerA = (unsigned char) data[programCounter];
    programCounter += sizeof(unsigned char);
    
    datum->value = (int) data[programCounter];
    programCounter += sizeof(int);
    break;
  case m_add:
  case m_sub:
  case m_mul:
  case m_div:
  case m_compare:
    datum->registerA = (unsigned char) data[programCounter];
    programCounter += sizeof(unsigned char);

    datum->registerB = (unsigned char) data[programCounter];
    programCounter += sizeof(unsigned char);
    break;
  case m_load:
  case m_store:
    datum->registerA = (unsigned char) data[programCounter];
    programCounter += sizeof(unsigned char);

    datum->ramAddress = (unsigned int) data[programCounter];
    programCounter += sizeof(unsigned int);
    break;
  default:
    break;
  }
}
void post (datap data, int programCounter,  struct asm_datum * datum) {
  data[programCounter] = datum->cmd;
}

enum asm_cmd asm_cmd_from_str (str cmd) {
  if (str_equal(cmd, "push")) return m_push;
  if (str_equal(cmd, "pop")) return m_pop;
  if (str_equal(cmd, "set")) return m_set;
  if (str_equal(cmd, "add")) return m_add;
  if (str_equal(cmd, "sub")) return m_sub;
  if (str_equal(cmd, "mul")) return m_mul;
  if (str_equal(cmd, "div")) return m_div;
  if (str_equal(cmd, "compare")) return m_compare;
  if (str_equal(cmd, "jump")) return m_jump;
  if (str_equal(cmd, "noop")) return m_noop;
  return -1;
}

void asmToBin (str src) {
  
  struct str_split_info splitLineInfo;
  splitLineInfo.source = src;
  splitLineInfo.delimiter = "\n";

  str_split_begin(&splitLineInfo);

  str line;

  
  for (int i=0; i<splitLineInfo.splitStringsCount; i++) {
    line = splitLineInfo.splitStrings[i];
    struct str_split_info splitSpaceInfo;
    
    splitSpaceInfo.source = line;
    splitSpaceInfo.delimiter = " ";

    str_split_begin(&splitSpaceInfo);
    
    if (splitSpaceInfo.splitStringsCount < 1) continue;

    if (splitSpaceInfo.splitStrings[0][0] == '#') continue;

    int cmdId = asm_cmd_from_str(splitSpaceInfo.splitStrings[0]);
    printf("%s -> %i\n", splitSpaceInfo.splitStrings[0], cmdId);
    
    str_split_end(&splitSpaceInfo);
  }

  str_split_end(&splitLineInfo);
}

#endif
