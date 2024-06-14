# Tragarze cli

## Input format
pairs one per line as index of left element and index of right element separated by a space (indexing seperate)\
end of file

## Output format
resulting pairs one per line

## Arguments
first argument is input file, second argument is output file\
instead of filename you can enter `--` to use stdio instead of file

## Example
`in.txt`:
```
1 1
1 2
2 3
3 2
3 3
3 5
3 6
4 4
4 6
5 5
```

`tragarze in.txt out.txt`

`out.txt`:
```
1 1
2 3
3 2
4 4
5 5
```
