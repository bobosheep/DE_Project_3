# rgrep 
A record search program

## Usage
```bash
$ rgrep [OPTION]... PATTERN FILE/DIR
```
## Options
```bash
Record relatived
    -rb,    	use BEGIN_PATTERN to separate the records in the file
    -fq,    	search PATTERN in the FIELD of arecord
    
Direcotry relatived
    -fn,    	search PATTERN in the name of files in directory
    -r,     	search directory recursively
    
General
    -h, --help  rgrep usage
    -w      	word match
    -mt     	search NUM patterns and patterns should be after NUM
```


## Example

1.  Search the pattern `'hello` in the `@body` field of the records started with `@title` field
    ```bash
    $ rgrep -rb '@title' -fq '@body' hello news.rec
    ```
2.  Search multi-pattern `hello`, `Taiwan`, `Taipei` in the files in current directory
    ```bash
    $ rgrep -mt 3 hello Taiwan Taiepei ./
    ```

## Experimental Environment

```bash
-OS: Windows 10, 64 bit
-CPU: i5-8250U, 1.6~3.4 GHz
-RAM: 8 GB
-DATA: ettoday.rec, 1.88GB
-PROGRAM-LANGUAGE: C
```

## Program Flow

1. **Check configurations:** 

   ​	processing a file or a directory, searching pattern in a file or a record.

   ​	See detail in [Option](##Options) 

2. **Search the pattern:** 

   * File name search in directory (option with `-fn`)

     ​	If the program check there is a `-fn` in the arguments, it will read the file name argument as a directory name. Then, program opens the directory to search for  the files whose name contains the search pattern in the directory (or add -r to read directory recursively). If match, then print the file name.

   * File(s) search (default)

     ​	As default, the program search the pattern in a file read line by line. If a line match the pattern, then print the match informations, included file name, search pattern, the position of found pattern and the line content.

     ​	In the end, the program will print the total match count of searching. 

   * Record search (option with `-rb`)

     ​	If the program check there is a `-rb` in the arguments, it will split the file into records based on the `record begin pattern`. After getting all records, the program will search the pattern in each of records. If a record contains the pattern, then print the match information with contents of that record and total match count in that record. 

     ​	User can add `-fq` to query just specific field not all the record by adding a field name after `-fq`. 

3. **Search method:**

   ​	Use `strstr` as default, also support Boyer-Moore method.

## Problem in Windows

1. Character 0x1A (^Z)
2. fseek

## Future Work

- [x] Demo web
- [ ] add KMP
- [ ] add shift-or
- [ ] add shift-and

## 

