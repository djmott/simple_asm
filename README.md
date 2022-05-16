# simple_asm

This is an example of compiling a psudo assembly language program in C++. The file user_code.txt is consumed by simple_asm.cpp at compile time, converted into a collection of instructions and executed at compile time with proper modern compiler and optimizer settings. 

The user_code.txt file can contain N lines of the following syntax:

start := instruction+ <EOF>;

instruction := push | pop | add | sub | mul | div;

push := 'push' int;

pop := 'pop' ;

add := 'add' ;

sub := 'sub' ;

mul := 'mul' ;

div := 'div' ;

The maths operations pop the top two items in the stack and push the result. The return value of main is the result of the calculations and SHOULD produce zero runtime-time code to calculate the result.
