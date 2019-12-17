#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "rgrep.h"

int fReadRec(FILE * fptr, const char * recordBegin, REC * records)
{
    const int MAX_RECSIZE = 1 << 17; // at least 1 << 17 QQ
    int status = 1;
    char * buffer = malloc(sizeof(char) * MAX_RECSIZE);
    int nread, recLen, recordCount = 0;
    int patternLen;


    if(fptr == NULL || feof(fptr) || records == NULL)
    {
        status = -1;
        goto exit;
    }


    patternLen = strlen(recordBegin);
    while((nread = fread(buffer, 1, MAX_RECSIZE - 1 , fptr)))
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
                    fseek(fptr, -recLen, SEEK_CUR);
                    break;
                } 
            }
            
            /*  Get a record  */
            if(records[recordCount].content == NULL)
            {
                records[recordCount].content = malloc(sizeof(char) * recLen + 1);
                //printf("recLen: %d\n", recLen);
                assert(records[recordCount].content != NULL);
            }
            memcpy(records[recordCount].content , cur, recLen);
            records[recordCount].content[recLen] = '\0';
            records[recordCount].content_size = recLen;
            //if(recordCount > 155000)
                //printf("[ REC %d ]\n%s", recordCount, records[recordCount].content);

            recordCount++;
            //if(recordCount % 5000 == 0)
                //printf("%d\n", recordCount);
        }

    }

exit:    
    free(buffer);
    return status == -1 ? -1 : recordCount;
  
        
}

//int recSearch(REC  rec, const char * field, const char * pattern)
int recSearch(const recSearchConfig * rec_s_config)
{
    if(rec_s_config == NULL)
        return -1;

    sConfig search_config = rec_s_config->search_config;
    REC rec = rec_s_config->record;
    char *field = rec_s_config->queryfield;
    int matchCount = 0;
    int patternLen = strlen(search_config.pattern);

    char head[128];
    if(search_config.pattern == NULL || rec.content == NULL)
        return -1;

    if(field != NULL)
    {
        /*  Field search    */
        char * fieldPart = strstr(rec.content, field);
        if(!fieldPart)
        {
            return -1;
        }
        //printf("[FIELD] %s\n", fieldPart);
        char * fieldEnd = fieldPart + 1;
        while(fieldEnd != NULL && (*fieldEnd) != FIELD_DEL && (*fieldEnd) != '\0') fieldEnd++;
        if(fieldEnd== NULL)
            return -1;
        *fieldEnd = '\0';

        char * findPattern = fieldPart ;
        search_config.target = findPattern;
        while((findPattern = strSearch(&search_config)) != NULL)
        {
            matchCount++;
            //sprintf(head, "[ MATCH : %s][ REC ][ FIELD: %s ]", search_config.pattern, field);
            //sprintf(detail, "%s\n[ REC Detail ]:\n", fieldPart);
            *fieldEnd = FIELD_DEL;
            
            printMatch(NULL, head, rec.content);
            findPattern = findPattern + patternLen;
            search_config.target = findPattern;
            *fieldEnd = '\0';

        }
        
        *fieldEnd = FIELD_DEL;

    }
    else
    {
        /*  Record search   */
        char * ptr = rec.content;
        search_config.target = ptr;
        while((ptr = strSearch(&search_config)) != NULL)
        {
            matchCount++;
            ptr = ptr + patternLen;
            search_config.target = ptr;
        }
        if(matchCount > 0)
        {
            sprintf(head, "[ MATCH : %s][ REC ]", search_config.pattern);
            printMatch(NULL, head, rec.content);
        }
    }

    return matchCount;
    
}