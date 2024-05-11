//Katelyn Grimes
//COP3402: Fall 2023
//HW2: Lexical Analyzer
//September 29, 2023

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//Katelyn Grimes: defined variables for max sizes
#define MAXchar 11
#define MAXdig 5
#define reserveMAX 14
#define symbolMAX 13
#define MAXSIZE 1000

//Katelyn Grimes: declaration of token types
typedef enum{
    skipsym=1, identsym=2, numbersym=3, plussym=4, minussym=5, multsym=6, slashnum=7,
    ifelsym=8, eqlsym=9, neqsym=10, lessym=11, leqsym=12, gtrsym=13, geqsym=14,
    lparentsym=15, rparentsym=16, commasym=17, semicolonsym=18, periodsym=19,
    becomesym=20, beginsym=21, endsym=22, ifsym=23, thensym=24, whilesym=25,
    dosym=26, callsym=27, constsym=28, varsym=29, procsym=30, writesym=31,
    readsym=32, elsesym=33
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

//Katelyn Grimes: list of names and symbols with corresponding internal representation arrays
char *reservedWords[] = {"begin", "end", "if", "then", "while", "do", "call", "const", "var", "procedure", "write", "read", "else", "ifel"};
int nameInternalRep[] = {beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, constsym, varsym, procsym, writesym, readsym, elsesym, ifelsym};
//Katelyn Grimes: list of valid special symbols
char specialSym[] = {'+', '-', '*', '/', '=', '<', '>', '(', ')', ',', ';', '.', ':'};

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

//Katelyn Grimes: Checks if a word is a reserved word or identifier
int findWordType(char word[], int l, int length, FILE *ofp){
    //Katelyn Grimes: first check if the word is part of reserved word list
    for(int i = 0; i < reserveMAX; i++){
        if(strcmp(word, reservedWords[i]) == 0){
            for(int size = 0; size < length; size++){
                lexUnits[l][size] = word[size];
            }
            tokenType[l] = nameInternalRep[i];
            printTableLine(l, ofp);
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
        printTableLine(l, ofp);
        l++;
    }
    else{
        for(int i = 0; word[i] != 0; i++){
            idNames[idCount][i] = word[i];
            lexUnits[l][i] = word[i];
        }
        tokenType[l] = identsym;
        printTableLine(l, ofp);
        l++;
        idCount++;
    }

    return l;
}

//Katelyn Grimes: Groups characters into lexical units and stores into arrays
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
                    if(length > MAXchar - 1){
                        error = 1;
                    }
                    temp[length] = inputArray[i];
                    length++;
                    i++;
                }
                if(error == 1){
                    for(int h = 0; temp[h] != 0; h++){
                        printf("%c", temp[h]);
                        fprintf(ofp, "%c", temp[h]);
                    }
                    printf("\tError: Name too long!\n");
                    fprintf(ofp, "\tError: Name too long!\n");
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
                    printTableLine(l, ofp);
                    l++;
                    i++;
                }
                else{
                    idNames[idCount][0] = inputArray[i];
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = identsym;
                    printTableLine(l, ofp);
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
                    if(length > MAXdig - 1){
                        error = 1;
                    }
                    temp2[length] = inputArray[i];
                    length++;
                    i++;
                }
                if(error == 1){
                    for(int h = 0; temp2[h] != 0; h++){
                        printf("%c", temp2[h]);
                        fprintf(ofp, "%c", temp2[h]);
                    }
                    if(length >= 8){
                      printf("\t\tError: Number too long!\n");
                      fprintf(ofp, "\t\tError: Number too long!\n");
                    }
                    else{
                      printf("\t\t\tError: Number too long!\n");
                      fprintf(ofp, "\t\t\tError: Number too long!\n");
                    }
                }
                else{
                    for(int size = 0; size < length; size++){
                        lexUnits[l][size] = temp2[size];
                    }
                    tokenType[l] = numbersym;
                    printTableLine(l, ofp);
                    l++;
                }
                //Katelyn Grimes: reset temp array
                memset(temp2, '\0', sizeof temp2);
            }
            //Katelyn Grimes: Only one digit
            else{
                lexUnits[l][0] = inputArray[i];
                tokenType[l] = numbersym;
                printTableLine(l, ofp);
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
                    tokenType[l] = slashnum;
                    printTableLine(l, ofp);
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
                        printTableLine(l, ofp);
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
                        printTableLine(l, ofp);
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
                        printTableLine(l, ofp);
                        l++;
                        i = i + 2;
                    }
                    //Katelyn Grimes: less than case
                    else{
                        lexUnits[l][0] = inputArray[i];
                        tokenType[l] = lessym;
                        printTableLine(l, ofp);
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
                        printTableLine(l, ofp);
                        l++;
                        i = i + 2;
                    }
                    //Katelyn Grimes: greater than case
                    else{
                        lexUnits[l][0] = inputArray[i];
                        tokenType[l] = gtrsym;
                        printTableLine(l, ofp);
                        l++;
                        i++;
                    }
                }
                //Katelyn Grimes: plus case
                if(inputArray[i] == '+'){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = plussym;
                    printTableLine(l, ofp);
                    l++;
                    i++;
                }
                //Katelyn Grimes: minus case
                if(inputArray[i] == '-'){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = minussym;
                    printTableLine(l, ofp);
                    l++;
                    i++;
                }
                //Katelyn Grimes: multiply case
                if(inputArray[i] == '*'){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = multsym;
                    printTableLine(l, ofp);
                    l++;
                    i++;
                }
                //Katelyn Grimes: left parenthesis case
                if(inputArray[i] == '('){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = lparentsym;
                    printTableLine(l, ofp);
                    l++;
                    i++;
                }
                //Katelyn Grimes: right parenthesis case
                if(inputArray[i] == ')'){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = rparentsym;
                    printTableLine(l, ofp);
                    l++;
                    i++;
                }
                //Katelyn Grimes: equal case
                if(inputArray[i] == '='){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = eqlsym;
                    printTableLine(l, ofp);
                    l++;
                    i++;
                }
                //Katelyn Grimes: comma case
                if(inputArray[i] == ','){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = commasym;
                    printTableLine(l, ofp);
                    l++;
                    i++;
                }
                //Katelyn Grimes: semicolon case
                if(inputArray[i] == ';'){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = semicolonsym;
                    printTableLine(l, ofp);
                    l++;
                    i++;
                }
                //Katelyn Grimes: period case
                if(inputArray[i] == '.'){
                    lexUnits[l][0] = inputArray[i];
                    tokenType[l] = periodsym;
                    printTableLine(l, ofp);
                    l++;
                    i++;
                }
            }
        }
        //Katelyn Grimes: Error - not valid input
        else{
            printf("%c", inputArray[i]);
            fprintf(ofp, "%c", inputArray[i]);
            printf("\t\t\tError: Invalid symbol!\n");
            fprintf(ofp, "\t\t\tError: Invalid symbol!\n");
            i++;
        }

    }while(inputArray[i] != '\0');

    //Katelyn Grimes: Need to include the period
    /*lexUnits[l][0] = inputArray[i];
    tokenType[l] = periodsym;
    printTableLine(l, ofp);
    l++;
    i++;*/

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

int main(int argc, char *argv[])
{
    //Katelyn Grimes: Read input file
    FILE *ifp = fopen(argv[1], "r");
    //Katelyn Grimes: Write output file
    FILE *ofp = fopen("output.txt", "w");
    readInput(ifp);

    //Katelyn Grimes: Print the source code
    printf("Source Program:\n");
    fprintf(ofp, "Source Program:\n");
    printArray(ofp);
    printf("\n\nLexeme Table:\n\n");
    fprintf(ofp, "\n\nLexeme Table:\n\n");
    printf("lexeme\t\t\ttoken type\n");
    fprintf(ofp, "lexeme\t\t\ttoken type\n");

    //Katelyn Grimes: Separate into lexical units
    createLexUnits(ofp);
    printf("\n\n");
    fprintf(ofp, "\n\n");

    //Katelyn Grimes: Create token list
    printf("Token List:\n");
    fprintf(ofp, "Token List:\n");
    createTokenList();
    printTokenList(ofp);
    printf("\n");
    fprintf(ofp,"\n");

    fclose(ifp);
    fclose(ofp);

    return 0;
}
