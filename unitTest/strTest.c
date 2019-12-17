#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../strmat/strmat.h"

#define FILE_TEST 0
#define PRINT_DIFF 0
#define TARGET T1
#define PATTERN P1

#define T "選前倒數2天，你還同時擁有著國立臺南大學校長的身分，我們看見了你為選戰打造了許多有創意的競選廣告、四處到廟口演講拉票，但我們看不到的是，臺南大學這幾個月來的校務是否有因此而延誤停擺？ 舉國立中正大學在太陽花學運之後在校園內立碑的部分碑文上來說，「大學為知識殿堂，探尋真理，沒有包袱，亦無所畏"
#define P "選前倒數2天"
#define T1 "HERE IS A SIMPLE EXAMPLE, WHICH CONTAINS MULTIPLE EXAMPLES. SIXLEE IS A WRONG WORD. EXAMPLEEXAMPLE"
#define P1 "EXAMPLE"
#define SIZE 1 << 17

int main()
{
    FILE * fptr = fopen("../../DE_Project_2/pettoday.rec", "r");
    char buffer[SIZE];

    int shiftV[260];
    //BM_init(P, shiftV);
#if FILE_TEST == 0
    char * ptr = strhe(TARGET, PATTERN);
    printf("%s\n", ptr);
    ///*
#else
    while(fgets(buffer, SIZE, fptr))
    {
        //char * bmptr = boyerMoore(buffer, P);
        //char * bmptr = BM_exec(buffer, P, shiftV);
        char * strptr = strstr(buffer, P);
        char * ptr = strhe(buffer, P);

        ///*
#if PRINT_DIFF == 1
        if(strptr)
        {
            printf("buffer: %s", buffer);
            printf("ptr: %s\n", ptr);
            printf("strstr: %s\n", strptr);
        }
#endif
        //*/
        assert(ptr == strptr);
    }
#endif
    //*/
    //BM_exec(T, P, shiftV);


    return 0;
}