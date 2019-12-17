#include <stdio.h>
#include <string.h>
int main(int argc, char ** argv)
{
    //SetConsoleOutputCP(65001);
    FILE *fp = fopen("FQ.txt","w");
    char * p = "中正";
    int i;
    printf("😀👙\n");
    printf("%s\n%s\n", argv[1], p);
    if(strncmp(argv[1], p, strlen(p)) != 0)
    {
        printf("not equal\n");
        int len = strlen(argv[1]);
        for(i = 0 ; i < len ; i++)
            printf("%d ", argv[1][i]);
        printf("\n");
        len = strlen(p);
        for(i = 0 ; i < len ; i++)
            printf("%d ", p[i]);
        printf("\n");
    }
    for(i = 0 ; i < argc ; i++)
    {
        printf("%s\n", argv[i]);
    }

    fclose(fp);
    return 0;
}