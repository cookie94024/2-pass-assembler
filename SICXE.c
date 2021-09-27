#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * OPTAB
 */
struct Operation
{
    char *mnemonic;
    int length;
    int opcode;
    int format;
};

typedef struct Operation Operation;

// Mnemonic, length, Opcode, Format( 3 mean 3/4 )
Operation ADD = {"ADD", 3, 0x18, 3};
Operation ADDF = {"ADDF", 3, 0x58, 3};
Operation ADDR = {"ADDR", 2, 0x90, 2};
Operation AND = {"AND", 3, 0x40, 3};
Operation CLEAR = {"CLEAR", 2, 0xB4, 2};
Operation COMPF = {"COMPF", 3, 0x88, 3};
Operation COMPR = {"COMPR", 2, 0xA0, 2};
Operation COMP = {"COMP", 3, 0x28, 3};
Operation DIVF = {"DIVF", 3, 0x64, 3};
Operation DIVR = {"DIVR", 2, 0x9C, 2};
Operation DIV = {"DIV", 3, 0x24, 3};
Operation FIX = {"FIX", 1, 0xC4, 1};
Operation FLOAT = {"FLOAT", 1, 0xC0, 1};
Operation HIO = {"HIO", 1, 0xF4, 1};
Operation J = {"J", 3, 0x3C, 3};
Operation JEQ = {"JEQ", 3, 0x30, 3};
Operation JGT = {"JGT", 3, 0x34, 3};
Operation JLT = {"JLT", 3, 0x38, 3};
Operation JSUB = {"JSUB", 3, 0x48, 3};
Operation LDA = {"LDA", 3, 0x00, 3};
Operation LDB = {"LDB", 3, 0x68, 3};
Operation LDCH = {"LDCH", 3, 0x50, 3};
Operation LDF = {"LDF", 3, 0x70, 3};
Operation LDL = {"LDL", 3, 0x08, 3};
Operation LDS = {"LDS", 3, 0x6C, 3};
Operation LDT = {"LDT", 3, 0x74, 3};
Operation LDX = {"LDX", 3, 0x04, 3};
Operation LPS = {"LPS", 3, 0xE0, 3};
Operation UML = {"UML", 3, 0x20, 3};
Operation MULF = {"MULF", 3, 0x60, 3};
Operation MULR = {"MULR", 2, 0x98, 2};
Operation NORM = {"NORM", 1, 0xC8, 1};
Operation OR = {"OR", 3, 0x44, 3};
Operation RD = {"RD", 3, 0xD8, 3};
Operation RMO = {"RMO", 2, 0xAC, 2};
Operation RSUB = {"RSUB", 3, 0x4C, 3};
Operation SHIFTL = {"SHIFTL", 2, 0xA4, 2};
Operation SHIFTR = {"SHIFTR", 2, 0xA8, 2};
Operation SIO = {"SIO", 1, 0xF0, 1};
Operation SSK = {"SSK", 3, 0xEC, 3};
Operation STA = {"STA", 3, 0x0C, 3};
Operation STB = {"STB", 3, 0x78, 3};
Operation STCH = {"STCH", 3, 0x54, 3};
Operation STF = {"STF", 3, 0x80, 3};
Operation STI = {"STI", 3, 0xD4, 3};
Operation STL = {"STL", 3, 0x14, 3};
Operation STSW = {"STSW", 3, 0xE8, 3};
Operation STS = {"STS", 3, 0x7C, 3};
Operation STT = {"STT", 3, 0x84, 3};
Operation STX = {"STX", 3, 0x10, 3};
Operation SUBF = {"SUBF", 3, 0x5C, 3};
Operation SUBR = {"SUBR", 2, 0x94, 2};
Operation SUB = {"SUB", 3, 0x1C, 3};
Operation SVC = {"SVC", 2, 0xB0, 2};
Operation TD = {"TD", 3, 0xE0, 3};
Operation TIO = {"TIO", 1, 0xF8, 1};
Operation TIXR = {"TIXR", 2, 0xB8, 2};
Operation TIX = {"TIX", 3, 0x2C, 3};
Operation WD = {"WD", 3, 0xDC, 3};

Operation *OPTAB[] = {
    &ADD,
    &ADDF,
    &ADDR,
    &AND,
    &CLEAR,
    &COMPF,
    &COMPR,
    &COMP,
    &DIVF,
    &DIVR,
    &DIV,
    &FIX,
    &FLOAT,
    &HIO,
    &J,
    &JEQ,
    &JGT,
    &JLT,
    &JSUB,
    &LDA,
    &LDB,
    &LDCH,
    &LDF,
    &LDL,
    &LDS,
    &LDT,
    &LDX,
    &LPS,
    &UML,
    &MULF,
    &MULR,
    &NORM,
    &OR,
    &RD,
    &RMO,
    &RSUB,
    &SHIFTL,
    &SHIFTR,
    &SIO,
    &SSK,
    &STA,
    &STB,
    &STCH,
    &STF,
    &STI,
    &STL,
    &STSW,
    &STS,
    &STT,
    &STX,
    &SUBF,
    &SUBR,
    &SUB,
    &SVC,
    &TD,
    &TIO,
    &TIXR,
    &TIX,
    &WD,
};

struct Relocate_Address
{
    char address[7];
};

typedef struct Relocate_Address RE;

/**
 * Global variables
 */

FILE *input_file, *pass1_output_file, *symtab_file;                                                       // for pass 1.
FILE *intermediate_file, *object_file, *output_file;                                                      // for pass 2.
char LABEL[10], OPCODE[10], OPERAND[10], OPERAND2[10], ORIGIN_OPERAND[10], LOCATION[10], OBJECT_CODE[10]; // Variables for input.
int LOCCTR, PREV_LOCCTR, PROGRAM_LENGTH;                                                                  // Variables for pass1 output file.
int BASE, BASE_SYMBOL;
char CODE[10], MNEMONIC[10]; // Variables for optab.
char OBJECT[20];
bool HAS_LABEL = false;
bool HAS_PLUS = false;
bool NO_OPERAND = false;

RE RETAB[30];

/**
 * Pass 1 functions
 */

void pass1();             // pass1 function
void lineHandler();       // Get current line LABEL, OPCODE and OPERAND.
void insertPass1Output(); // Insert current LABEL, OPCODE, OPERAND and LOCCTR to pass1 output file.
void insertSymtab();      //Insert current LABEL and LOCCTR to Symtab file.
int searchOptab();        //Search current OPCODE is in OPTAB.
int getByteLength();
//char *trim();              //Remove \n or \t in the string

/**
 * Pass 2 functions
 */

void pass2();            // pass 2 function (not complete yet);
void pass2LineHandler(); //line handler for pass 2.
void convertToObject();  //covert to object code.
int searchSymtab();      //search symtab.
int format2Register();   //get register.
void insertOutputFile();
int isRelative();
int isBase();
void insertTextRecord();
void textRecordLineHandler();
void saveRelocateAddress();

/**
 * General functions
 */

void clearGlobal();

/**
 * Debug functions
 */
void printCurrentLine();
void printOptab();

void main()
{
    /**
     * Pass 1.
     */

    // Open input file
    input_file = fopen("input.txt", "r");
    if (!input_file)
    {
        printf("Unable to open input file.");
        system("pause");
        exit(1);
    }

    // Open pass 1 output file
    pass1_output_file = fopen("pass1_output_file.txt", "w");
    if (!pass1_output_file)
    {
        printf("Unable to open pass1 output file.");
        system("pause");
        exit(1);
    }

    // Open pass 1 output file (symtab)
    symtab_file = fopen("symtab.txt", "w");
    if (!symtab_file)
    {
        printf("Unable to open symtab file.");
        system("pause");
        exit(1);
    }

    // Exec pass 1 function
    pass1();

    printf("PASS 1 FINISH... \n");
    fclose(input_file);
    fclose(pass1_output_file);
    fclose(symtab_file);

    //Clear all global variables.
    clearGlobal();

    // Open intermediate file output file
    intermediate_file = fopen("pass1_output_file.txt", "r");
    if (!intermediate_file)
    {
        printf("Unable to open intermediate file.");
        system("pause");
        exit(1);
    }

    symtab_file = fopen("symtab.txt", "r");
    if (!symtab_file)
    {
        printf("Unable to open symtab file.");
        system("pause");
        exit(1);
    }

    // Open pass 2 output file (symtab)
    object_file = fopen("object_file.txt", "w");
    if (!object_file)
    {
        printf("Unable to open object file.");
        system("pause");
        exit(1);
    }

    output_file = fopen("output_file.txt", "w");
    if (!output_file)
    {
        printf("Unable to open output file.");
        system("pause");
        exit(1);
    }
    //Exec pass 2 function
    pass2();
    fclose(object_file);
    fclose(output_file);

    printf("PASS 2 FINISH...\n");
    
    system("pause");
}

void pass1()
{
    char line[100];
    char copy_line[100];
    int line_counter = 0; // For error handling

    // Handle first line of the input
    fgets(line, sizeof(line), input_file);
    lineHandler(line);

    // If OPCODE is "START"
    if (!strcmp(OPCODE, "START"))
    {
        LOCCTR = atoi(OPERAND);

        // Insert the first line in pass1 output file.
        fprintf(pass1_output_file, "%04X\t%s\t%s\t%s", LOCCTR, LABEL, OPCODE, OPERAND);
        // Insert the first line in symtab file.
        fprintf(symtab_file, "%s\t%X\n", LABEL, LOCCTR);

        // Go to next line
        fgets(line, sizeof(line), input_file);
        lineHandler(line);
        line_counter++;
    }
    else
    {
        LOCCTR = 0;
    }

    int START_LOCCTR = LOCCTR;
    searchOptab();

    // Loop until hit OPCODE is "END"
    while (strcmp(OPCODE, "END"))
    {
        int optab_index;

        PREV_LOCCTR = LOCCTR;

        // if has LABEL
        if (HAS_LABEL == true)
        {
            insertSymtab(LABEL);
        }

        // If OPCODE is in optab, then return the index of opcode, if not, return -1.
        optab_index = searchOptab();

        if (optab_index != -1)
        {
            int length = OPTAB[optab_index]->length;
            if (HAS_PLUS == true)
            {
                LOCCTR += length + 1;
            }
            else
            {
                LOCCTR += length;
            }
            insertPass1Output(0);
        }
        else if (!strcmp(OPCODE, "RESW"))
        {
            LOCCTR += (3 * atoi(OPERAND));
            insertPass1Output(0);
        }
        else if (!strcmp(OPCODE, "RESB"))
        {
            LOCCTR += atoi(OPERAND);
            insertPass1Output(0);
        }
        else if (!strcmp(OPCODE, "WORD"))
        {
            LOCCTR += 3;
            insertPass1Output(0);
        }
        else if (!strcmp(OPCODE, "BYTE"))
        {
            LOCCTR += getByteLength();
            insertPass1Output(0);
        }
        else if (!strcmp(OPCODE, "BASE"))
        {
            // Base has no location, exception case.
            insertPass1Output(1);
        }
        else
        {
            line_counter++;
            printf("ERROR: Unable to get OPCODE. At line %d\n", line_counter);
            system("pause");
            exit(0);
        }
        // Get next line
        fgets(line, sizeof(line), input_file);
        lineHandler(line);
        line_counter++;
    }

    //File end.
    insertPass1Output(1);

    // save product length, for pass 2
    PROGRAM_LENGTH = LOCCTR - START_LOCCTR;
}

/**
 * Process input line and get label, opcode and operand.
 */
void lineHandler(char *line)
{
    char string[100], *temp;

    // Clear current line variables when this function is called.
    clearGlobal();
    // LABEL[0] = '\0';
    // OPCODE[0] = '\0';
    // OPERAND[0] = '\0';

    /**
     * ==========Get label==========
     */

    // Copy line
    strcpy(string, line);

    // Check if there are symbol in the label field.
    if (string[0] != '\t')
    {
        // Get label
        temp = strtok(string, "	");

        // Save as current LABEL
        strcpy(LABEL, temp);

        // Get next, (+1 is \t)
        line = line + strlen(LABEL) + 1;
        strcpy(string, line);

        HAS_LABEL = true;
    }
    else
    {
        // Get next
        line++;
        strcpy(string, line);

        HAS_LABEL = false;
    }

    /**
     * ==========Get OPCODE==========
     */
    temp = strtok(string, "	");
    // Has plus sign, extended
    if (temp[0] == '+')
    {
        // temp = &temp[1];
        HAS_PLUS = true;
    }
    else
    {
        HAS_PLUS = false;
    }

    // If no OPERAND (for RSUB)
    if (temp[strlen(temp) - 1] == '\n')
    {
        char temp2[20];
        strcpy(temp2, temp);
        strtok(temp2, "\n");
        strcpy(OPCODE, temp2);

        NO_OPERAND = true;

        return;
    }
    else
    {
        NO_OPERAND = false;
    }
    strcpy(OPCODE, temp);

    line = line + strlen(OPCODE) + 1;
    strcpy(string, line);

    /**
     * ==========Get OPERAND==========
     */

    // Check if there are operand (not space nor break)
    if (string[0] != '\t' || string[0] != '\n')
    {
        temp = strtok(string, " ");
        strcpy(OPERAND, temp);
    }
}

/**
 * Insert current LABEL and LOCCTR to Symtab.
 */

void insertSymtab()
{
    char copy_label[10];
    int copy_locctr;

    strcpy(copy_label, LABEL);
    copy_locctr = LOCCTR;

    fprintf(symtab_file, "%s\t%X\n", LABEL, LOCCTR);
}

int searchOptab()
{
    char copy_opcode[20];
    strcpy(copy_opcode, OPCODE);
    int size = sizeof(OPTAB) / sizeof(OPTAB[0]);
    if (HAS_PLUS == true)
    {
        memmove(copy_opcode, copy_opcode + 1, strlen(copy_opcode));
    }

    int i;
    for (i = 0; i < size; i++)
    {
        char mnemonic[10];
        strcpy(mnemonic, OPTAB[i]->mnemonic);
        if (strcmp(mnemonic, copy_opcode) == 0)
        {
            return i;
        }
    }

    // If no match, return -1.
    return -1;
}

void insertPass1Output(int type)
{
    if (type == 0)
    {
        if (NO_OPERAND == true)
        {
            fprintf(pass1_output_file, "%04X\t%s\t%s\t%s\n", PREV_LOCCTR, LABEL, OPCODE, OPERAND);

            return;
        }

        fprintf(pass1_output_file, "%04X\t%s\t%s\t%s", PREV_LOCCTR, LABEL, OPCODE, OPERAND);

        return;
    }
    else if (type == 1)
    {
        fprintf(pass1_output_file, "\t%s\t%s\t%s", LABEL, OPCODE, OPERAND);

        return;
    }
    else
    {
        printf("Error: Unvalid type");
        system("pause");
        exit(0);
    }
}

int getByteLength()
{
    char *stringA, *stringB, copy_operand[20];
    strcpy(copy_operand, OPERAND);
    stringA = strtok(copy_operand, "'");
    stringB = strtok(NULL, "'");

    // Debug
    // printf("OPERAND: %s\n", OPERAND);
    // printf("stringA: %s\n", stringA);
    // printf("stringB: %s\n", stringB);

    if (stringA[0] == 'X')
    {
        return 1;
    }
    else
    {
        return strlen(stringB);
    }

    return 0;
}

/**
 * Pass 2
 */

void pass2()
{
    char line[100];

    fgets(line, sizeof(line), intermediate_file);
    pass2LineHandler(line);

    if (!strcmp(OPCODE, "START"))
    {
        // Header record
        fprintf(object_file, "H%-6s00%s%05X\n", LABEL, LOCATION, PROGRAM_LENGTH);

        // Get next line
        fgets(line, sizeof(line), intermediate_file);
        pass2LineHandler(line);
    }

    // Start loop until hit 'END'
    while (strcmp(OPCODE, "END"))
    {

        // Check base, and save it's location into BASE variable
        if (!strcmp(OPCODE, "BASE"))
        {
            BASE = searchSymtab(&OPERAND[1]);
        }

        convertToObject();

        fgets(line, sizeof(line), intermediate_file);
        pass2LineHandler(line);
    }

    // write last line.
    fprintf(output_file, "%s\t%s\t%s", LABEL, OPCODE, OPERAND);

    fclose(output_file);
    insertTextRecord();

    //pass 2 end.
}

void pass2LineHandler(char *line)
{
    char string[100], *temp;

    //Clear all global variables.
    clearGlobal();

    /**
     * ==========Get location==========
     */

    // Check if there are location.
    strcpy(string, line);
    if (string[0] != '\t')
    {
        // Get location
        temp = strtok(string, "	");

        // Save as current location
        strcpy(LOCATION, temp);

        // Get next
        line = line + strlen(LOCATION) + 1;
        strcpy(string, line);
    }

    /**
     * ==========Get label==========
     */

    // Check if there are symbol in the label field.
    if (string[0] != '\t')
    {
        // Get label
        temp = strtok(string, "	");

        // Save as current LABEL
        strcpy(LABEL, temp);

        // Get next, (+1 is \t)
        line = line + strlen(LABEL) + 1;
        strcpy(string, line);

        HAS_LABEL = true;
    }
    else
    {
        // Get next
        line++;
        strcpy(string, line);

        HAS_LABEL = false;
    }

    /**
     * ==========Get OPCODE==========
     */
    temp = strtok(string, "	");
    // Has plus sign, extended
    if (temp[0] == '+')
    {
        // temp = &temp[1];
        HAS_PLUS = true;
    }
    else
    {
        HAS_PLUS = false;
    }
    // Save as current OPCODE

    // If no OPERAND (for RSUB)
    if (temp[strlen(temp) - 1] == '\n')
    {
        char temp2[20];
        strcpy(temp2, temp);
        strtok(temp2, "\n");
        strcpy(OPCODE, temp2);

        NO_OPERAND = true;

        // No OPERAND, return here.
        return;
    }
    else
    {
        NO_OPERAND = false;
    }
    strcpy(OPCODE, temp);

    line = line + strlen(OPCODE) + 1;
    strcpy(string, line);

    /**
     * ==========Get OPERAND==========
     */

    strcpy(ORIGIN_OPERAND, string);
    //Clear \n in the end
    if (ORIGIN_OPERAND[strlen(ORIGIN_OPERAND) - 1] == '\n')
    {
        ORIGIN_OPERAND[strlen(ORIGIN_OPERAND) - 1] = '\0';
    }

    // Check if is byte.
    if (!strcmp(OPCODE, "BYTE"))
    {
        temp = strtok(string, "'");
        strcpy(OPERAND, temp);
        temp = strtok(NULL, "'");
        if (temp != NULL)
        {
            strcpy(OPERAND2, temp);
        }
    }
    // Check if there are operand (not space nor break)
    else if (string[0] != '\t' || string[0] != '\n')
    {
        temp = strtok(string, " ");
        temp = strtok(string, ",");
        strcpy(OPERAND, temp);
        temp = strtok(NULL, ",");

        // Check if there is second OPERAND
        if (temp != NULL)
        {
            strcpy(OPERAND2, temp);
            OPERAND2[strlen(OPERAND2) - 1] = '\0';
        }
        else
        {
            OPERAND[strlen(OPERAND) - 1] = '\0';
        }
    }

    return;
}

int searchSymtab(char *string)
{
    char label[10], location[10], prev_label[10];
    char label_copy[10];

    strcpy(label_copy, string);
    while (true)
    {
        fscanf(symtab_file, "%s\t%s", label, location);

        // find current label in symtab, if find, return location.
        if (!strcmp(label_copy, label))
        {
            //reset the file
            rewind(symtab_file);
            return (int)strtol(location, NULL, 16);
        }

        // end of the file
        if (!strcmp(prev_label, label))
        {
            //reset the file
            rewind(symtab_file);
            return -1;
        }

        strcpy(prev_label, label);
    }
}

void convertToObject()
{
    int n, i, x, b, p, e; // Declare flags.
    int disp = 0;         // Declare displacement.
    int optab_index;

    optab_index = searchOptab();

    if (optab_index != -1)
    {
        // Start to check format.

        // There is no format one in input file, so I just skip it.
        if (OPTAB[optab_index]->format == 1)
        {
            disp = OPTAB[optab_index]->opcode;
            insertOutputFile(0, 0, disp, 1);
        }
        // Format 2
        else if (OPTAB[optab_index]->format == 2)
        {
            int reg_value, reg2_value;
            reg_value = format2Register(OPERAND);

            if (OPERAND2[0] != '\0')
            {
                reg2_value = format2Register(OPERAND2);
            }
            else
            {
                reg2_value = 0;
            }

            insertOutputFile(OPTAB[optab_index]->opcode, reg_value, reg2_value, 2);
        }
        // Format 3
        else if (OPTAB[optab_index]->format == 3 && !HAS_PLUS)
        {

            // no plus sign, so e flag = 0.
            e = 0;

            // Indirect Addressing.
            if (OPERAND[0] == '@')
            {
                n = 1;
                i = 0;
            }
            // Immediate Addressing.
            else if (OPERAND[0] == '#')
            {
                n = 0;
                i = 1;
            }
            // Simple.
            else
            {
                i = 1;
                n = 1;
            }

            // Decide flags and displacement

            // Indirect Addressing
            if (n == 1 && i == 0)
            {
                p = 1;
                b = 0;
                x = 0;

                int pc = (int)strtol(LOCATION, NULL, 16) + OPTAB[optab_index]->length;
                int symbol_location = searchSymtab(&OPERAND[1]);

                disp = symbol_location - pc;
            }
            // Immediate Addressing
            else if (n == 0 && i == 1)
            {
                b = 0;
                x = 0;

                // check if symbol, we dont neet # here.
                int symbol_location = searchSymtab(&OPERAND[1]);

                if (symbol_location == -1)
                {
                    //Immediate
                    p = 0;
                    disp = atoi(&OPERAND[1]);
                }
                else
                {
                    //Immediate + pc relative
                    p = 1;
                    int pc = (int)strtol(LOCATION, NULL, 16) + OPTAB[optab_index]->length;
                    disp = symbol_location - pc;
                }
            }
            else if (OPERAND2[0] != '\0')
            {
                // indexed addressing.
                p = 0;
                b = 1;
                x = 1;
                int symbol_location = searchSymtab(OPERAND);
                disp = symbol_location - BASE;
            }
            // simple
            else
            {
                p = 1;
                b = 0;
                x = 0;

                int pc = (int)strtol(LOCATION, NULL, 16) + OPTAB[optab_index]->length;
                int symbol_location = searchSymtab(OPERAND);

                // try pc relative;
                disp = symbol_location - pc;

                if (!isRelative(disp))
                {
                    // try base relative.
                    disp = symbol_location - BASE;

                    if (!isBase(disp))
                    {
                        printf("unvalid displacement");
                        system("pause");
                        exit(1);
                    }
                }
            }

            // Generate object code.
            int first = OPTAB[optab_index]->opcode + (2 * n) + (1 * i);
            int second = (8 * x) + (4 * b) + (2 * p) + (1 * e);

            // if is RSUB
            if (!strcmp(OPCODE, "RSUB"))
            {
                insertOutputFile(first, 0, 0, 3);
            }
            else
            {
                insertOutputFile(first, second, disp & 0xfff, 3);
            }
        }
        // Format 4
        else if (OPTAB[optab_index]->format == 3 && HAS_PLUS)
        {
            // has plus sign, so e flag = 0.
            e = 1;

            p = 0;

            // Indirect Addressing.
            if (OPERAND[0] == '@')
            {
                n = 1;
                i = 0;
            }
            // Immediate Operand.
            else if (OPERAND[0] == '#')
            {
                n = 0;
                i = 1;
            }
            // Simple.
            else
            {
                i = 1;
                n = 1;

                saveRelocateAddress();
            }

            // Decide flags and displacement

            // Indirect Addressing
            if (n == 1 && i == 0)
            {
                b = 0;
                x = 0;
                disp = searchSymtab(&OPERAND[1]);
            }
            // Immediate Operand
            else if (n == 0 && i == 1)
            {
                b = 0;
                x = 0;
                disp = atoi(&OPERAND[1]);
            }
            // simple
            else
            {
                b = 0;
                x = 0;
                disp = searchSymtab(OPERAND);
            }

            // Generate object code.
            int first = OPTAB[optab_index]->opcode + (2 * n) + (1 * i);
            int second = (8 * x) + (4 * b) + (2 * p) + (1 * e);

            insertOutputFile(first, second, disp, 4);
        }
    }
    else
    {
        insertOutputFile(0, 0, 0, 0);
    }
}

int format2Register(char *reg)
{
    char reg_copy[10];
    strcpy(reg_copy, reg);
    if (!strcmp(reg_copy, "A"))
    {
        return 0;
    }
    else if (!strcmp(reg_copy, "X"))
    {
        return 1;
    }
    else if (!strcmp(reg_copy, "L"))
    {
        return 2;
    }
    else if (!strcmp(reg_copy, "PC"))
    {
        return 8;
    }
    else if (!strcmp(reg_copy, "SW"))
    {
        return 9;
    }
    else if (!strcmp(reg_copy, "B"))
    {
        return 3;
    }
    else if (!strcmp(reg_copy, "S"))
    {
        return 4;
    }
    else if (!strcmp(reg_copy, "T"))
    {
        return 5;
    }
    else if (!strcmp(reg_copy, "F"))
    {
        return 6;
    }
    else
    {
        return -1;
    }
}

void insertOutputFile(int first, int second, int third, int format)
{
    if (format == 4)
    {
        fprintf(output_file, "%s\t%s\t%s\t%s\t%02X%X%05X\n", LOCATION, LABEL, OPCODE, OPERAND, first, second, third);
    }
    else if (format == 3)
    {
        fprintf(output_file, "%s\t%s\t%s\t%s\t%02X%X%03X\n", LOCATION, LABEL, OPCODE, OPERAND, first, second, third);
    }
    else if (format == 2)
    {
        fprintf(output_file, "%s\t%s\t%s\t%s\t%X%d%d\n", LOCATION, LABEL, OPCODE, OPERAND, first, second, third);
    }
    else if (format == 1)
    {
        fprintf(output_file, "%X", third);
    }
    else if (!strcmp(OPCODE, "BYTE"))
    {
        if (!strcmp(OPERAND, "X"))
        {
            fprintf(output_file, "%s\t%s\t%s\t%s\t%s\n", LOCATION, LABEL, OPCODE, ORIGIN_OPERAND, OPERAND2);
        }
        else
        {
            int object_code;
            int i;
            for (i = 0; OPERAND2[i] != '\0'; i++)
            {
                object_code = (int)OPERAND2[i] + (object_code << 8);
            }
            fprintf(output_file, "%s\t%s\t%s\t%s\t%X\n", LOCATION, LABEL, OPCODE, ORIGIN_OPERAND, object_code);
        }
    }
    else
    {
        if (!strcmp(OPCODE, "BASE"))
        {
            fprintf(output_file, "\t\t%s%s\n", OPCODE, OPERAND);
        }
        else
        {
            fprintf(output_file, "%s\t%s\t%s\t%s\n", LOCATION, LABEL, OPCODE, OPERAND);
        }
    }
}

int isRelative(int disp)
{
    if ((disp >= -2048) && (disp < 2048))
    {
        return 1;
    }

    return 0;
}

int isBase(int disp)
{
    if (disp < 4096)
    {
        return 1;
    }

    return 0;
}

void insertTextRecord()
{
    clearGlobal();

    char opcode[10], object_code[10], *next_string;
    char temp[10], temp2[10];
    char newLine[70] = {};
    char line[70] = {};
    char object_codes[70];
    char start_add_and_length[10];
    int location, prev_location;
    int length;
    int start_location;
    bool init = true;
    bool first = true;
    output_file = fopen("output_file.txt", "r");

    fgets(line, sizeof(line), output_file);
    // Delete first white space.
    object_codes[0] = '\0';

    while (strcmp(OPCODE, "END"))
    {
        textRecordLineHandler(line);

        if (OBJECT_CODE[0] == '\0')
        {
            // do nothing
            fgets(line, sizeof(line), output_file);
            continue;
        }
        else if (init == true)
        {
            if (first == true)
            {
                start_location = (int)strtol(LOCATION, NULL, 16);

                first = false;
            }
            strcat(object_codes, OBJECT_CODE);
            location = (int)strtol(LOCATION, NULL, 16);
            prev_location = location;
        }
        else if (!strcmp(LABEL, "EOF"))
        {
            // Insert new line
            strcat(object_codes, OBJECT_CODE);
            newLine[0] = '\0';
            strcat(newLine, "T");
            location = (int)strtol(LOCATION, NULL, 16);
            sprintf(temp, "%06X", prev_location);
            sprintf(temp2, "%02X", location - prev_location + getByteLength());
            prev_location = location;
            strcat(newLine, temp);
            strcat(newLine, temp2);
            strcat(newLine, object_codes);
            fprintf(object_file, "%s\n", newLine);
            object_codes[0] = '\0';

            init = true;
            fgets(line, sizeof(line), output_file);
            continue;
        }
        else if ((strlen(object_codes) + strlen(OBJECT_CODE)) < (69 - 10 + 1))
        {
            strcat(object_codes, OBJECT_CODE);
        }
        else
        {
            // Insert new line
            newLine[0] = '\0';
            strcat(newLine, "T");
            location = (int)strtol(LOCATION, NULL, 16);
            sprintf(temp, "%06X", prev_location);
            sprintf(temp2, "%02X", location - prev_location);
            prev_location = location;
            strcat(newLine, temp);
            strcat(newLine, temp2);
            strcat(newLine, object_codes);
            fprintf(object_file, "%s\n", newLine);
            object_codes[0] = '\0';

            strcat(object_codes, OBJECT_CODE);
        }

        init = false;
        fgets(line, sizeof(line), output_file);
    }

    newLine[0] = '\0';
    strcat(newLine, "T");
    location = (int)strtol(LOCATION, NULL, 16);
    sprintf(temp, "%06X", prev_location);
    sprintf(temp2, "%02X", location - prev_location);
    prev_location = location;
    strcat(newLine, temp);
    strcat(newLine, temp2);
    strcat(newLine, object_codes);
    fprintf(object_file, "%s\n", newLine);

    // write relocate record.
    int relocate_size = sizeof(RETAB) / sizeof(RETAB[0]);
    int i;
    for (i = 0; i < relocate_size; i++)
    {
        if (strcmp(RETAB[i].address, "\0"))
        {
            fprintf(object_file, "M%s05\n", RETAB[i].address);
        }
    }

    // write end record
    fprintf(object_file, "E%06X", start_location);
}

void saveRelocateAddress()
{
    static int index = 0;
    int temp;
    temp = (int)strtol(LOCATION, NULL, 16) + 1;
    sprintf(RETAB[index].address, "%06X", temp);
    index++;
}

void textRecordLineHandler(char *line)
{
    char string[100], *temp;

    //Clear all global variables.
    clearGlobal();

    /**
     * ==========Get location==========
     */

    // Check if there are location.
    strcpy(string, line);
    if (string[0] != '\t')
    {
        // Get location
        temp = strtok(string, "	");

        // Save as current location
        strcpy(LOCATION, temp);

        // Get next
        line = line + strlen(LOCATION) + 1;
        strcpy(string, line);
    }

    /**
     * ==========Get label==========
     */

    // Check if there are symbol in the label field.
    if (string[0] != '\t')
    {
        // Get label
        temp = strtok(string, "	");

        // Save as current LABEL
        strcpy(LABEL, temp);

        // Get next, (+1 is \t)
        line = line + strlen(LABEL) + 1;
        strcpy(string, line);

        HAS_LABEL = true;
    }
    else
    {
        // Get next
        line++;
        strcpy(string, line);

        HAS_LABEL = false;
    }

    /**
     * ==========Get OPCODE==========
     */
    temp = strtok(string, "	");
    // Has plus sign, extended
    if (temp[0] == '+')
    {
        // temp = &temp[1];
        HAS_PLUS = true;
    }
    else
    {
        HAS_PLUS = false;
    }
    // Save as current OPCODE

    // If no OPERAND (for RSUB)
    if (temp[strlen(temp) - 1] == '\n')
    {
        char temp2[20];
        strcpy(temp2, temp);
        strtok(temp2, "\n");
        strcpy(OPCODE, temp2);

        NO_OPERAND = true;

        // No OPERAND, return here.
        return;
    }
    else
    {
        NO_OPERAND = false;
    }
    strcpy(OPCODE, temp);

    line = line + strlen(OPCODE) + 1;
    strcpy(string, line);

    if (!strcmp(OPCODE, "BASE"))
    {
        return;
    }

    if (!strcmp(OPCODE, "RSUB"))
    {
        // Get the object code.
        strtok(line, "\n");

        // Delete the white space.
        line = line + 1;
        strcpy(OBJECT_CODE, line);

        return;
    }

    if (!strcmp(OPCODE, "END"))
    {
        return;
    }

    /**
     * ==========Get OPERAND==========
     */

    temp = strtok(string, "	");
    strcpy(OPERAND, temp);
    if (OPERAND[strlen(OPERAND) - 1] == '\n')
    {
        strtok(OPERAND, "\n");
        return;
    }
    line = line + strlen(OPERAND) + 1;
    strcpy(string, line);

    /**
     * ==========Get Object code==========
     */
    temp = strtok(string, " ");
    strcpy(OBJECT_CODE, temp);

    strtok(OBJECT_CODE, "\n");

    return;
}

void clearGlobal()
{
    //Clear all global variables.
    LOCATION[0] = '\0';
    LABEL[0] = '\0';
    OPCODE[0] = '\0';
    ORIGIN_OPERAND[0] = '\0';
    OPERAND[0] = '\0';
    OPERAND2[0] = '\0';
    OBJECT_CODE[0] = '\0';
}

// char *trim(char *string)
// {
//     char *copy_string;
//     strcpy(copy_string, string);
//     int length = sizeof(copy_string);
//     if (copy_string[length - 1] == '\n')
//     {
//         copy_string[length - 1] == '0';
//         return copy_string;
//     }

//     return string;
// }

/**
 * Debug functions
 */

/**
 * Print current line
 */
void printCurrentLine()
{
    printf("==================\n");
    printf("LABLE: %s\n", LABEL);
    printf("OPCODE: %s\n", OPCODE);
    printf("OPERAND: %s\n", OPERAND);
    // printf("OPERAND2: %s\n", OPERAND2);
    // printf("ORIGIN_OPERAND: %s\n", ORIGIN_OPERAND);
    // printf("LOCCTR: %d\n", LOCCTR);
    // printf("HAS_LABEL: %d\n", HAS_LABEL);
    printf("LOCATION: %s\n", LOCATION);
    printf("OBJECT_CODE: %s\n", OBJECT_CODE);
    printf("==================\n");
}

void printOptab()
{
    char copy_label[10];
    strcpy(copy_label, LABEL);
    int size = sizeof(OPTAB) / sizeof(OPTAB[0]);

    printf("====================Operation table====================\n");
    int i;
    for (i = 0; i < size; i++)
    {
        printf("Mnemonic: %s\t", OPTAB[i]->mnemonic);
        printf("Opcode: %d\t", OPTAB[i]->opcode);
        printf("Length: %d\n", OPTAB[i]->length);
    }
    printf("=======================================================\n");
}
