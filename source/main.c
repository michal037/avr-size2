/*
 * MIT License
 * Copyright (c) 2018, Michal Kozakiewicz, github.com/michal037
 *
 * Standard: GCC-C11
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include "avrNameList.h"

/* Detect the system */
#ifdef _WIN32 /* For Windows 32 and 64 bit */
#define BINARY_PROGRAM_NAME "avr-size2.exe"
#define OS_PROGRAM_EXTENSION ".exe"
#else /* Unix like OS */
#define BINARY_PROGRAM_NAME "avr-size2"
#define OS_PROGRAM_EXTENSION ""
#endif
/* Safe free */
#define safeFree(p) __safeFree((void**)&(p))

typedef unsigned char uint8;
typedef signed int int32;
typedef unsigned int uint32;
typedef char * string;
typedef const char * constString;

void *safeMalloc(size_t size);
void __safeFree(void **ptr);
uint8 checkFile(constString name);
string concatenateStrings(string *dest, uint8 n, ...);
void customError(constString content);
void printHelp(void);
void createBinaryDirPath(string *result, constString progPath);
void setAvrName(constString avrELF);
void setAvrUsage(constString avrELF);
void printAvrUsage(uint8 forPlugin);

string binaryDirPath = NULL; /* Path to program directory */
string avrName = NULL; /* AVR name */
struct
{
    struct { uint32 flash, sram, eeprom; } byte;
    struct { double flash, sram, eeprom; } percent;
} avrUsage = {{0, 0, 0}, {0, 0, 0}}; /* AVR usage */

int main(const int argc, constString argv[])
{
    /* Check program inputs */
    if(argc == 1) printHelp();
    if(argc > 3) customError("Too many inputs");

    /* Check input ELF file */
    if(!checkFile(argv[1])) customError("Input file do not exists");

    /* Create path to directory where program is located */
    createBinaryDirPath(&binaryDirPath, argv[0]);

    /* Get from ELF file AVR name and set it */
    setAvrName(argv[1]);

    /* Get from ELF file AVR usage and set it */
    setAvrUsage(argv[1]);

    /* Print AVR usage */
    if(argc == 2)
        printAvrUsage(0); /* Console use */
    else if(argc == 3)
        printAvrUsage((uint8)(stricmp("plugin", argv[2]) ? 0 :  1)); /* Plugin use */

    /* Free pointers */
    safeFree(binaryDirPath);
    avrName = NULL; /* Pointer from const tab avrNameListA */

    return 0;
} /* main */

/*
 * safeMalloc()
 *
 * Allocate memory.
 *
 * Parameters:
 *   INPUT: size - Size of memory to allocate.
 *
 * Result:
 *   Pointer to memory. If cause error, close program.
 *
 */
void *safeMalloc(size_t size)
{
    void *mem = malloc(size);

    if(mem == NULL) customError("Out of RAM memory");

    return mem;
} /* safeMalloc */

/*
 * __safeFree()
 *
 * Free memory.
 *
 * Parameters:
 *   OUTPUT: ptr - Address of [pointer to memory] to clear.
 *
 * Use with macro:
 *   #define safeFree(p) __safeFree((void**)&(p))
 *
 */
void __safeFree(void **ptr)
{
    if(ptr != NULL && *ptr != NULL)
    {
        free(*ptr);
        *ptr = NULL;
    }
} /* __safeFree */

/*
 * checkFile()
 *
 * Check for file existing.
 *
 * Parameters:
 *   INPUT: name - Path to file.
 *
 * Result:
 *   Check file, if file exist return 1 else 0.
 *
 */
uint8 checkFile(constString name)
{
    struct stat buf;

    return (uint8) !stat(name, &buf);
} /* checkFile */

/*
 * concatenateStrings()
 *
 * Concatenate strings.
 *
 * Parameters:
 *   OUTPUT: dest - address of [pointer to char], where will concatenated string appear.
 *                  if (*dest != NULL) then free(*dest).
 *   INPUT: n - Number of strings to concatenate.
 *   INPUT: ... - String to concatenate.
 *
 * Result:
 *   OUTPUT: *dest - Pointer to concatenated string.
 *
 * REMEMBER TO FREE POINTER OF CONCATENATED STRING !!!
 *
 */
string concatenateStrings(string *dest, uint8 n, ...)
{
    /* Checks for bugs */
    if(n == 0) return NULL;
    if(dest == NULL) return NULL;
    if(*dest != NULL) safeFree(*dest);

    /* VARIABLES */
    va_list strlist;
    uint32 allstrlen = 0; /* Must be set as 0 here */

    /* Calculate strings' length */
    va_start(strlist, n);
    for(int i=0; i<n ;i++) allstrlen += strlen(va_arg(strlist, string));
    va_end(strlist);
    allstrlen += 1; /* Add NULL symbol */

    /* Allocate memory for concatenated string */
    *dest = safeMalloc(allstrlen);
    /* Fill memory with string terminators */
    *dest = memset(*dest, '\0', allstrlen);

    /* Concatenate strings */
    va_start(strlist, n);
    for(int i=0; i<n ;i++) strcat(*dest, va_arg(strlist, string));
    va_end(strlist);

    return *dest; /* REMEMBER TO FREE THIS POINTER !!! */
} /* concatenateStrings */

/*
 * customError()
 *
 * Custom error.
 *
 * Parameters:
 *   INPUT: content - Error information.
 *
 */
void customError(constString content)
{
    fprintf(stderr, "%s: %s!\n", BINARY_PROGRAM_NAME, content);
    exit(EXIT_FAILURE);
} /* customError */

/*
 * printHelp()
 *
 * Print program help.
 *
 */
void printHelp(void)
{
    printf(
            "AVR Size 2 - Print AVR usage\n\n"
            "  %s [file] (optional parameter)\n"
            "  %s avr_app.elf\n"
            "  %s avr_app.elf plugin\n\n"
            "Compilation: %s | %s\n"
            "MIT License | github.com/michal037\n",
            BINARY_PROGRAM_NAME, BINARY_PROGRAM_NAME, BINARY_PROGRAM_NAME, __DATE__, __TIME__
    );
    exit(EXIT_SUCCESS);
} /* printHelp */

/*
 * createBinaryDirPath()
 *
 * Create path to binaries.
 *
 * Parameters:
 *   OUTPUT: result - Pointer to result string.
 *   INPUT: progPath - String to avr-size2 program.
 *
 */
void createBinaryDirPath(string *result, constString progPath)
{
    string temp = NULL;

    /* Copy program path to temp variable and select directory */
    temp = safeMalloc(strlen(progPath) + 1);
    temp[0] = '\0';
    strcpy(temp, progPath);
    temp[strlen(progPath)] = '\0';
    temp = dirname(temp);

    /* Copy directory path to output variable */
    *result = safeMalloc(strlen(temp) + 1);
    (*result)[0] = '\0';
    strcpy(*result, temp);
    (*result)[strlen(temp)] = '\0';

    /* Free unused memory */
    free(temp);
} /* createBinaryDirPath */

/*
 * setAvrName()
 *
 * Set AVR name to global variables.
 *
 * Parameters:
 *   INPUT: avrELF - Path to AVR ELF file.
 *
 */
void setAvrName(constString avrELF)
{
    /* Compare AVR name - function */
    auto constString cmpAvrName(constString inputString);

    FILE *pipeStream1 = NULL; /* Communication with another process */
    string cmdToSend = NULL; /* Command to send */
    int tmpCnt = 0, tmpChar = 0; /* Must be signed integer */
    #define tmpString_size (20) /* Minimum is number of characters of the longest microcontroller's name + 1 */
    char tmpString[tmpString_size];

    /* Check for existing avr-readelf */
    concatenateStrings(&cmdToSend, 3, binaryDirPath, "\\avr-readelf", OS_PROGRAM_EXTENSION);
    if(!checkFile(cmdToSend)) customError("Can not find the avr-readelf");

    /* Create command */
    concatenateStrings(&cmdToSend, 5, binaryDirPath, "\\avr-readelf", OS_PROGRAM_EXTENSION, " --string-dump=.note.gnu.avr.deviceinfo ", avrELF);

    /* If the result pointer indicates something, clear it */
    safeFree(avrName);

    /* Open pipe; Mode: read */
    pipeStream1 = popen(cmdToSend, "r");

    /* Read records and compare */
    while((tmpChar = fgetc(pipeStream1)) != EOF)
    {
        if(tmpChar != ']') continue; /* Skip until it finds ']' char */
        for(uint8 i=1; i<=2 ;i++) fgetc(pipeStream1); /* Shift 2 spaces */
        tmpCnt = 0; /* Reset counter */
        while((tmpChar = fgetc(pipeStream1)) != 0x0A)
        {
            /* Save input and increase counter */
            tmpString[tmpCnt] = (char) tmpChar;
            tmpCnt++;
            /* Counter is too long => bad input => skip */
            if(tmpCnt >= tmpString_size) { tmpCnt = 0; break; }
        }
        tmpString[tmpCnt] = '\0'; /* Add NUL char at the end of string */
        if(tmpCnt <= 3) continue; /* Skip too short strings */
        if((avrName = (string) cmpAvrName(tmpString)) != NULL) break; /* Compare record */
    }

    /* Check result pointer */
    if(avrName == NULL) customError("Can not find the AVR name");

    /* Close pipe */
    if(pclose(pipeStream1) == -1) customError("Pipe stream error");

    /* Free pointer */
    safeFree(cmdToSend);

    /* Compare AVR name */
    constString cmpAvrName(constString inputString)
    {
        for(uint32 i=0; i<avrNameListSize ;i++)
        {
            if(!stricmp(inputString, avrNameList[i])) return avrNameList[i];
        }
        return NULL;
    } /* cmpAvrName */

    /* Undefine tmpString_size */
    #undef tmpString_size
} /* getAvrName */

/*
 * setAvrUsage()
 *
 * Set AVR usage to global variables.
 *
 * Parameters:
 *   INPUT: avrELF - Path to AVR ELF file.
 *
 */
void setAvrUsage(constString avrELF)
{
    /* Tool functions */
    auto void skipToChar(char a);
    auto void copyToChar(char a);

    FILE *pipeStream1 = NULL; /* Communication with another process */
    string cmdToSend = NULL; /* Command to send */
    uint8 cntColons = 0, correctName = 0;
    int32 tmpChar = 0; /* Must be signed integer */
    #define tmpString_size (22) /* Define tmpString_size */
    char tmpString[tmpString_size];
    struct
    {
        uint32 tbyte[3]; double tpercent[3];
    } tmpAvrUsage = {{0, 0, 0}, {0, 0, 0}}; /* Temp AVR usage */

    /* Check for existing avr-size */
    concatenateStrings(&cmdToSend, 3, binaryDirPath, "\\avr-size", OS_PROGRAM_EXTENSION);
    if(!checkFile(cmdToSend)) customError("Can not find the avr-size");

    /* Create command */
    concatenateStrings(&cmdToSend, 7, binaryDirPath, "\\avr-size", OS_PROGRAM_EXTENSION, " -C --mcu=", avrName, " ", avrELF);

    /* Count colons; Mode: read */
    pipeStream1 = popen(cmdToSend, "r");
    while((tmpChar = fgetc(pipeStream1)) != EOF) if(tmpChar == ':') cntColons++;
    if(pclose(pipeStream1) == -1) customError("Pipe stream error");

    /* Open pipe; Mode: read */
    pipeStream1 = popen(cmdToSend, "r");

    /* Read device name */
    skipToChar(':'); /* Skip multiple */
    fgetc(pipeStream1); /* Skip 1 */
    copyToChar(0x0A); /* Copy device name */

    /* Compare device name */
    correctName = (uint8) (strcmp("Unknown", tmpString) ? 1 : 0);

    /* Read usage */
    cntColons--; /* -1 -> Device name colon */

    for(uint8 i=0; i<cntColons ;i++)
    {
        skipToChar(':'); copyToChar('b');
        tmpAvrUsage.tbyte[i] = (uint32) atoi(tmpString);

        if(correctName)
        {
            skipToChar('('); copyToChar('%');
            tmpAvrUsage.tpercent[i] = (double) atof(tmpString);
        }
    }

    /* Assign AVR usage values */
    avrUsage.byte.flash = tmpAvrUsage.tbyte[0];
    avrUsage.byte.sram = tmpAvrUsage.tbyte[1];
    avrUsage.byte.eeprom = tmpAvrUsage.tbyte[2];
    avrUsage.percent.flash = tmpAvrUsage.tpercent[0];
    avrUsage.percent.sram = tmpAvrUsage.tpercent[1];
    avrUsage.percent.eeprom = tmpAvrUsage.tpercent[2];

    /* Close pipe */
    if(pclose(pipeStream1) == -1) customError("Pipe stream error");

    /* Free pointer */
    safeFree(cmdToSend);

    /* Tool functions */
    void skipToChar(char a)
    {
        while( fgetc(pipeStream1)  !=  ((int)a) );
    } /* skipToChar */

    void copyToChar(char a)
    {
        int tmpCnt = 0;
        while((tmpChar = fgetc(pipeStream1)) != ((int)a))
        {
            /* Save input and increase counter */
            tmpString[tmpCnt] = (char) tmpChar;
            tmpCnt++;
            /* Counter is too long => bad input => error */
            if(tmpCnt >= tmpString_size) customError("Input string too long");
        }
        tmpString[tmpCnt] = '\0'; /* Add NUL char at the end of string */
    } /* copyToChar */

    /* Undefine tmpString_size */
    #undef tmpString_size
} /* setAvrUsage */

/*
 * printAvrUsage()
 *
 * Print AVR usage.
 *
 * Parameters:
 *   INPUT: forPlugin - Console use = 0, Plugin use = 1.
 *
 */
void printAvrUsage(uint8 forPlugin)
{
    if(forPlugin == 0)
    {
        uint8 movfl = 0; /* temp */

        /* Test for memory overflow */
        if(avrUsage.percent.flash > 100.0 || avrUsage.percent.sram > 100.0 || avrUsage.percent.eeprom > 100.0)
            movfl = 1;

        /* Print AVR usage */
        printf(
                "device: %s\n"
                "=-----%s-----=\n"
                "flash:  %7u [B] %6.1f %%%s\n"
                "sram:   %7u [B] %6.1f %%%s\n"
                "eeprom: %7u [B] %6.1f %%%s\n",
                avrName, (movfl) ? "|MemoryOverflow|" : "----------------",
                avrUsage.byte.flash, avrUsage.percent.flash, (avrUsage.percent.flash > 100.0) ? " <- Overflow" : "",
                avrUsage.byte.sram, avrUsage.percent.sram, (avrUsage.percent.sram > 100.0) ? " <- Overflow" : "",
                avrUsage.byte.eeprom, avrUsage.percent.eeprom, (avrUsage.percent.eeprom > 100.0) ? " <- Overflow" : ""
        );
    } else if(forPlugin == 1)
    {
        printf(
                "%s;%u;%.1f;%u;%.1f;%u;%.1f;", avrName,
                avrUsage.byte.flash, avrUsage.percent.flash,
                avrUsage.byte.sram, avrUsage.percent.sram,
                avrUsage.byte.eeprom,avrUsage.percent.eeprom
        );
    }
} /* printAvrUsage */
