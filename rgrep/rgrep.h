#ifndef RGREP_H
#define RGREP_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define TRUE 1
#define FALSE 0

#define DIR_MODE 1
#define FILE_MODE 1<<1



typedef struct config {
    char * searchPattern;
    char * sFileName;
    char * recordBegin;    // -rb record_begin pattern
    char * fieldQuery;     // -field-query use for field query
    int8_t insensitive;       // true or false, default is false
    int8_t fileNameMode;       // true or false, default is false
    int8_t recursive;       // true or false, default is false
    int multiPatternCount;
    char **multiPattern;
    
} CONFIG;

void getConfig(int argc, char * argv[], CONFIG*);

// return result count
int rgrep(const CONFIG*);

// return file count
int getDirFile(const char * , FILE ** files, const char *);

// return match count
int fileSearch(const char * fileName, const char * pattern);

char * strSearch(const char * target, const char * pattern);


#endif