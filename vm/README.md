### What is a VM?
A virtual machine, or VM, is a machine that implements an ISA, otherwise known as instruction set architecture. The source code I have written (vm.c) is a virtual machine that represents how an ISA operates in the CPU with a given input of instructions. By using the P-machine, PAS, one area of memory contains the set of instructions from input and the other area contains the stack. After each instruction is called, the source code will print it out in the terminal. Therefore, we can see how the virtual machine works when each step is executed in the stack.

### ISA Pseudo Code:
01 - LIT 0,M  
02 - OPR, o,#  
   0 RTN  
   1 ADD  
   2 SUB  
   3 MUL  
   4 DIV
   5 EQL  
   6 NEQ  
   7 LSS  
   8 LEQ  
   9 GTR  
   10 GEQ  
03 - LOD L,M  
04 - STO L,M  
05 - CAL L,M  
06 - INC 0,M  
07 - JMP 0,M  
08 - JPC 0,M  
09 - SYS 0,1  
     SYS 0,2  
     SYS 0,3  

### Test files:
There are 2 files used for testing: _"input.txt"_ and _"recursive_input.txt"_

### Commands to Compile/Run:
```
gcc vm.c -o out
./out input.txt
```

### Output:
An output file called _"output.txt"_ should be created after successfully running the program. You should already see the results printed to the terminal screen, but it will also display those results in the textfile.

