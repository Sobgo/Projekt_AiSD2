# Zapis melodii cli

## Zapisywanie

### Input format
In replacements file one per line pair of word to replace and word to replace it with of the same length\
Input is the ASCII text to compress

### Output format
Output is a sequence of characters `1` and `0` representing bits of compressed text

### Arguments
First argument is the replacements file, second argument is theinput file, third argument is the output file.\
Instead of filename you can enter `--` to use stdio instead of file.

### Example

`replacements.txt`
```
poli boli
aaa bbb
```

`in.txt`
```
wszystko mnie poli aaa
```

`zapis_melodii_write replacements.txt in.txt out.txt`

`out.txt`
```
0000111000000100000000000010110001000001100010110010111010011101011110110011011011101110110111111100111110100111011111110011111010110110111111111010111100011010100001000100101010100000001101001110101000001001001
```

## Odczytywanie

### Input format
Input is a sequence of characters `1` and `0` representing bits of compressed text

### Output format
Output is the decompressed ASCII text

### Arguments
First  argument is theinput file, second argument is the output file.\
Instead of filename you can enter `--` to use stdio instead of file.

### Example

`in.txt`
```
000011110000010100000001001001001000110001001000001110011110100111010111101111111101000010101100101110110011011011101110111010011101111111001111100111011111110111110110011010010110111110010001100101000101011010100001000000011000
```

`zapis_melodii_read in.txt out.txt`

`out.txt`
```
wszystko mnie boli bbb
```
