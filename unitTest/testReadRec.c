#include <stdio.h>
#include <stdlib.h>
#include "../rgrep/rgrep.h"

#define REC_SIZE 1 << 18

int main()
{
    //"../../DE_Project_2/pettoday.rec"
    //"../test1.rec"

    FILE * fptr = fopen("../../DE_Project_2/pettoday.rec", "r");
    char record[REC_SIZE];
    int status, count = 0;
    char ** records = malloc(sizeof(char *) * 1000000);
    int i ;

    count = fReadRec(fptr, "@Gais", records);
    printf("%d\n", count);
    for(i = 0 ; i < count  ; i++)
    {
        int c;
        //printf("%s\n",records[i]);
        c = recSearch(records[i], NULL, "周幼婷");
        free(records[i]);
        records[i] = NULL;

    }
    free(records);
    fclose(fptr);

    return 0;
}