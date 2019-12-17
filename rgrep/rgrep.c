#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include "rgrep.h"

int rgrep(const CONFIG* config)
{
    char * filesNameInDir[1024];
    char * fName = config->sFileName;
    char * sPattern = config->searchPattern;
    struct dirent *dent;

    int mode = 0, fileCnt = 0, matchCount = 0;

    if(config->fileNameMode)
    {
        if(config->multiPatternCount > 0)
        {
            int i;
            for(i = 0 ; i < config->multiPatternCount ; i++)
            {
                matchCount +=  getDirFile(fName, NULL, config->multiPattern[i], config->recursive);
            }
        }
        else
        {
            matchCount = getDirFile(fName, NULL, sPattern, config->recursive);
        }
        printf("[ NAME MODE ] Total match count: %d\n", matchCount);
    }

    else
    {
        /* Open directory to do files search */
        if((fileCnt = getDirFile(fName, filesNameInDir, NULL, config->recursive)) > 0)
        {

            /* search the files in the directory */
            if(config->recordBegin != NULL)
            {
                FILE * fptr;
                int i, k, count;
                REC * records = malloc(sizeof(REC) * 830000);
                
                /* Open files to do record search*/
                for(k = 0 ; k < fileCnt ; k++)
                {
                    int c;
                    recSearchConfig recConfig;
                    
                    /*  get records */
                    fptr = fopen(filesNameInDir[k], "rb");

                    /* records pattern search */
                    count = fReadRec(fptr, config->recordBegin, records);
                    for(i = 0 ; i < count  ; i++)
                    {
                        c = 0;
                        recConfig.record = records[i];
                        recConfig.queryfield = config->fieldQuery;
                        recConfig.search_config.mode = DefaultDetail;
                        recConfig.search_config.nError = 0;
                        
                        if(config->multiPatternCount > 0)
                        {
                            int j;
                            for(j = 0 ; j < config->multiPatternCount ; j++)
                            {
                                recConfig.search_config.pattern =  config->multiPattern[j];
                                c += recSearch(&recConfig);
                            }
                        }
                        else
                        {
                            recConfig.search_config.pattern =  config->searchPattern;
                            c = recSearch(&recConfig);
                        }
                        if(c > 0)
                        {
                            printf("[ REC ] Total match: %d\n", c);
                        }
                        free(records[i].content);


                    }
                    fclose(fptr);
                }
                free(records);
            }
            else 
            {
                int i, match = 0;

                /* Open files to normal search */
                for(i = 0 ; i < fileCnt ; i++)
                {
                    if(config->multiPatternCount > 0)
                    {
                        int j;
                        for(j = 0 ; j < config->multiPatternCount ; j++)
                        {
                            match += fileSearch(filesNameInDir[i], config->multiPattern[j]);
                        }
                    }
                    else
                    {
                        match = fileSearch(filesNameInDir[i], sPattern);
                    }
                    printf("[ FILE %s ] Total match: %d\n", filesNameInDir[i], match);
                }
            }
        }
        /* Open file to do record search */
        else if(config->recordBegin != NULL)
        {
            FILE * fptr;
            int i, count;
            int c;
            REC * records = malloc(sizeof(REC) * 830000);
            recSearchConfig recConfig;
                
            /*  get records */
            fptr = fopen(fName, "rb");

            /* records pattern search */
            count = fReadRec(fptr, config->recordBegin, records);
            //printf("[ FILE %s ] rec count: %d\n", fName, count);

            for(i = 0 ; i < count  ; i++)
            {
                c = 0;
                recConfig.record = records[i];
                recConfig.queryfield = config->fieldQuery;
                recConfig.search_config.mode = DefaultDetail;
                recConfig.search_config.nError = 0;

                if(config->multiPatternCount > 0)
                {
                    int j, tmp;
                    for(j = 0 ; j < config->multiPatternCount ; j++)
                    {
                        recConfig.search_config.pattern =  config->multiPattern[j];
                        tmp = recSearch(&recConfig);
                        if(tmp > 0)
                            c += tmp;
                    }
                }
                else
                {
                    recConfig.search_config.pattern = config->searchPattern;
                    c = recSearch(&recConfig);
                }
                if(c > 0)
                {
                    printf("[ REC ABOVE ] Total match: %d\n\n", c);
                }
                free(records[i].content);
                records[i].content_size = 0;

            }
            fclose(fptr);
            free(records);
        }
        /* Open file to do file search */
        else 
        {
            if(config->multiPatternCount > 0)
            {
                int i, c;
                for(i = 0 ; i < config->multiPatternCount ; i++)
                {
                    c = fileSearch(config->sFileName, config->multiPattern[i]);
                    if(c != -1)
                        matchCount += c;
                    else 
                    {
                        matchCount = -1;
                        printf("[ ERROR ] Searching failed!\n");
                        break;
                    }

                }
                if(matchCount >= 0)
                    printf("[ FILE %s ][ TOTAL MATCH ]: %d\n", fName, matchCount);
            }
            else
            { 
                matchCount = fileSearch(config->sFileName, config->searchPattern);
                if(matchCount != -1)
                    printf("[ FILE %s ][ TOTAL MATCH ]: %d\n", config->sFileName, matchCount);
                else 
                {
                    matchCount = -1;
                    printf("[ ERROR ] Searching failed!\n");
                }
            }
        }

    }

}

/**
 * getDirFile function will get files from the directory recursively.
 * 
 * param: [dirName] the directory name 
 *        [files]  get the list of files name in the directory, set NULL to use filename search.
 *        [pattern] the search pattern of filename, set NULL to get files recusively.
 *        [recursive] the mode to get directory recursively or not.
 * 
 * return: if [files] is not  NULL then return file count.
 *         else if [files] is NULL and pattern is not NULL, then return match count.
 *         else return error(-1). 
 * */
int getDirFile(const char * dirName, char ** files, const char * pattern, const int8_t recursive)
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
                int len = strlen(dirName);
                sConfig search_config;

                search_config.mode = DefaultSearch;
                search_config.pattern = pattern;
                search_config.target = filename;
                search_config.nError = 0;

                if(dirName[len - 1] == '/')
                    sprintf(filename, "%s%s", dirName, dent->d_name);
                else
                    sprintf(filename, "%s/%s", dirName, dent->d_name);

                if(pattern != NULL && strSearch(&search_config) != NULL)
                {
                    matchCount++;
                    printf(filename);
                    printf("\n");
                }
                if(dent->d_type & (1 << 4))
                {
                    //it is a directory
                    if(recursive)
                    {
                        if(files != NULL)
                            fileCount += getDirFile(filename, files + fileCount, pattern, recursive);
                        else if(pattern != NULL)
                            matchCount += getDirFile(filename, files, pattern, recursive);

                    }
                }
                else
                {
                    //if not, it is a file
                    if(files != NULL)
                    {
                        //*(files + fileCount) = fopen(filename, "r");
                        files[fileCount] = malloc(sizeof(filename) + 1);
                        strcpy(files[fileCount], filename);
                    }
                    fileCount++;
                }
                
            }
        }
        closedir(dptr);
    }
    return files == NULL ? (pattern == NULL ? -1 : matchCount) : fileCount;
}



/**
 * fileSearch function will return the count of match of the pattern in the file..
 * 
 * param: [fileName] the file name 
 *        [pattern] the search pattern.
 * 
 * return: match count
 * 
 * */
int fileSearch(const char * fileName, const char * pattern)
{
    const int BUF_SIZE = 1 << 16;

    FILE * fptr = fopen(fileName, "rb");
    char * buf;
    int match_count = 0;
    int line = 0;

    //printf("[ FILE SEARCH ]\n[ file ] %s\n[ pattern ] %s\n", fileName, pattern);
    if(fileName == NULL || pattern == NULL)
        return -1;

    buf = malloc(sizeof(char) * BUF_SIZE + 1);


    while(fgets(buf, BUF_SIZE, fptr))
    {
        char * ptr = buf;
        int patternLen = strlen(pattern);
        char head[128];
        char detail[BUF_SIZE + 1];
        sConfig search_config;

        search_config.mode = DefaultSearch;
        search_config.target = ptr;
        search_config.pattern = pattern;
        search_config.nError = 0;

        while((ptr = strSearch(&search_config)) != NULL)
        {            
            //memset(head, '\0', 128);
            //memset(detail, '\0', 1 << 15);
            
            ptr += patternLen;
            search_config.target = ptr;
            match_count++;
            //if(line == 10000)
               //printf("match count: %d\n", match_count);
        }
        //sprintf(head, "[ MATCH : %s][ FILE:\"%s\" %d:%d ]", pattern, fileName, line, ptr - buf);
        // sprintf(detail, "%s", buf);
        if(match_count > 0)
            printMatch(NULL, head, buf);
        line++;
    }
    fclose(fptr);
    free(buf);

    return match_count;
}



void printMatch(const char * format, const char * head, const char * detail)
{
    if(format == NULL)
    {
        if(head != NULL && detail != NULL)
        {
            printf("%s\n", detail);
            //printf("%s :\n%s\n", head, detail);
        }
    }
    else
    {
        
    }
    
}