#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include "rgrep.h"


void getConfig(int argc, char * argv[], CONFIG* config)
{
    int i, j, usage = 0;

    config->searchPattern = NULL;
    config->sFileName = NULL;
    config->recordBegin = NULL;
    config->fieldQuery = NULL;
    config->insensitive = FALSE;
    config->fileNameMode = FALSE;
    config->recursive = FALSE;
    config->multiPatternCount = 0;
    config->multiPattern = NULL;
 
    for(i = 1 ; i < argc  ; i++)
    {
        //printf("[%d] %s\n", i, argv[i]);
        if(strcmp("-i", argv[i]) == 0)
        {
            config->insensitive = TRUE;
        }
        else if (strcmp("-fn", argv[i]) == 0 || strcmp("--file-name", argv[i]) == 0)
        {// search file name
            config->fileNameMode = TRUE;
        }
        else if (strcmp("-r", argv[i]) == 0)
        {// recursive mode
            config->recursive = TRUE;
        }
        else if (strcmp("-rb", argv[i]) == 0 || strcmp("--record-begin", argv[i]) == 0)
        {
            i++;
            config->recordBegin = malloc(strlen(argv[i]) + 1);
            assert(config->recordBegin != NULL);
            strcpy(config->recordBegin, argv[i]);
        }
        else if (strcmp("-fq", argv[i]) == 0 || strcmp("--field-query", argv[i]) == 0)
        {
            i++;
            config->fieldQuery = malloc(strlen(argv[i]) + 1);
            assert(config->fieldQuery != NULL);
            strcpy(config->fieldQuery, argv[i]);
        
        }
        else if (strcmp("-mt", argv[i]) == 0 || strcmp("--multi-pattern", argv[i]) == 0)
        {
            i++;
            config->multiPatternCount = atoi(argv[i]);
            assert(config->multiPatternCount != 0);
            config->multiPattern = malloc(sizeof(char *) * config->multiPatternCount);
            int mc ;
            i++;
            for(mc = 0 ; mc < config->multiPatternCount ; mc++, i++)
            {
                config->multiPattern[mc] = malloc(strlen(argv[i]) + 1);
                assert(config->multiPattern[mc] != NULL);
                strcpy(config->multiPattern[mc] , argv[i]);
            }
        }
        else if (strcmp("-h", argv[i]) == 0)
        {
            usage = 1;
        }
        else if (strcmp("--help", argv[i]) == 0)
        {
            usage = 2;
        }
    }

    if(usage)
    {
        showUsage(usage);
        exit(1);
    }
    else if(argc < 3)
    {
        showUsage(2);
        exit(1);
    }



    if(config->multiPatternCount == 0)
    {
        config->searchPattern = malloc(sizeof(char) * (strlen(argv[argc - 2]) + 1));
        strcpy(config->searchPattern, argv[argc - 2]);
    }

    config->sFileName = malloc(sizeof(char) * (strlen(argv[argc - 1]) + 1));
    strcpy(config->sFileName, argv[argc - 1]);

    if(config->fieldQuery != NULL && config->recordBegin == NULL)
    {
        printf("[ ERROR ] You should give record begin pattern to use field query.\n");
    }
}

void showConfig(CONFIG* config)
{
    printf("[ CONFIG ]\n");
    
    if(config->searchPattern != NULL)
        printf("[ SEARCH PATTERN ] \"%s\"\n", config->searchPattern);
    if(config->sFileName != NULL)
        printf("[ FILE/DIR NAME ] \"%s\"\n", config->sFileName );
    if(config->recordBegin != NULL)
        printf("[ REC BEGIN PETTERN ] \"%s\"\n", config->recordBegin );
    if(config->fieldQuery != NULL)
        printf("[ QUERY FIELD ] \"%s\"\n", config->fieldQuery );

    printf("[ INSENSITIVE ] \"%s\"\n", (config->insensitive ? "true" : "false") );
    printf("[ NAME MODE ] \"%s\"\n", (config->fileNameMode ? "true" : "false") );
    printf("[ RECURSIVE MODE ] \"%s\"\n", (config->recursive ? "true" : "false") );

    if(config->multiPatternCount != 0)
    {
        printf("[ MULTIPATTERN ] \"%s\"\n", "true");
        int i = 0;
        for(i = 0 ; i < config->multiPatternCount ; i++)
        {
            printf("\"%s\" ", config->multiPattern[i]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void showUsage(int isDetail)
{
    if(isDetail == 1)
    {
        printf("Usage: rgrep [OPTION]... PATTERN FILE/DIR\n");
        printf("Search for PATTERN in a FILE, RECORDS or a DIR.\n");
        printf("Example: rgrep -rb '@title' -fq '@body' 'hello world' news.rec\n");

        printf("\nRecord relatived:\n");
        printf("  -rb, --record-begin=PATTERN\tuse PATTERN to separate the records in the file\n");
        printf("  -fq, --field-query=FIELD\tsearch PATTERN in the FIELD of arecord\n");
        printf("\nDirectory relatived:\n");
        printf("  -fn, --file-name\t\tsearch PATTERN in the name of files in directory\n");
        printf("  -r \t\t\t\tsearch directory recursively\n");
        printf("\nGeneral:\n");
        printf("  -h, --help\t\t\trgrep usage\n");
        printf("  -mt, --multi-pattern=NUM \tsearch NUM patterns and patterns should be after NUM\n");
    }
    else
    {
        /* code */
        printf("Usage: rgrep [OPTION]... PATTERN FILE/DIR\nTry 'rgrep --help' for more information.\n");
    }
    
}

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
        if((fileCnt = getDirFile(fName, filesNameInDir, NULL, config->recursive)) > 0)
        {
            /* search the files in the directory */
            if(config->recordBegin != NULL)
            {
                FILE * fptr;
                int i, k, count;
                char ** records = malloc(sizeof(char *) * 1000000);
                for(k = 0 ; k < fileCnt ; k++)
                {/* Open files to do record search*/
                    int c;
                    
                    /*  get records */
                    fptr = fopen(filesNameInDir[k], "r");

                    /* records pattern search */
                    count = fReadRec(fptr, config->recordBegin, records);
                    for(i = 0 ; i < count  ; i++)
                    {
                        c = 0;
                        if(config->multiPatternCount > 0)
                        {
                            int j;
                            for(j = 0 ; j < config->multiPatternCount ; j++)
                            {
                                c += recSearch(records[i], config->fieldQuery, config->multiPattern[j]);
                            }
                        }
                        else
                        {
                            c = recSearch(records[i], config->fieldQuery, config->searchPattern);
                        }
                        if(c > 0)
                        {
                            printf("[ REC ] Total match: %d\n", c);
                        }
                        free(records[i]);
                        records[i] = NULL;

                    }
                    fclose(fptr);
                    free(records);
                }
            }
            else 
            {
                /* Open files to search */
                int i, match = 0;
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
        else if(config->recordBegin != NULL)
        {
            FILE * fptr;
            int i, count;
            int c;
            char ** records = malloc(sizeof(char *) * 830000);

            //printf("Record Search!\n");
                
            /*  get records */
            fptr = fopen(fName, "r");

            /* records pattern search */
            count = fReadRec(fptr, config->recordBegin, records);
            printf("[ FILE %s ]rec count: %d\n", fName, count);

            for(i = 0 ; i < count  ; i++)
            {
                c = 0;
                if(config->multiPatternCount > 0)
                {
                    int j;
                    for(j = 0 ; j < config->multiPatternCount ; j++)
                    {
                        c += recSearch(records[i], config->fieldQuery, config->multiPattern[j]);
                    }
                }
                else
                {
                    c = recSearch(records[i], config->fieldQuery, config->searchPattern);
                }
                if(c > 0)
                {
                    printf("[ REC ABOVE ] Total match: %d\n\n", c);
                }
                free(records[i]);
                records[i] = NULL;

            }
            fclose(fptr);
            free(records);
        }
        else 
        {
            if(config->multiPatternCount > 0)
            {
                int i, c;
                for(i = 0 ; i < config->multiPatternCount ; i++)
                {
                    c = fileSearch(config->sFileName, config->searchPattern);
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
                if(dirName[len - 1] == '/')
                    sprintf(filename, "%s%s", dirName, dent->d_name);
                else
                    sprintf(filename, "%s/%s", dirName, dent->d_name);
                if(pattern != NULL && strSearch(filename, pattern) != NULL)
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

    FILE * fptr = fopen(fileName, "r");
    char * buf;
    int match_count = 0;
    int line = 0;

    if(fileName == NULL || pattern == NULL)
        return -1;

    buf = malloc(sizeof(char) * BUF_SIZE);

    printf("[ FILE SEARCH ]\n[ file ] %s\n[ pattern ] %s\n", fileName, pattern);

    while(fgets(buf, BUF_SIZE, fptr))
    {
        char * ptr = buf;
        char * ptrEnd = buf + strlen(buf) - 1;
        int patternLen = strlen(pattern);
        while((ptr = strSearch(ptr, pattern)) != NULL)
        {
            char * foundPattern = NULL;
            char breakPoint;
            int isBroken = 0;
            char head[128];
            char detail[1 << 12];
            if(foundPattern != buf)
            {
                sprintf(head, "[ MATCH : %s][ FILE:\"%s\" %d:%d ]", pattern, fileName, line, ptr - buf);
                sprintf(detail, "%s", buf);
                //printf("[ MATCH: %s ][ FILE:\"%s\" %d:%d ]: ...%s", pattern, fileName, line, ptr - buf, foundPattern);    
                //if(isBroken)
                //    printf("...\n");
            }
            else
            {
                sprintf(head, "[ MATCH : %s][ FILE:\"%s\" %d:%d ]", pattern, fileName, line, ptr - buf);
                sprintf(detail, "%s", buf);
                //printf("[ MATCH: %s ][ FILE:\"%s\" %d:%d ]: %s", pattern, fileName, line, ptr - buf, foundPattern);    
                //if(isBroken)
                //    printf("...\n");
            }

            printMatch(NULL, head, detail);
            
            if(isBroken)
            {
                *(ptr + patternLen + 20) = breakPoint;
            }
            ptr += patternLen;
            match_count++;
            //if(line == 10000)
               //printf("match count: %d\n", match_count);
        }
        line++;
    }
    fclose(fptr);

    return match_count;
}

int fReadRec(FILE * fptr, const char * recordBegin, char ** records)
{
    const int MAX_RECSIZE = 1 << 17;
    int status = 1;
    char * buffer = malloc(sizeof(char) * MAX_RECSIZE);
    int nread, recLen, recordCount = 0;
    int patternLen;


    if(feof(fptr))
    {
        status = -1;
        goto exit;
    }
    if(records == NULL)
    {
        status = -1;
        goto exit;
    }


    patternLen = strlen(recordBegin);
    while((nread = fread(buffer, 1, MAX_RECSIZE -1 , fptr)))
    {
        buffer[nread] = '\0';

        char *cur, *next;
        

        next = strstr(buffer, recordBegin);

        while(next)
        {
            cur = next;
            next = strstr(cur + patternLen, recordBegin);

            if(next)
                recLen = next - cur;
                
            else
            {
                recLen = (buffer + nread) - cur;
                if(nread >= MAX_RECSIZE - 1)
                {
                    //printf("fseek!\n");
                    fseek(fptr, -recLen-50, SEEK_CUR);
                    break;
                } 
            }
            
            /*  Get a record  */
            if(records[recordCount] == NULL)
            {
                records[recordCount] = malloc(sizeof(char) * recLen + 1);
                assert(records[recordCount] != NULL);
            }
            memcpy(records[recordCount], cur, recLen);
            records[recordCount][recLen] = '\0';
            //printf("[REC]\n%s", record);

            recordCount++;
            //if(recordCount % 5000 == 0)
                //printf("%d\n", recordCount);
        }

    }

exit:    
    free(buffer);
    return status == -1 ? -1 : recordCount;
  
        
}

int recSearch(char * rec, const char * field, const char * pattern)
{
    int matchCount = 0;
    int patternLen = strlen(pattern);

    char head[128];
    if(rec == NULL)
        return -1;

    if(field != NULL)
    {
        /*  Field search    */
        char * fieldPart = strstr(rec, field);
        if(!fieldPart)
        {
            return -1;
        }
        //printf("[FIELD] %s\n", fieldPart);
        char * fieldEnd = fieldPart + 1;
        while((*fieldEnd) != FIELD_DEL && (*fieldEnd) != '\0') fieldEnd++;
        *fieldEnd = '\0';

        char * findPattern = fieldPart ;
        char * detail = malloc(sizeof(char) * (strlen(fieldPart) + strlen(rec) * 1.5));
        if((findPattern = strSearch(findPattern, pattern)) != NULL)
        {
            matchCount++;
            sprintf(head, "[ MATCH : %s][ REC ][ FIELD: %s ]", pattern, field);
            sprintf(detail, "%s\n[REC Detail]:\n", fieldPart);
            *fieldEnd = FIELD_DEL;
            strcat(detail, rec);
            
            printMatch(NULL, head, detail);
            findPattern = findPattern + patternLen;
        }
        
        *fieldEnd = FIELD_DEL;

    }
    else
    {
        /*  Record search   */
        char * ptr = rec;
        while((ptr = strSearch(ptr, pattern)) != NULL)
        {
            matchCount++;
            ptr = ptr + patternLen;
        }
        if(matchCount > 0)
        {
            sprintf(head, "[ MATCH : %s][ REC ]", pattern);
            printMatch(NULL, head, rec);
        }
    }

    return matchCount;
    
}

char * strSearch(const char * str, const char * pattern)
{
    return strstr(str, pattern);
}

void printMatch(const char * format, const char * head, const char * detail)
{
    if(format == NULL)
    {
        if(head != NULL && detail != NULL)
        {
            printf("%s :\n%s\n", head, detail);
        }
    }
    else
    {
        
    }
    
}