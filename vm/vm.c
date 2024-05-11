//Katelyn Grimes
//COP3402 - HW1
//September 15, 2023

#include <stdio.h>
#include <stdlib.h>
//Katelyn Grimes: constant variable given
#define ARRAY_SIZE 512

//Katelyn Grimes: Create PAS array - contains instructions and stack
int PAS[ARRAY_SIZE];

//Katelyn Grimes: Create AR array that keeps track of number of AR's and their size;
//used for printing each AR out after an instruction is called
int AR[ARRAY_SIZE];

//Katelyn Grimes: These are all the global variables that will keep track of index they are at in the PAS array
int PC = 0;
int SP = 0;
int BP = 0;

//Katelyn Grimes: These global variable will change as each instruction is getting read from input.txt
int IR_op = 0;
int IR_l = 0;
int IR_m =  0;

//Katelyn Grimes: number of elements in an AR
int ARcount = 0;

//Katelyn Grimes: debugging print out to see if the array is getting set correctly
void printArray(FILE *ofp){
    for(int i = 0; i < ARRAY_SIZE; i++){
        printf("%d ", PAS[i]);
        fprintf(ofp, "%d ", PAS[i]);
    }
}

//Katelyn Grimes: This function will read the input file into the PAS array
void loadInputFile(FILE *ifp, FILE *ofp){
    char buf[512];

    if(ifp == NULL){
        printf("Failed to open file!\n");
        fprintf(ofp, "Failed to open file!\n");
    }

    while(fgets(buf, ARRAY_SIZE, ifp) != NULL){
        int op;
        int l;
        int m;

        if(sscanf(buf, "%d %d %d\n", &op, &l, &m)){
            PAS[PC] = op;
            PAS[PC + 1] = l;
            PAS[PC + 2] = m;
            PC = PC + 3;
        }
    }
}

//Katelyn Grimes: Given function for the assignment - will help find a variable in a different AR some L levels down
int base(int BP, int L){
    int arb = BP;
    while(L > 0){
        arb = PAS[arb];
        L--;
    }
    return arb;
}

//Katelyn Grimes: This function will print the initial values of SP, BP, and PC after the file is read into the array
void printInitial(FILE *ofp){
    printf("\t\t PC\tBP\tSP\tstack\n");
    printf("Initial values:  %d\t%d\t%d\n\n", PC, BP, SP);

    //Katelyn Grimes: print to the output file
    fprintf(ofp, "\t\t PC\tBP\tSP\tstack\n");
    fprintf(ofp, "Initial values:  %d\t%d\t%d\n\n", PC, BP, SP);
}

//Katelyn Grimes: This function will print out the separate AR's after each instruction is called
void printAR(int initialBP, FILE *ofp){
    int tempBP = initialBP;
    for(int i = 0; i <= ARcount; i++){
        for(int j = 0; j < AR[i]; j++){
            printf("%d ", PAS[tempBP+j]);
            fprintf(ofp, "%d ", PAS[tempBP+j]);
        }
        tempBP = tempBP + AR[i];
        if(i < ARcount){
            printf("| ");
            fprintf(ofp, "| ");
        }
    }
    printf("\n");
    fprintf(ofp, "\n");
}

int main(int argc, char *argv[])
{
    int halt = 1;

    //Katelyn Grimes: Need to load the input file into PAS array.
    FILE *ifp = fopen(argv[1], "r");
    //Katelyn Grimes: creates an output file called "output.txt"
    //fprintf will write to the output file using the file pointer ofp
    FILE *ofp = fopen("output2.txt", "w");

    loadInputFile(ifp, ofp);

    //Katelyn Grimes: reset the PC variable and initialize others
    BP = PC;
    SP = BP - 1;
    PC = 0;
    //Katelyn Grimes: this variable will keep track of the first element in the stack
    int initialBP = BP;

    printInitial(ofp);

    while(halt == 1){
        //Katelyn Grimes: fetch operations
        IR_op = PAS[PC];
        IR_l = PAS[PC + 1];
        IR_m = PAS[PC + 2];
        PC = PC + 3;

        //Katelyn Grimes: this switch statement has a case for each instruction (execute)
        switch(IR_op){
            //Katelyn Grimes: LIT
            case 1:
                SP = SP + 1;
                PAS[SP] = IR_m;
                printf("  LIT %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                fprintf(ofp, "  LIT %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                //Katelyn Grimes: line 136 is inside every instruction because it will constantly change to keep track of the number of elements in one AR and store it into the AR array. EX: AR[0] = 6 means there are 6 elements in the first AR
                AR[ARcount] = (SP-BP)+1;
                //Katelyn Grimes: function will print out the AR's after each instruction
                printAR(initialBP, ofp);
                break;
            //Katelyn Grimes: OPR
            case 2:
                //Katelyn Grimes: RTN
                if(IR_m == 0){
                    SP = BP - 1;
                    BP = PAS[SP + 2];
                    PC = PAS[SP + 3];
                    printf("  RTN %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    fprintf(ofp, "  RTN %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    //Katelyn Grimes: set AR[current index] to 0 to delete the AR
                    AR[ARcount] = 0;
                    //Katelyn Grimes: decrement the index to show we deleted an AR at that position
                    ARcount--;
                    printAR(initialBP, ofp);
                }
                //Katelyn Grimes: ADD
                if(IR_m == 1){
                    PAS[SP - 1] = PAS[SP - 1] + PAS[SP];
                    SP = SP - 1;
                    printf("  ADD %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    fprintf(ofp, "  ADD %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    AR[ARcount] = (SP-BP)+1;
                    printAR(initialBP, ofp);
                }
                //Katelyn Grimes: SUB
                if(IR_m == 2){
                    PAS[SP - 1] = PAS[SP - 1] - PAS[SP];
                    SP = SP - 1;
                    printf("  SUB %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    fprintf(ofp, "  SUB %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    AR[ARcount] = (SP-BP)+1;
                    printAR(initialBP, ofp);
                }
                //Katelyn Grimes: MUL
                if(IR_m == 3){
                    PAS[SP - 1] = PAS[SP - 1] * PAS[SP];
                    SP = SP - 1;
                    printf("  MUL %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    fprintf(ofp, "  MUL %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    AR[ARcount] = (SP-BP)+1;
                    printAR(initialBP, ofp);
                }
                //Katelyn Grimes: DIV
                if(IR_m == 4){
                    PAS[SP - 1] = PAS[SP - 1] / PAS[SP];
                    SP = SP - 1;
                    printf("  DIV %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    fprintf(ofp, "  DIV %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    AR[ARcount] = (SP-BP)+1;
                    printAR(initialBP, ofp);
                }
                //Katelyn Grimes: EQL
                if(IR_m == 5){
                    PAS[SP - 1] = PAS[SP - 1] == PAS[SP];
                    SP = SP - 1;
                    printf("  EQL %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    fprintf(ofp, "  EQL %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    AR[ARcount] = (SP-BP)+1;
                    printAR(initialBP, ofp);
                }
                //Katelyn Grimes: NEQ
                if(IR_m == 6){
                    PAS[SP - 1] = PAS[SP - 1] != PAS[SP];
                    SP = SP - 1;
                    printf("  NEQ %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    fprintf(ofp, "  NEQ %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    AR[ARcount] = (SP-BP)+1;
                    printAR(initialBP, ofp);
                }
                //Katelyn Grimes: LSS
                if(IR_m == 7){
                    PAS[SP - 1] = PAS[SP - 1] < PAS[SP];
                    SP = SP - 1;
                    printf("  LSS %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    fprintf(ofp, "  LSS %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    AR[ARcount] = (SP-BP)+1;
                    printAR(initialBP, ofp);
                }
                //Katelyn Grimes: LEQ
                if(IR_m == 8){
                    PAS[SP - 1] = PAS[SP - 1] <= PAS[SP];
                    SP = SP - 1;
                    printf("  LEQ %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    fprintf(ofp, "  LEQ %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    AR[ARcount] = (SP-BP)+1;
                    printAR(initialBP, ofp);
                }
                //Katelyn Grimes: GTR
                if(IR_m == 9){
                    PAS[SP - 1] = PAS[SP - 1] > PAS[SP];
                    SP = SP - 1;
                    printf("  GTR %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    fprintf(ofp, "  GTR %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    AR[ARcount] = (SP-BP)+1;
                    printAR(initialBP, ofp);
                }
                //Katelyn Grimes: GEQ
                if(IR_m == 10){
                    PAS[SP - 1] = PAS[SP - 1] >= PAS[SP];
                    SP = SP - 1;
                    printf("  GEQ %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    fprintf(ofp, "  GEQ %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    AR[ARcount] = (SP-BP)+1;
                    printAR(initialBP, ofp);
                }
                break;
            //Katelyn Grimes: LOD
            case 3:
                SP = SP + 1;
                PAS[SP] = PAS[base(BP,IR_l) + IR_m];
                printf("  LOD %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                fprintf(ofp, "  LOD %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                AR[ARcount] = (SP-BP)+1;
                printAR(initialBP, ofp);
                break;
            //Katelyn Grimes: STO
            case 4:
                PAS[base(BP,IR_l) + IR_m] = PAS[SP];
                SP = SP - 1;
                printf("  STO %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                fprintf(ofp, "  STO %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                AR[ARcount] = (SP-BP)+1;
                printAR(initialBP, ofp);
                break;
            //Katelyn Grimes: CAL
            case 5:
                PAS[SP + 1] = base(BP, IR_l);
                PAS[SP + 2] = BP;
                PAS[SP + 3] = PC;
                BP = SP + 1;
                PC = IR_m;
                printf("  CAL %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                fprintf(ofp, "  CAL %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                printAR(initialBP, ofp);
                //Katelyn Grimes: increment the index to show we created a new AR
                ARcount++;
                AR[ARcount] = (SP-BP) + 1;
                break;
            //Katelyn Grimes: INC
            case 6:
                SP = SP + IR_m;
                printf("  INC %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                fprintf(ofp, "  INC %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                AR[ARcount] = (SP-BP)+1;
                printAR(initialBP, ofp);
                break;
            //Katelyn Grimes: JMP
            case 7:
                PC = IR_m;
                printf("  JMP %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                fprintf(ofp, "  JMP %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                printAR(initialBP, ofp);
                break;
            //Katelyn Grimes: JPC
            case 8:
                if(PAS[SP] == 0)
                    PC = IR_m;
                SP = SP - 1;
                printf("  JPC %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                fprintf(ofp, "  JPC %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                AR[ARcount] = (SP-BP)+1;
                printAR(initialBP, ofp);
                break;
            //Katelyn Grimes: SYS
            case 9:
                if(IR_m == 1){
                    //Katelyn Grimes: output the result
                    printf("Output result is: %d\n", PAS[SP]);
                    fprintf(ofp, "Output result is: %d\n", PAS[SP]);
                    SP = SP - 1;
                    printf("  SYS %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    fprintf(ofp, "  SYS %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    AR[ARcount] = (SP-BP)+1;
                    printAR(initialBP, ofp);
                }
                if(IR_m == 2){
                    SP = SP + 1;
                    //Katelyn Grimes: get user input
                    printf("Please Enter an Integer: ");
                    scanf("%d", &PAS[SP]);
                    fprintf(ofp, "Please Enter an Integer: %d\n", PAS[SP]);
                    printf("  SYS %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    fprintf(ofp, "  SYS %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    AR[ARcount] = (SP-BP)+1;
                    printAR(initialBP, ofp);
                }
                if(IR_m == 3){
                    //Katelyn Grimes: change halt to stop switch cases (instructions are done)
                    halt = 0;
                    printf("  SYS %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    fprintf(ofp, "  SYS %d   %d\t %d\t%d\t%d\t", IR_l, IR_m, PC, BP, SP);
                    AR[ARcount] = (SP-BP)+1;
                    printAR(initialBP, ofp);
                }
                break;
            default:
                printf("invalid instruction\n");
                fprintf(ofp, "invalid instruction\n");
        }
    }

    fclose(ifp);
    fclose(ofp);

    return 0;
}
