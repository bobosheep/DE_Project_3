
#include <stdio.h>
#include <string.h>
#include "strmat.h"

void BM_init(const unsigned char * str, int * shiftV)
{
    int i, len = strlen(str);
    const unsigned char * ptr = str;
    const unsigned char * ptrEnd = str + len;
    int shiftLen = len - 1;
    for(i = 0 ; i < 260 ; i++) *(shiftV + i) = len;
    while(ptr < ptrEnd - 1)
    {
        shiftV[*ptr] = shiftLen - (ptr - str);
        //printf("%c: %d\n", *((char*) ptr), shiftV[*((unsigned char*) ptr)]);
        ptr++;
    }

}

char * BM_exec(const char * target, const char * pattern, int * shiftV)
{
    const unsigned char * ptrEnd = (const unsigned char *)target + strlen(target);
    int plen = strlen(pattern);
    const unsigned char * ptr = (const unsigned char *)target + plen - 1;
    const unsigned char lastChar = *(pattern + plen - 1);
    int shiftv;

    //printf("plen: %d\n", plen);
    //printf("start ptr: %s\n", ptr);
    while(ptr < ptrEnd)
    {
        while(ptr < ptrEnd && *ptr != lastChar ) 
        {
            shiftv = shiftV[*ptr];
            //printf("ptr: %04p, value: %c, shiftv: %d\n", ptr, *((unsigned char *) ptr), shiftv);
            ptr += shiftv;
        }
        
        //printf("match last word: %s\n", ptr - plen + 1);
        if(strncmp(ptr - plen + 1, pattern , plen) == 0)
            return ptr - plen + 1;
        else ptr += shiftV[*ptr];
    }
    return NULL;
}

char * boyerMoore(const char * target, const char * pattern)
{
    int shiftV[260];
    BM_init(pattern, shiftV);
    return BM_exec(target, pattern, shiftV);
}
