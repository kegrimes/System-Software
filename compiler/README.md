### What is a Complier?
A complier takes high level languages, like JAVA or C, and translates them into assembly language. This is done in a few steps: (1) create a lexical analyzer that reads in characters to group into lexical units and create corresponding tokens, (2) check if the program is syntactically correct by parsing through the tokens generated (if not, emit an error message), and (3) generate the assembly code. By following the given grammar, the programs inputed should be syntactically correct and will display the source code and assembly language prodcued. If the program does not follow the grammar, it will print an error message and immediately stop execution.

### Commands to Compile/Run:
```
gcc hw4compiler.c -o out
./out input.txt
```

Output:
The output will be printed to the terminal screen. It will also be written to an output file called "output.txt". You can open the output.txt with Notepad app on the computer to see the formatting printed correctly. Also, there is another file called "elf.txt" that is being written to after the compiler determines a program is syntactically correct. It contains the assembly language (all represented by integers) which can then be the input file to HW1, the virtual machine.

Note:
There is a zip folder with all the error input files along with their corresponding output files. For example, "errorin1.txt" is the input file and "errorout1.txt" is the output file. When testing this code, the output file will always be called "output.txt" and should be overwritten when testing all the input files. However, it should match one of the output files in the zip folder. The only errors that are contained in this zip file are for procedure and call.
