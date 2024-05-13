### What is a Lexical Analyzer?
A lexical analyzer is the beginning stage of the complier, often referred to as the scanner. It will read characters from an input one at a time, ignoring control characters and white spaces, and group them into lexical units. These units translate each character into a stream of tokens to help the Parser, the next stage of the compiler. The token types have an internal representation in integers. Therefore, we are only taking an input of characters and reading them into the scanner one at a time to translate them into tokens.

### EBNF Grammar:
![image](https://github.com/kegrimes/System-Software/assets/117098682/416625b4-b799-4078-8970-8ca2c33ee4c4)

### Test Files:
There are 5 files that are used for testing: _"input.txt"_, _"input2.txt"_, _"input3.txt"_, _"input4.txt"_, and _"input5.txt"_

### Commands to Compile/Run:
```
gcc lex.c -o out
./out input.txt
```

### Output:
The results will be printed to the terminal screen. It will also be written to an output file called _"output.txt"_. You can open the textfile with Notepad or another similar application to see the formatting printed correctly.
