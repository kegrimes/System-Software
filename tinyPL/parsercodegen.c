//Katelyn Grimes
//COP3402: Fall 2023
//HW3: Tiny PL/0
//November 3, 2023

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//Lexical Analyzer:
//------------------------------------------------------------------------------------

//Katelyn Grimes: defined variables for max sizes
#define MAXchar 12
#define MAXdig 6
#define reserveMAX 11
#define symbolMAX 13
#define MAXSIZE 1000

//Katelyn Grimes: declaration of token types - deleted call, procedure, else, and ifel, but kept token values the same
typedef enum{
    oddsym=1, identsym=2, numbersym=3, plussym=4, minussym=5, multsym=6, slashsym=7,
    eqlsym=9, neqsym=10, lessym=11, leqsym=12, gtrsym=13, geqsym=14,
    lparentsym=15, rparentsym=16, commasym=17, semicolonsym=18, periodsym=19,
    becomesym=20, beginsym=21, endsym=22, ifsym=23, thensym=24, whilesym=25,
    dosym=26, constsym=28, varsym=29, writesym=31, readsym=32
} token_type;

//Katelyn Grimes: not sure what is the max
char inputArray[MAXSIZE];
//Katelyn Grimes: gets the total number of elements in main array after reading input file
int arrayCount = 0;

//Katelyn Grimes: lexeme array with lexSize as the number of elements in the array
char lexUnits[MAXSIZE][MAXchar] = {};
int lexSize = 0;
//Katelyn Grimes: token type array
int tokenType[MAXSIZE];
//Katelyn Grimes: token list with tokenCount as the number of elements in the array
char tokenList[MAXSIZE][MAXchar] = {};
int tokenCount = 0;
//Katelyn Grimes: ID table used later for token representation - assumed to have no more than 100 id's in a program
char idNames[100][MAXchar] = {};
int idCount = 0;

//Katelyn Grimes: list of names and symbols with corresponding internal representation arrays - altered for grammar in tiny PL/0
char *reservedWords[] = {"odd", "begin", "end", "if", "then", "while", "do", "const", "var", "write", "read"};
int nameInternalRep[] = {oddsym, beginsym, endsym, ifsym, thensym, whilesym, dosym, constsym, varsym, writesym, readsym};
//Katelyn Grimes: list of valid special symbols
char specialSym[] = {'+', '-', '*', '/', '=', '<', '>', '(', ')', ',', ';', '.', ':'};

//------------------------------------------------------------------------------------

//Tiny PL/0:
//------------------------------------------------------------------------------------

//Katelyn Grimes: defined variables for max sizes
#define MAX_SYMBOL_TABLE_SIZE 500
#define CODE_SIZE 500

//Katelyn Grimes: declaration of a symbol
typedef struct{
    int kind;
    char name[12]; //can't be more than 11 characters - room for extra
    char val[6]; //can't be more than 5 digits - room for extra
    int level;
    int addr;
    int mark;
}symbol;

//Katelyn Grimes: declaration of a single line of assembly code
typedef struct{
    int op;
    int L;
    int M;
}code;

//Katelyn Grimes: symbol table with it's index count to add symbols
symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];
int symbolCount = 0;

//Katelyn Grimes: code generation array with it's index
code text[CODE_SIZE];
int cx = 0;

//Katelyn Grimes: count for parsing through token list and the current token
int tCount = -1;
int token = 0;

//Katelyn Grimes: List of errors that will be printed to the screen if input program is syntactically incorrect
void ERROR(FILE *ofp, int type){
    switch(type){
        case 1:
            printf("Error: program must end with period\n");
            fprintf(ofp, "Error: program must end with period\n");
            exit(0);
            break;
        case 2:
            printf("Error: const, var, and read keywords must be followed by identifier\n");
            fprintf(ofp, "Error: const, var, and read keywords must be followed by identifier\n");
            exit(0);
            break;
        case 3:
            printf("Error: symbol name has already been declared\n");
            fprintf(ofp, "Error: symbol name has already been declared\n");
            exit(0);
            break;
        case 4:
            printf("Error: constants must be assigned with =\n");
            fprintf(ofp, "Error: constants must be assigned with =\n");
            exit(0);
            break;
        case 5:
            printf("Error: constants must be assigned an integer value\n");
            fprintf(ofp, "Error: constants must be assigned an integer value\n");
            exit(0);
            break;
        case 6:
            printf("Error: constant and variable declarations must be followed by a semicolon\n");
            fprintf(ofp, "Error: constant and variable declarations must be followed by a semicolon\n");
            exit(0);
            break;
        case 7:
            printf("Error: undeclared identifier ");
            fprintf(ofp, "Error: undeclared identifier ");
            break;
        case 8:
            printf("Error: only variable values may be altered\n");
            fprintf(ofp, "Error: only variable values may be altered\n");
            exit(0);
            break;
        case 9:
            printf("Error: assignment statements must use :=\n");
            fprintf(ofp, "Error: assignment statements must use :=\n");
            exit(0);
            break;
        case 10:
            printf("Error: begin must be followed by end\n");
            fprintf(ofp, "Error: begin must be followed by end\n");
            exit(0);
            break;
        case 11:
            printf("Error: if must be followed by then\n");
            fprintf(ofp, "Error: if must be followed by then\n");
            exit(0);
            break;
        case 12:
            printf("Error: while must be followed by do\n");
            fprintf(ofp, "Error: while must be followed by do\n");
            exit(0);
            break;
        case 13:
            printf("Error: condition must contain comparison operator\n");
            fprintf(ofp, "Error: condition must contain comparison operator\n");
            exit(0);
            break;
        case 14:
            printf("Error: right parenthesis must follow left parenthesis\n");
            fprintf(ofp, "Error: right parenthesis must follow left parenthesis\n");
            exit(0);
            break;
        case 15:
            printf("Error: arithmetic equations must contain operands, parentheses, numbers, or symbols\n");
            fprintf(ofp, "Error: arithmetic equations must contain operands, parentheses, numbers, or symbols\n");
            exit(0);
            break;
        case 16:
            //Katelyn Grimes: not an error for the syntax of a program, but checking if code generation is out of bounds
            printf("Error: code generation out of bounds\n");
            fprintf(ofp, "Error: code generation out of bounds\n");
            exit(0);
            break;
        case 17:
            printf("Error: identifier name too long (");
            fprintf(ofp, "Error: identifier name too long (");
            break;
        case 18:
            printf("Error: number too long (");
            fprintf(ofp, "Error: number too long (");
            break;
        case 19:
            printf("Error: invalid symbol ");
            fprintf(ofp, "Error: invalid symbol ");
        default:
            break;
    }
}

//Katelyn Grimes: gets the name of the assembly code being generated
void codeGen(FILE *ofp, int num){
    switch(num){
        case 1:
            printf("LIT");
            fprintf(ofp, "LIT");
            break;
        case 2:
            printf("OPR");
            fprintf(ofp, "OPR");
            break;
        case 3:
            printf("LOD");
            fprintf(ofp, "LOD");
            break;
        case 4:
            printf("STO");
            fprintf(ofp, "STO");
            break;
        //Katelyn Grimes: there is no "CAL" so 5 was skipped
        case 6:
            printf("INC");
            fprintf(ofp, "INC");
            break;
        case 7:
            printf("JMP");
            fprintf(ofp, "JMP");
            break;
        case 8:
            printf("JPC");
            fprintf(ofp, "JPC");
            break;
        case 9:
            //Katelyn Grimes: will be used for halt, read, and write
            printf("SYS");
            fprintf(ofp, "SYS");
            break;
        default:
            break;
    }
}

//------------------------------------------------------------------------------------

//Katelyn Grimes: Read input file and stores into main array
void readInput(FILE *ifp){
    char ch;
    int i = 0;

    if(ifp == NULL){
        printf("Failed to open file!\n");
    }

    while((ch = fgetc(ifp)) != EOF){
        inputArray[i] = ch;
        arrayCount++;
        i++;
    }
}

//Katelyn Grimes: Print main array out to terminal screen
void printArray(FILE *ofp){
    for(int i = 0; i < arrayCount; i++){
        printf("%c", inputArray[i]);
        fprintf(ofp, "%c", inputArray[i]);
    }
}

//Katelyn Grimes: Stores the id names into separate array
void printIds(FILE *ofp){
    printf("  ID List:\n");
    fprintf(ofp, "  ID List:\n");
    printf("-------------\n");
    fprintf(ofp, "-------------\n");
    for(int i = 0; i < 100; i++){
        if(idNames[i][0] != 0){
            printf("%d. ", i+1);
            fprintf(ofp, "%d. ", i+1);
            for(int j = 0; j < MAXchar; j++){
                printf("%c", idNames[i][j]);
                fprintf(ofp, "%c", idNames[i][j]);
            }
            printf("\n");
            fprintf(ofp, "\n");
        }
    }
}

//Katelyn Grimes: Prints each line as the lexeme and token types are being created
void printTableLine(int l, FILE *ofp){
    //Katelyn Grimes: words with 8+ characters does not align correctly, so this helps
    int x = 0;
    for(int i = 0; lexUnits[l][i] != 0; i++){
        printf("%c", lexUnits[l][i]);
        fprintf(ofp, "%c", lexUnits[l][i]);
        x++;
    }
    if(x >= 8){
      printf("\t\t%d\n", tokenType[l]);
      fprintf(ofp, "\t\t%d\n", tokenType[l]);
    }
    else{
      printf("\t\t\t%d\n", tokenType[l]);
      fprintf(ofp, "\t\t\t%d\n", tokenType[l]);
    }
}

//Katelyn Grimes: Checks if a given symbol is a valid symbol
int issymbol(char ch){
    for(int i = 0; i < symbolMAX; i++){
        if(ch == specialSym[i])
            return 1;
    }
    return 0;
}

//Katelyn Grimes: Makes sure id's are not being duplicated
int checkID(char word[]){
    for(int i = 0; i < 100; i++){
        if(strcmp(word, idNames[i]) == 0){
            return 1;
        }
    }
    return 0;
}

//Katelyn Grimes: Checks if a word is a reserved word or identifier - commented out printTableLine
int findWordType(char word[], int l, int length, FILE *ofp){
    //Katelyn Grimes: first check if the word is part of reserved word list
    for(int i = 0; i < reserveMAX; i++){
        if(strcmp(word, reservedWords[i]) == 0){
            for(int size = 0; size < length; size++){
                lexUnits[l][size] = word[size];
            }
            tokenType[l] = nameInternalRep[i];
            //printTableLine(l, ofp);
            l++;
            return l;
        }
    }

    //Katelyn Grimes: if not returned yet, the word is an identifier
    //Katelyn Grimes: check if the id is already in the id list
    if(checkID(word) == 1){
        for(int i = 0; word[i] != 0; i++){
            lexUnits[l][i] = word[i];
        }
        tokenType[l] = identsym;
        //printTableLine(l, ofp);
        l++;
    }
    else{
        for(int i = 0; word[i] != 0; i++){
            idNames[idCount][i] = word[i];
            lexUnits[l][i] = word[i];
        }
        tokenType[l] = identsym;
        //printTableLine(l, ofp);
        l++;
        idCount++;
    }

    return l;
}

//Katelyn Grimes: Groups characters into lexical units and stores into arrays - commented out printTableLine
void createLexUnits(FILE *ofp){
    int error = 0;

    //Katelyn Grimes: main array current index
    int i = 0;
    //Katelyn Grimes: lexUnits array current index
    int l = 0;

    //Katelyn Grimes: temp arrays store a word/number with one or more characters - each assumed to not go over 100 characters
    char temp[100] = {};
    char temp2[100] = {};

    do{
        if(isspace(inputArray[i])){
            i++;
        }
        else if(iscntrl(inputArray[i])){
            i++;
        }
        //Katelyn Grimes: check if character is a letter
        else if(isalpha(inputArray[i])){
            //Katelyn Grimes: Multiple letters/digits in a word
            if(isalpha(inputArray[i+1]) || isdigit(inputArray[i+1])){
                int length = 0;
                while(isalpha(inputArray[i]) || isdigit(inputArray[i])){
                    //Katelyn Grimes: Error if greater than 11 characters
                    if(length > MAXchar - 2){
                        error = 1;
                    }
                    temp[length] = inputArray[i];
                    length++;
                    i++;
                }
                if(error == 1){
                    ERROR(ofp, 17);
                    for(int h = 0; temp[h] != 0; h++){
                        printf("%c", temp[h]);
                        fprintf(ofp, "%c", temp[h]);
                    }
                    printf(")\n");
                    fprintf(ofp, ")\n");
                    exit(0);
                }
                else{
                    l = findWordType(temp, l, length, ofp);
                }
                //Katelyn Grimes: Reset temp array
                memset(temp, '\0', sizeof temp);
            }
            //Katelyn Grimes: Only one letter in the word - means it's an identifier
            else{
                temp[0] = inputArray[i];
                if(checkID(temp) == 1){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = identsym;
                    //printTableLine(l, ofp);
                    l++;
                    i++;
                }
                else{
                    idNames[idCount][0] = inputArray[i];
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = identsym;
                    //printTableLine(l, ofp);
                    idCount++;
                    l++;
                    i++;
                }
                memset(temp, '\0', sizeof temp);
            }
            error = 0;
        }
        //Katelyn Grimes: Check if character is a digit
        else if(isdigit(inputArray[i])){
            //Katelyn Grimes: Multiple digits
            if(isdigit(inputArray[i+1])){
                int length = 0;
                while(isdigit(inputArray[i])){
                    //Katelyn Grimes: Error if greater than 5 digits
                    if(length > MAXdig - 2){
                        error = 1;
                    }
                    temp2[length] = inputArray[i];
                    length++;
                    i++;
                }
                if(error == 1){
                    ERROR(ofp, 18);
                    for(int h = 0; temp2[h] != 0; h++){
                        printf("%c", temp2[h]);
                        fprintf(ofp, "%c", temp2[h]);
                    }
                    printf(")\n");
                    fprintf(ofp, ")\n");
                    exit(0);
                }
                else{
                    for(int size = 0; size < length; size++){
                        lexUnits[l][size] = temp2[size];
                    }
                    tokenType[l] = numbersym;
                    //printTableLine(l, ofp);
                    l++;
                }
                //Katelyn Grimes: reset temp array
                memset(temp2, '\0', sizeof temp2);
            }
            //Katelyn Grimes: Only one digit
            else{
                lexUnits[l][0] = inputArray[i];
                tokenType[l] = numbersym;
                //printTableLine(l, ofp);
                l++;
                i++;
            }
            error = 0;
        }
        //Katelyn Grimes: Check if character is a special symbol
        else if(issymbol(inputArray[i])){
            //Katelyn Grimes: comment case
            if(inputArray[i] == '/'){
                if(inputArray[i+1] == '*'){
                    i = i + 2;
                    while(inputArray[i] != '*'){
                        i++;
                    }
                    i = i + 2;
                }
                //Katelyn Grimes: slash case
                else{
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = slashsym;
                    //printTableLine(l, ofp);
                    l++;
                    i++;
                }
            }
            else{
                //Katelyn Grimes: becomes case
                if(inputArray[i] == ':'){
                    if(inputArray[i+1] == '='){
                        char temp[] = {':', '='};
                        for(int size = 0; size < 2; size++){
                            lexUnits[l][size] = temp[size];
                        }
                        tokenType[l] = becomesym;
                        //printTableLine(l, ofp);
                        l++;
                        i = i + 2;
                    }
                }
                //Katelyn Grimes: not equal to case
                if(inputArray[i] == '<'){
                    if(inputArray[i+1] == '>'){
                        char temp[] = {'<', '>'};
                        for(int size = 0; size < 2; size++){
                            lexUnits[l][size] = temp[size];
                        }
                        tokenType[l] = neqsym;
                        //printTableLine(l, ofp);
                        l++;
                        i = i + 2;
                    }
                    //Katelyn Grimes: less than or equal to case
                    else if(inputArray[i+1] == '='){
                        char temp[] = {'<', '='};
                        for(int size = 0; size < 2; size++){
                            lexUnits[l][size] = temp[size];
                        }
                        tokenType[l] = leqsym;
                        //printTableLine(l, ofp);
                        l++;
                        i = i + 2;
                    }
                    //Katelyn Grimes: less than case
                    else{
                        lexUnits[l][0] = inputArray[i];
                        tokenType[l] = lessym;
                        //printTableLine(l, ofp);
                        l++;
                        i++;
                    }
                }
                //Katelyn Grimes: greater than or equal case
                if(inputArray[i] == '>'){
                    if(inputArray[i+1] == '='){
                        char temp[] = {'>', '='};
                        for(int size = 0; size < 2; size++){
                            lexUnits[l][size] = temp[size];
                        }
                        tokenType[l] = geqsym;
                        //printTableLine(l, ofp);
                        l++;
                        i = i + 2;
                    }
                    //Katelyn Grimes: greater than case
                    else{
                        lexUnits[l][0] = inputArray[i];
                        tokenType[l] = gtrsym;
                        //printTableLine(l, ofp);
                        l++;
                        i++;
                    }
                }
                //Katelyn Grimes: plus case
                if(inputArray[i] == '+'){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = plussym;
                    //printTableLine(l, ofp);
                    l++;
                    i++;
                }
                //Katelyn Grimes: minus case
                if(inputArray[i] == '-'){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = minussym;
                    //printTableLine(l, ofp);
                    l++;
                    i++;
                }
                //Katelyn Grimes: multiply case
                if(inputArray[i] == '*'){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = multsym;
                    //printTableLine(l, ofp);
                    l++;
                    i++;
                }
                //Katelyn Grimes: left parenthesis case
                if(inputArray[i] == '('){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = lparentsym;
                    //printTableLine(l, ofp);
                    l++;
                    i++;
                }
                //Katelyn Grimes: right parenthesis case
                if(inputArray[i] == ')'){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = rparentsym;
                    //printTableLine(l, ofp);
                    l++;
                    i++;
                }
                //Katelyn Grimes: equal case
                if(inputArray[i] == '='){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = eqlsym;
                    //printTableLine(l, ofp);
                    l++;
                    i++;
                }
                //Katelyn Grimes: comma case
                if(inputArray[i] == ','){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = commasym;
                    //printTableLine(l, ofp);
                    l++;
                    i++;
                }
                //Katelyn Grimes: semicolon case
                if(inputArray[i] == ';'){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = semicolonsym;
                    //printTableLine(l, ofp);
                    l++;
                    i++;
                }
                //Katelyn Grimes: period case
                if(inputArray[i] == '.'){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = periodsym;
                    //printTableLine(l, ofp);
                    l++;
                    i++;
                }
            }
        }
        //Katelyn Grimes: Error - not valid input
        else{
            ERROR(ofp, 19);
            printf("%c", inputArray[i]);
            fprintf(ofp, "%c", inputArray[i]);
            printf("\n");
            fprintf(ofp, "\n");
            exit(0);
            i++;
        }

    }while(inputArray[i] != '\0');

    lexSize = l;
}

//Katelyn Grimes: Creates the token list including printing integers and identifiers
void createTokenList(){
    //Katelyn Grimes: temp array to convert integers into char array to store
    char temp[2] = {};
    int index = 0;
    char num;
    for(int i = 0; tokenType[i] != 0; i++){
        if(tokenType[i] <= 9){
            num = tokenType[i] + '0';
            tokenList[index][0] = num;
            tokenCount++;
        }
        else{
            sprintf(temp, "%d", tokenType[i]);
            for(int k = 0; k < 2; k++){
                tokenList[index][k] = temp[k];
                tokenCount++;
            }
        }

        if(tokenType[i] == 2 || tokenType[i] == 3){
            index++;
            for(int j = 0; lexUnits[i][j] != 0; j++){
               tokenList[index][j] = lexUnits[i][j];
               tokenCount++;
            }
        }
        index++;
    }
}

//Katelyn Grimes: Print the token list
void printTokenList(FILE *ofp){
    for(int i = 0; i < tokenCount; i++){
        for(int j = 0; tokenList[i][j] != 0; j++){
            printf("%c", tokenList[i][j]);
            fprintf(ofp, "%c", tokenList[i][j]);
        }
        printf(" ");
        fprintf(ofp, " ");
    }
}

//Katelyn Grimes: Convert integer values held in an array into a single integer
int convert(char val[]){
    int newVal;
    sscanf(val, "%d", &newVal);

    return newVal;
}

//Katelyn Grimes: Gets the next token for code generation
int getToken(){
    tCount++;
    int token = convert(tokenList[tCount]);

    return token;
}

//Katelyn Grimes: Add symbol to the symbol table with corresponding attributes
void addSymboltable(int kind, char name[], char val[], int L, int M, int mark){
    symbolTable[symbolCount].kind = kind;
    for(int i = 0; name[i] != 0; i++){
        symbolTable[symbolCount].name[i] = name[i];
    }
    if(val[0] == 0){
        symbolTable[symbolCount].val[0] = '0';
    }
    else{
        for(int j = 0; val[j] != 0; j++){
            symbolTable[symbolCount].val[j] = val[j];
        }
    }
    symbolTable[symbolCount].level = L;
    symbolTable[symbolCount].addr = M;
    symbolTable[symbolCount].mark = mark;
    symbolCount++;
}

//Katelyn Grimes: Linear search of the symbol table to find if the symbol exists and will return the index
int symbolTableCheck(char name[]){
    for(int i = 0; i < MAX_SYMBOL_TABLE_SIZE; i++){
        if(strcmp(name, symbolTable[i].name) == 0){
            return i;
        }
    }
    return -1;
}

//Katelyn Grimes: Will emit code generated into text array
void emit(FILE *ofp, int op, int L, int M){
    if(cx > CODE_SIZE){
        ERROR(ofp, 16);
    }
    else{
        text[cx].op = op;
        text[cx].L = L;
        text[cx].M = M;
        cx++;
    }
}

//Katelyn Grimes: Expression - part of the grammar
void expression(FILE *ofp){
    term(ofp);
    while(token == plussym || token == minussym){
        if(token == plussym){
            token = getToken();
            term(ofp);
            //ADD
            emit(ofp, 2, 0, 1);
        }
        else{
            token = getToken();
            term(ofp);
            //SUB
            emit(ofp, 2, 0, 2);
        }
    }
}

//Katelyn Grimes: Factor - part of the grammar
void factor(FILE *ofp){
    if(token == identsym){
        //Katelyn Grimes: get name of identifier
        char name[MAXchar] = {};
        for(int i = 0; tokenList[tCount + 1][i] != 0; i++){
            name[i] = tokenList[tCount + 1][i];
        }
        int symIndex = symbolTableCheck(name);
        if(symIndex == -1){
            ERROR(ofp, 7);
            //Katelyn Grimes: prints the identifier name that is not declared
            for(int i = 0; name[i] != 0; i++){
                printf("%c", name[i]);
                fprintf(ofp, "%c", name[i]);
            }
            printf("\n");
            fprintf(ofp, "\n");
            exit(0);
        }
        if(symbolTable[symIndex].kind == 1){
            //LIT
            int val = convert(symbolTable[symIndex].val);
            emit(ofp, 1, 0, val);
        }
        else{
            //LOD
            emit(ofp, 3, 0, symbolTable[symIndex].addr);
        }
        tCount++;
        token = getToken();
    }
    else if(token == numbersym){
        //Katelyn Grimes: get value of identifier
        char num[MAXdig] = {};
        for(int i = 0; tokenList[tCount + 1][i] != 0; i++){
            num[i] = tokenList[tCount + 1][i];
        }
        //LIT
        int val = convert(num);
        emit(ofp, 1, 0, val);
        tCount++;
        token = getToken();
    }
    else if(token == lparentsym){
        token = getToken();
        expression(ofp);
        if(token != rparentsym){
            ERROR(ofp, 14);
        }
        token = getToken();
    }
    else{
        ERROR(ofp, 15);
    }
}

//Katelyn Grimes: Term - part of grammar
void term(FILE *ofp){
    factor(ofp);
    while(token == multsym || token == slashsym){
        if(token == multsym){
            token = getToken();
            factor(ofp);
            //MUL
            emit(ofp, 2, 0, 3);
        }
        if(token == slashsym){
            token = getToken();
            factor(ofp);
            //DIV
            emit(ofp, 2, 0, 4);
        }
    }
}

//Katelyn Grimes: Condition - part of grammar
void condition(FILE *ofp){
    if(token == oddsym){
        token = getToken();
        expression(ofp);
        //ODD
        emit(ofp, 2, 0, 11);
    }
    else{
        expression(ofp);
        if(token == eqlsym){
            token = getToken();
            expression(ofp);
            //EQL
            emit(ofp, 2, 0, 5);
        }
        else if(token == neqsym){
            token = getToken();
            expression(ofp);
            //NEQ
            emit(ofp, 2, 0, 6);
        }
        else if(token == lessym){
            token = getToken();
            expression(ofp);
            //LSS
            emit(ofp, 2, 0, 7);
        }
        else if(token == leqsym){
            token = getToken();
            expression(ofp);
            //LEQ
            emit(ofp, 2, 0, 8);
        }
        else if(token == gtrsym){
            token = getToken();
            expression(ofp);
            //GTR
            emit(ofp, 2, 0, 9);
        }
        else if(token == geqsym){
            token = getToken();
            expression(ofp);
            //GEQ
            emit(ofp, 2, 0, 10);
        }
        else{
            ERROR(ofp, 13);
        }
    }
}

//Katelyn Grimes: Statement - part of grammar
void statement(FILE *ofp){
    if(token == identsym){
        //Katelyn Grimes: get name of identifier
        char name[MAXchar] = {};
        for(int i = 0; tokenList[tCount + 1][i] != 0; i++){
            name[i] = tokenList[tCount + 1][i];
        }
        int symIndex = symbolTableCheck(name);
        if(symIndex == -1){
            ERROR(ofp, 7);
            //Katelyn Grimes: prints the identifier name that is not declared
            for(int i = 0; name[i] != 0; i++){
                printf("%c", name[i]);
                fprintf(ofp, "%c", name[i]);
            }
            printf("\n");
            fprintf(ofp, "\n");
            exit(0);
        }
        if(symbolTable[symIndex].kind != 2){
            ERROR(ofp, 8);
        }
        tCount++;
        token = getToken();
        if(token != becomesym){
            ERROR(ofp, 9);
        }
        token = getToken();
        expression(ofp);
        //STO
        emit(ofp, 4, 0, symbolTable[symIndex].addr);
        return;
    }

    if(token == beginsym){
        do{
            token = getToken();
            statement(ofp);
        }while(token == semicolonsym);
        if(token != endsym){
            ERROR(ofp, 10);
        }
        token = getToken();
        return;
    }

    if(token == ifsym){
        token = getToken();
        condition(ofp);
        int jpcIndex = cx;
        //JPC
        emit(ofp, 8, 0, jpcIndex);
        if(token != thensym){
            ERROR(ofp, 11);
        }
        token = getToken();
        statement(ofp);
        text[jpcIndex].M = cx;
        return;
    }

    if(token == whilesym){
        token = getToken();
        int loopIndex = cx;
        condition(ofp);
        if(token != dosym){
            ERROR(ofp, 12);
        }
        token = getToken();
        int jpcIndex = cx;
        //JPC
        emit(ofp, 8, 0, jpcIndex);
        statement(ofp);
        //JMP
        emit(ofp, 7, 0, loopIndex);
        text[jpcIndex].M = cx;
        return;
    }

    if(token == readsym){
        token = getToken();
        if(token != identsym){
            ERROR(ofp, 2);
        }
        //Katelyn Grimes: get name of identifier
        char name[MAXchar] = {};
        for(int i = 0; tokenList[tCount + 1][i] != 0; i++){
            name[i] = tokenList[tCount + 1][i];
        }
        int symIndex = symbolTableCheck(name);
        if(symIndex == -1){
            ERROR(ofp, 7);
            //Katelyn Grimes: prints the identifier name that is not declared
            for(int i = 0; name[i] != 0; i++){
                printf("%c", name[i]);
                fprintf(ofp, "%c", name[i]);
            }
            printf("\n");
            fprintf(ofp, "\n");
            exit(0);
        }
        if(symbolTable[symIndex].kind != 2){
            ERROR(ofp, 8);
        }
        tCount++;
        token = getToken();
        //READ
        emit(ofp, 9, 0, 2);
        //STO
        emit(ofp, 4, 0, symbolTable[symIndex].addr);
        return;
    }

    if(token == writesym){
        token = getToken();
        expression(ofp);
        //WRITE
        emit(ofp, 9, 0, 1);
        return;
    }

    return;
}

//Katelyn Grimes: Variable Declaration - part of grammar
int variableDeclaration(FILE *ofp){
    int numVars = 0;
    if(token == varsym){
        do{
            numVars++;
            token = getToken();
            if(token != identsym){
                ERROR(ofp, 2);
            }
            //Katelyn Grimes: get name of identifier
            char name[MAXchar] = {};
            for(int i = 0; tokenList[tCount + 1][i] != 0; i++){
                name[i] = tokenList[tCount + 1][i];
            }
            if(symbolTableCheck(name) != -1){
                ERROR(ofp, 3);
            }
            char num[] = {0};
            addSymboltable(2, name, num, 0, numVars + 2, 0);
            tCount++;
            token = getToken();
        }while(token == commasym);
        if(token != semicolonsym){
            ERROR(ofp, 6);
        }
        token = getToken();
    }
    return numVars;
}

//Katelyn Grimes: Constant Declaration - part of grammar
void constantDeclaration(FILE *ofp){
    if(token == constsym){
        do{
            token = getToken();
            if(token != identsym){
                ERROR(ofp, 2);
            }
            //Katelyn Grimes: get name of identifier
            char name[MAXchar] = {};
            for(int i = 0; tokenList[tCount + 1][i] != 0; i++){
                name[i] = tokenList[tCount + 1][i];
            }
            if(symbolTableCheck(name) != -1){
                ERROR(ofp, 3);
            }
            tCount++;
            token = getToken();
            if(token != eqlsym){
                ERROR(ofp, 4);
            }
            token = getToken();
            if(token != numbersym){
                ERROR(ofp, 5);
            }
            //Katelyn Grimes: get value of identifier
            char num[MAXdig] = {};
            for(int i = 0; tokenList[tCount + 1][i] != 0; i++){
                num[i] = tokenList[tCount + 1][i];
            }
            addSymboltable(1, name, num, 0, 0, 0);
            tCount++;
            token = getToken();
        }while(token == commasym);
        if(token != semicolonsym){
            ERROR(ofp, 6);
        }
        token = getToken();
    }
}

//Katelyn Grimes: Block - part of grammar
void block(FILE *ofp){
    constantDeclaration(ofp);
    int numVars = variableDeclaration(ofp);
    //INC
    emit(ofp, 6, 0, numVars + 3);
    statement(ofp);
}

//Katelyn Grimes: Program - part of grammar and the starting symbol
void program(FILE *ofp){
    token = getToken();
    block(ofp);
    if(token != periodsym){
        ERROR(ofp, 1);
    }
    //HALT
    emit(ofp, 9, 0, 3);
}

//Katelyn Grimes: Prints the code generated from parsing through the tokens to screen and output file
void printAssembly(FILE *ofp){
    for(int i = 0; i < cx; i++){
        printf("%3d\t", i);
        fprintf(ofp, "%3d\t", i);
        codeGen(ofp, text[i].op);
        printf("\t%d\t%d\n", text[i].L, text[i].M);
        fprintf(ofp, "\t%d\t%d\n", text[i].L, text[i].M);
    }
}

//Katelyn Grimes: Gets the size of an array (used to help with print formatting)
int getSize(char arr[]){
    int count = 0;
    for(int j = 0; arr[j] != 0; j++){
        count++;
    }
    return count;
}

//Katelyn Grimes: Prints the symbol table to the screen and output file
void printSymbolTable(FILE *ofp){
    printf("Kind | Name        | Value | Level | Address | Mark\n");
    fprintf(ofp, "Kind | Name        | Value | Level | Address | Mark\n");
    printf("---------------------------------------------------\n");
    fprintf(ofp, "---------------------------------------------------\n");
    for(int i = 0; i < symbolCount; i++){
        printf("%4d | ", symbolTable[i].kind);
        fprintf(ofp, "%4d | ", symbolTable[i].kind);
        int sizeName = getSize(symbolTable[i].name);
        for(int i = 0; i < 11 - sizeName; i++){
            printf(" ");
            fprintf(ofp, " ");
        }
        for(int j = 0; symbolTable[i].name[j] != 0; j++){
            printf("%c", symbolTable[i].name[j]);
            fprintf(ofp, "%c", symbolTable[i].name[j]);
        }
        printf(" | ");
        fprintf(ofp, " | ");
        int sizeNum = getSize(symbolTable[i].val);
        for(int i = 0; i < 5 - sizeNum; i++){
            printf(" ");
            fprintf(ofp, " ");
        }
        if(symbolTable[i].val[0] == 0){
            printf("%c", symbolTable[i].val[0]);
            fprintf(ofp, "%c", symbolTable[i].val[0]);
        }
        else{
            for(int j = 0; symbolTable[i].val[j] != 0; j++){
                printf("%c", symbolTable[i].val[j]);
                fprintf(ofp, "%c", symbolTable[i].val[j]);
            }
        }
        printf(" | ");
        fprintf(ofp, " | ");
        printf("%5d | ", symbolTable[i].level);
        fprintf(ofp, "%5d | ", symbolTable[i].level);
        if(symbolTable[i].addr >= 10){
            printf("%6d | ", symbolTable[i].addr);
            fprintf(ofp, "%6d | ", symbolTable[i].addr);
        }
        else{
            printf("%7d | ", symbolTable[i].addr);
            fprintf(ofp, "%7d | ", symbolTable[i].addr);
        }
        printf("%3d\n", symbolTable[i].mark);
        fprintf(ofp, "%3d\n", symbolTable[i].mark);
    }
}

int main(int argc, char *argv[])
{
    //Katelyn Grimes: Lexical Analyzer
    //--------------------------------

    //Katelyn Grimes: Read input file
    FILE *ifp = fopen("exam.txt", "r");
    //Katelyn Grimes: Write output file
    FILE *ofp = fopen("output.txt", "w");
    readInput(ifp);

    //Katelyn Grimes: all printing called has been commented out or deleted in the lexical analyzer

    //Katelyn Grimes: Create lexical units
    createLexUnits(ofp);

    //Katelyn Grimes: Create token list
    createTokenList();

    //Katelyn Grimes: Code Generation
    //-------------------------------

    //Katelyn Grimes: first instruction is always JMP 0 3
    text[0].op = 7;
    text[0].L = 0;
    text[0].M = 3;
    cx++;

    //Katelyn Grimes: parse through token list to generate code - starting symbol is "program"
    program(ofp);

    //Katelyn Grimes: print the assembly code
    printf("Line\tOP\tL\tM\n");
    fprintf(ofp, "Line\tOP\tL\tM\n");
    printAssembly(ofp);
    printf("\n\n");
    fprintf(ofp, "\n\n");

    //Katelyn Grimes: sets mark to 1 indicating symbols are no longer needed or code generation is complete
    for(int i = 0; i < symbolCount; i++){
        symbolTable[i].mark = 1;
    }

    //Katelyn Grimes: print symbol table
    printf("Symbol Table:\n\n");
    fprintf(ofp, "Symbol Table:\n\n");
    printSymbolTable(ofp);
    printf("\n");
    fprintf(ofp, "\n");

    fclose(ifp);
    fclose(ofp);

    return 0;
}
