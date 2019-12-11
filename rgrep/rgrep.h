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
    int8_t wordMatch;
    int multiPatternCount;
    char **multiPattern;
    
} CONFIG;

typedef struct record {
    char * content;
    uint32_t record_id;
    uint32_t content_size;
} REC;

enum search_mode {
    DefaultSearch = 1,
    NError,
    WholeWords,
    WholeLines,
    RegularExpression
};

enum prefix_foramt {
    DefaultPrefix = 1,
    WithoutPrefix,
    MatchCountOnly,
    MatchNameOnly,
    MatchClassOnly
};

enum detail_format {
    DefaultDetail = 1,
    WithoutDetailClass,
    AddFrontLine,
    AddBackLine,
    AddBoth
};

typedef struct output_format {
    enum prefix_foramt prefixFormat;
    enum detail_format detailFormat;
    char * prefix;
    char * detail;
    char * format;

} OutputFormat;

typedef struct search_config {
    enum search_mode mode;
    char * target;
    const char * pattern;
    int nError;

} sConfig;

typedef struct record_searh_config {
    REC record;
    char * queryfield;
    sConfig search_config;

} recSearchConfig;

void getConfig(int argc, char * argv[], CONFIG*);
void showConfig(CONFIG*);
void showUsage();
void freeConfig(CONFIG*);

// return result count
int rgrep(const CONFIG*);

// return file count
int getDirFile(const char * , char ** files, const char *, const int8_t);

// return match count
int fileSearch(const char * fileName, const char * pattern);

int fReadRec(FILE * fptr, const char * recordBegin, REC * records);

int recSearch(const recSearchConfig * rec_s_config);

char * strSearch(const sConfig * s_config);

void printMatch(const char * format, const char * head, const char * detail);

#endif