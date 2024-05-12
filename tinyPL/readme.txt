Katelyn Grimes
COP3402 - Homework Assignment 3
Novemeber 3, 2023

What is a Tiny PL/0 machince?
In the lexical analyzer, characters in an input program were read and grouped into lexical units to create tokens, an internal representation of a lexeme, the sequence of characters. There was no check to see if the program inputed was syntactically correct. However, in the tiny PL/0 machine, the tokens are parsed through to (1) check the syntax and (2) generate assembly code. If there is something incorrect about the program, then an error will be printed and the code generation will terminate immediately. But, if there are no errors detected, then the program will print out the generated code and the symbol table. The symbol table will contain the kind, name, value (if there is one), lexigraphical level, address, and mark, which indiciates if the symbol is still being used in the program. 

Required Software: GCC Complier
For ucf students, download Cisco AnyConnect software to access the university's VPN.
Then use Eustis from downloading MobaXTerm to run the program
	- Cisco: https://secure.vpn.ucf.edu - sign in with NID and password and you will be logged into the Eustis server
	- MobaXTerm: create a new SSH session with remote host as "eustis.eecs.ucf.edu", specify username with NID, and port 22

Compling the Program:
1. In MobaXTerm, open the Eustis server (you will have to login with your NID and password)
2. Locate the correct path to source code, vm.c, and use the command "cd ______" to get into the correct directory (the blank represents the name of the file where the source code is contained)
3. To complie the code, use "gcc parsercodegen.c". This will also create a "a.out" file which will be used to run the code along with the input.txt file

Run the Program:
1. Run the source code using "./a.out input.txt" and the code should run successfully in the terminal.

Output:
The output will be printed to the terminal screen. It will also be written to an output file called output.txt. You can open the output.txt with Notepad app on the computer to see the formatting printed correctly.

Note:
There is a zip folder with all the error input files along with their corresponding output files. For example, "errorin1.txt" is the input file and "errorout1.txt" is the output file. When testing this code, the output file will always be called "output.txt" and should be overwritten when testing all the input files. However, it should match one of the output files in the zip folder.