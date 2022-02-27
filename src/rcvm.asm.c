
#ifndef RCVM_ASM_C
#define RCVM_ASM C 1

#include <string.h>
// #include "./flexbin.c"
#include "./defs.c"
#include "./strutils.c"

enum cmds {
  //register commands
  push,
  pop,

  set,
  
  m_add,
  m_sub,
  m_mul,
  m_div,

  compare,  

  //control flow commands
  jump,
};



void asmToBin (str src) {
  
  struct str_split_info splitLineInfo;
  splitLineInfo.source = src;
  splitLineInfo.delimiter = "\n";

  str_split_begin(&splitLineInfo);

  str line;

  
  for (int i=0; i<splitLineInfo.splitStringsCount; i++) {
    line = splitLineInfo.splitStrings[i];
    if (strlen(line) < 1) continue;
    struct str_split_info splitSpaceInfo;
    
    splitSpaceInfo.source = line;
    splitSpaceInfo.delimiter = " ";

    str_split_begin(&splitSpaceInfo);
    
    if (splitSpaceInfo.splitStringsCount < 1) continue;

    if (splitSpaceInfo.splitStrings[0][0] == '#') continue;

    printf("asm cmd: %s\n", splitSpaceInfo.splitStrings[0]);
    
    str_split_end(&splitSpaceInfo);
  }

  str_split_end(&splitLineInfo);
}

#endif
