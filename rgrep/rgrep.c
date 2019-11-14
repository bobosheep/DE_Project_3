#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include "rgrep.h"


void getConfig(int argc, char * argv[], CONFIG* config)
{
    int i, j;

    config->searchPattern = NULL;
    config->sFileName = NULL;
    config->recordBegin = NULL;
    config->fieldQuery = NULL;
    config->insensitive = FALSE;
    config->fileNameMode = FALSE;
    config->recursive = FALSE;
    config->multiPatternCount = 0;
    config->multiPattern = NULL;
    
    if(argc < 3)
    {
        printf("Configuration error!");
        exit(1);
    }

    //printf("%d\n", argc);
    printf("Configs:\n");
    for(i = 1 ; i < argc - 2 ; i++)
    {
        //printf("[%d] %s\n", i, argv[i]);
        if(strcmp("-i", argv[i]) == 0)
        {
            config->insensitive = TRUE;
        }
        else if (strcmp("-fn", argv[i]) == 0)
        {// search file name
            config->fileNameMode = TRUE;
        }
        else if (strcmp("-r", argv[i]) == 0)
        {// recursive mode
            config->recursive = TRUE;
        }
        else if (strcmp("-rb", argv[i]) == 0)
        {
            i++;
            config->recordBegin = malloc(strlen(argv[i]) + 1);
            assert(config->recordBegin != NULL);
            strcpy(config->recordBegin, argv[i]);
        }
        else if (strcmp("-fq", argv[i]) == 0)
        {
            i++;
            config->fieldQuery = malloc(strlen(argv[i]) + 1);
            assert(config->fieldQuery != NULL);
            strcpy(config->fieldQuery, argv[i]);
        
        }
        else if (strcmp("-mt", argv[i]) == 0)
        {
        }
        //printf("[%d] %s\n", i, argv[i]);
    }

    config->searchPattern = malloc(sizeof(strlen(argv[argc - 2]) + 1));
    strcpy(config->searchPattern, argv[argc - 2]);

    config->sFileName = malloc(sizeof(strlen(argv[argc - 1]) + 1));
    strcpy(config->sFileName, argv[argc - 1]);

}


int rgrep(const CONFIG* config)
{
    FILE* fsptr[1005];
    char *fName = config->sFileName;
    char *sPattern = config->searchPattern;
    struct dirent *dent;

    int mode = 0, fileCnt = 0, matchCount = 0;

    if(config->fileNameMode)
    {
        if(config->recursive)
            matchCount = getDirFile(fName, NULL, sPattern);
        else
        {
            /* code */
        }
        
    }

    else
    {
        if((fileCnt = getDirFile(fName, fsptr, NULL)) > 0)
            mode |= DIR_MODE;
        else if((matchCount = fileSearch(fName, sPattern)) != -1)
        {
            fileCnt = 1;
            mode |= FILE_MODE;
        }
        else
        {
            printf("Error!");
        }

    }

    



    printf("Files: %d\n", fileCnt);
    printf("Total match: %d\n", matchCount);

}

/**
 * getDirFile function will get files from the directory recursively.
 * 
 * param: [dirName] the directory name 
 *        [files]  the file pointer of a file pointer, set NULL to use filename search.
 *        [pattern] the search pattern of filename, set NULL to get files recusively.
 * 
 * return: if [files] is not  NULL then return file count.
 *         else if [files] is NULL and pattern is not NULL, then return match count.
 *         else return error(-1). 
 * */

int getDirFile(const char * dirName, FILE ** files, const char * pattern)
{
    DIR * dptr = opendir(dirName);
    struct dirent *dent;
    int matchCount = 0, fileCount = 0;

    if(dptr != NULL)
    {
        while((dent = readdir(dptr)) != NULL)
        {
            if((strcmp(dent->d_name,".")==0 || strcmp(dent->d_name,"..")==0 || (*dent->d_name) == '.' ))
            {
            }
            else
            {
                char filename[FILENAME_MAX];
                sprintf(filename, "%s/%s", dirName, dent->d_name);
                if(pattern != NULL && strSearch(filename, pattern) != NULL)
                {
                    matchCount++;
                    printf(filename);
                    printf("\n");
                }
                if(dent->d_type & (1 << 4))
                {
                    //if it is a directory
                    if(files != NULL)
                        fileCount += getDirFile(filename, files, pattern);
                    else if(pattern != NULL)
                        matchCount += getDirFile(filename, files, pattern);
                }
                else
                {
                    //if not, it is a file
                    if(files != NULL)
                    {

                        *(files + fileCount) = fopen(filename, "r");
                    }
                    fileCount++;
                }
                
            }
        }
        closedir(dptr);
    }
    return files == NULL ? (pattern == NULL ? -1 : matchCount) : fileCount;
}

int fileSearch(const char * fileName, const char * pattern)
{
    const int BUF_SIZE = 1 << 14;

    FILE * fptr = fopen(fileName, "r");
    char * buf;
    int match_count = 0;
    int line = 0;

    buf = malloc(sizeof(char) * BUF_SIZE);

    while(fgets(buf, BUF_SIZE, fptr))
    {
        if(strSearch(buf, pattern) != NULL)
        {
            match_count++;
            printf("[%s %d]:%s", fileName, line, buf);
        }
        line++;
    }
    fclose(fptr);

    return match_count;
}

char* strSearch(const char * target, const char * pattern)
{
    return strstr(target, pattern);
}