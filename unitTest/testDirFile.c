#include <stdio.h>
#include <assert.h>
#include "../rgrep/rgrep.h"

int main()
{
    char * dirName = "../";
    char * pattern = "c";
    char * files[1000];
    int matchCount, fileCount, i;

    /* filename test without recursive mode */
    printf("Name with patter \"%s\"(without recursive):\n", pattern);
    matchCount = getDirFile(dirName, NULL, pattern, 0);
    printf("\n");

    /* filename test with recursive mode */
    printf("Name with patter \"%s\"(with recursive):\n", pattern);
    matchCount = getDirFile(dirName, NULL, pattern, 1);
    printf("\n");

    /* get files */
    printf("Directory files (without recursive):\n", pattern);
    fileCount = getDirFile(dirName, files, NULL, 0);
    for(i = 0 ; i < fileCount ; i++)
    {   
        printf("%s\n", files[i]);
    }

    /* get files recursively */
    printf("Directory files (with recursive):\n", pattern);
    fileCount = getDirFile(dirName, files, NULL, 1);
    for(i = 0 ; i < fileCount ; i++)
    {   
        printf("%s\n", files[i]);
    }


    return 0;
}