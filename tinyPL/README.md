### What is a Tiny PL/0 machince?
In the lexical analyzer, characters in an input program were read and grouped into lexical units to create tokens, an internal representation of a lexeme, the sequence of characters. There was no check to see if the program inputed was syntactically correct. However, in the tiny PL/0 machine, the tokens are parsed through to (1) check the syntax and (2) generate assembly code. If there is something incorrect about the program, then an error will be printed and the code generation will terminate immediately. But, if there are no errors detected, then the program will print out the generated code and the symbol table. The symbol table will contain the kind, name, value (if there is one), lexigraphical level, address, and mark, which indiciates if the symbol is still being used in the program. 

### EBNF Grammar:

### Test Files:
There is a folder called _"testfiles"_ that contains input testing files along with their corresponding output files. For example, _"errorin1.txt"_ is the input file and _"errorout1.txt"_ is the output file. When testing this code, the output file will always be called _"output.txt"_ and should be overwritten when testing all the input files. However, it should match one of the correct output files in the folder.

### Commands to Compile/Run:
```
gcc parsercodegen.c -o out
./out input.txt
```

### Output:
The results will be printed to the terminal screen. It will also be written to an output file called _"output.txt"_. You can open the textfile with Notepad or another similar application to see the formatting printed correctly.
