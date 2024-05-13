Katelyn Grimes
COP3402 - Homework Assignment 2
Septemeber 29, 2023

What is a Lexical Analyzer?
A lexical analyzer is the beginning stage of the complier, often referred to as the scanner. It will read characters from an input one at a time, ignoring control characters and white spaces, and group them into lexical units. These units translate each character into a stream of tokens to help the Parser, the next stage of the compiler. The token types have an internal representation in integers. Therefore, we are only taking an input of characters and reading them into the scanner one at a time to translate them into tokens.

Required Software: GCC Compiler
For ucf students, download Cisco AnyConnect software to access the university's VPN.
Then use Eustis from downloading MobaXTerm to run the program
	- Cisco: https://secure.vpn.ucf.edu - sign in with NID and password and you will be logged into the Eustis server
	- MobaXTerm: create a new SSH session with remote host as "eustis.eecs.ucf.edu", specify username with NID, and port 22

Compling the Program:
1. In MobaXTerm, open the Eustis server (you will have to login with your NID and password)
2. Locate the correct path to source code, vm.c, and use the command "cd ______" to get into the correct directory (the blank represents the name of the file where the source code is contained)
3. To complie the code, use "gcc vm.c". This will also create a "a.out" file which will be used to run the code along with the input.txt file

Run the Program:
1. Run the source code using "./a.out input.txt" and the code should run successfully in the terminal.

Output:
The output will be printed to the terminal screen. It will also be written to an output file called output.txt. You can open the output.txt with Notepad app on the computer to see the formatting printed correctly.
