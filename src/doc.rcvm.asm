
# comment line, ignored by compiler

push all|...registerIds

# example:
# push 0,1,3
# push all
# push 5-7
# push 1,3,5-7

pop all|...registerIds

add dest src
# example, adds register 1 to register 0
# add 0 1

sub dest src
# example, subtracts register 1 from register 0
# sub 0 1

mul dest src
# example, multiplies register 0 by register 1
# mul 0 1

div dest src
# example, divides register 0 by register 1
# div 0 1

set dest value
# example, sets register 0 to 0xDEADBEEF
set 0 0xDEADBEEF

label name
# creates a label, treated as comment line and disgarded after compile

jump condition line|label
# conditions:
# jump always      line - no condition
# jump lessthan    line - if less than flag is set
# jump greaterthan line - if greater than flag is set
# jump equal       line - if equal flag is set
# jump unequal     line - if unequal flag is set

# example, creates a label and jumps to it
# label landingspot
# jump always landingspot

compare dest src
# example, compares register 0 with register 1 and sets
# equal, notequal, greaterthan, lessthan flags accordingly
# compare 0 1
