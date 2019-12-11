# rgrep 
A record search program

## Usage
```bash
$ rgrep [OPTION]... PATTERN FILE/DIR
```
## Options
```
Record relatived
    -rb,    use BEGIN_PATTERN to separate the records in the file
    -fq,    search PATTERN in the FIELD of arecord
Direcotry relatived
    -fn,    search PATTERN in the name of files in directory
    -r,     search directory recursively
General
    -h      rgrep usage
    -w      word match
    -mt     search NUM patterns and patterns should be after NUM
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

