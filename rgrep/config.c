#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "rgrep.h"


void getConfig(int argc, char * argv[], CONFIG* config)
{
    int i, j, usage = 0;

    if(config == NULL)
        return ;
        //config = malloc(sizeof(CONFIG));

    config->searchPattern = NULL;
    config->sFileName = NULL;
    config->recordBegin = NULL;
    config->fieldQuery = NULL;
    config->insensitive = FALSE;
    config->fileNameMode = FALSE;
    config->recursive = FALSE;
    config->wordMatch = FALSE;
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
        else if (strcmp("-w", argv[i]) == 0)
        {// word match mode
            config->wordMatch = TRUE;
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
                config->multiPattern[mc] = malloc(strlen(argv[i]) + 3);
                assert(config->multiPattern[mc] != NULL);
                if( config->wordMatch )
                    sprintf(config->multiPattern[mc], " %s ", argv[i]);
                else
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
        showUsage(1);
        exit(1);
    }



    if(config->multiPatternCount == 0)
    {
        config->searchPattern = malloc(sizeof(char) * (strlen(argv[argc - 2]) + 3));
        if( config->wordMatch )
            sprintf(config->searchPattern, " %s ", argv[argc - 2]);
        else
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
    if(isDetail == 2)
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

void freeConfig(CONFIG* config)
{
    if(config == NULL)
        return;
    if(config->fieldQuery != NULL)
        free(config->fieldQuery);
    if(config->multiPattern != NULL)
    {
        int i;
        for(i = 0 ; i < config->multiPatternCount ; i++)
            free(config->multiPattern[i]);
    }
    if(config->recordBegin != NULL)
        free(config->recordBegin);
    if(config->searchPattern != NULL)
        free(config->searchPattern);
    if(config->sFileName != NULL)
        free(config->sFileName);
    free(config);
}