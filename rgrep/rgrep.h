#ifndef RGREP_H
#define RGREP_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define TRUE 1
#define FALSE 0

#define MAX_RECORD_SIZE 1 << 15
#define DIR_MODE 1
#define FILE_MODE 1<<1
#define FIELD_DEL '@'



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

typedef struct record {
    char * content;
    uint32_t content_size;
} rec;

void getConfig(int argc, char * argv[], CONFIG*);
void showConfig(CONFIG*);
void showUsage();

// return result count
int rgrep(const CONFIG*);

// return file count
int getDirFile(const char * , char ** files, const char *, const int8_t);

// return match count
int fileSearch(const char * fileName, const char * pattern);

int fReadRec(FILE * fptr, const char * recordBegin, char ** records);

int recSearch( char * record, const char * field, const char * pattern);

char * strSearch(const char * target, const char * pattern);

void printMatch(const char * format, const char * head, const char * detail);

#endif