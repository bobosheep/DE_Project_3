#include "strmat.h"
#include <string.h>
#include <stdlib.h>

char * strhe(const char * str, const char * pattern)
{
    const char * ptrChar = NULL, * pNext = NULL;
    const char * strEnd = (str + strlen(str)); 
    const int ptLen = strlen(pattern);
    const int headC = *((int *)pattern), tailC = *(pattern + ptLen - 1);

    int hCount = 0;
    
    ptrChar = strchr4(str, headC);
    while(ptrChar)
    {
        pNext = ptrChar + ptLen - 1; 
        if(pNext < strEnd && *pNext == tailC)
        {
            //printf("%s\n", ptrChar);
            if( strncmp(ptrChar + 4, pattern + 4, ptLen - 1) == 0)
            {
                return (char *)ptrChar;
            } 
            //headCharPos[hCount++] = ptrChar - str;
            //printf("head: %d\n", headCharPos[hCount - 1]);  
        }
        
        ptrChar = strchr4(ptrChar + 1, headC);           
    }
    return NULL;


}

char * strchr4(const char * str, int ch)
{
    const int len = strlen(str);
    const int * ptrEnd = (const int *) (str + len);
    const char * ptr = str;

    for(;; ++ptr)
    {
        if( *(int *)ptr == ch)
        {
            //printf("%x %x %x %x\n", *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3));
            //printf("%x\n", ch);
            return (char *) ptr;
        }
        else if(*ptr == '\0')
            return NULL;
    }
}