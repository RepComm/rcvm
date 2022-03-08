
# example program

set 0 0xDEADBEEF
set 1 0
set 2 1

label loopstart
add 1 2

compare 0 1

jump lessthan loopstart

label loopend