Katelyn Grimes
COP3402 - Homework Assignment 1
September 15, 2023

What is a VM?
A virtual machine, or VM, is a machine that implements an ISA, otherwise known as instruction set architecture. The source code I have written (vm.c) is a virtual machine that represents how an ISA operates in the CPU with a given input of instructions. By using the P-machine, PAS, one area of memory contains the set of instructions from input and the other area contains the stack. After each instruction is called, the source code will print it out in the terminal. Therefore, we can see how the virtual machine works when each step is executed in the stack.

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
An output file called "output.txt" should be created after successfully running the program. Open the file with the Notepad app on your computer, so it will display the formatting correctly. You should see what was printed to the terminal screen on the Notepad app.

