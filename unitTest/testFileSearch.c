#include <stdio.h>
#include <assert.h>
#include "../rgrep/rgrep.h"

int main()
{
    char * fname = "../test.rec";
    char * pattern1 = "12";
    char * pattern2 = "22222";
    char * pattern3 = "\"\"";

    int match;
/*
    match = fileSearch(fname, pattern1);
    match = fileSearch(fname, pattern2);
    match = fileSearch(fname, pattern3);

    
    fname = "../rgrep/rgrep.c";
    pattern1 = "12";
    pattern2 = "ab";
    pattern3 = "one";

    match = fileSearch(fname, pattern1);
    match = fileSearch(fname, pattern2);
    match = fileSearch(fname, pattern3);
  */  
    fname = "../DE_Project_2/pettoday.rec";
    pattern1 = "周幼婷";
    pattern2 = "爽";
    pattern3 = "中正大學";

    match = fileSearch(fname, pattern1);
    match = fileSearch(fname, pattern2);
    match = fileSearch(fname, pattern3);
    return 0;
}