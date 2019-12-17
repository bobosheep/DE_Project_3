#ifndef _STR_MATCH_H
#define _STR_MATCH_H

#include <stdio.h>
#include <string.h>

char * boyerMoore(const char * target, const char * pattern);
void BM_init(const unsigned char * str, int * shift);
char * BM_exec(const char * target, const char * pattern, int * shiftV);

char * strhe(const char * str, const char * pattern);
char * strchr4(const char * , int);


#endif