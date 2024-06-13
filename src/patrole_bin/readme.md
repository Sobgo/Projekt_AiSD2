# Patrole cli

## Input format

In first line space separated integers representing brightness values of each point in guard's patrol path.\
Then in each line one integer representing maximum amount of points that guard can skip before needing to stop.

## Output format

In each line of output:\
Number of rests the guard needs to take. Then all the points where the guard needs to stop.

## Arguments

First argument is input file, second argument is output file.\
Instead of filename you can enter `--` to use stdio instead of file.\
If parameter `-v` has been given, explanatory information for a human reader will also be printed

## Example

`in.txt`:

```txt
10 20 30 20 10 5
2
4
1

```

`patrole in.txt out.txt -v`

`out.txt`:

```txt
With 2 maximum skips guard takes path: 0 2 3 5 
And needs to rest 2 times at points: 2 3

With 4 maximum skips guard takes path: 0 1 5 
And needs to rest 1 times at points: 2

With 1 maximum skips guard takes path: 0 1 2 3 4 5 
And needs to rest 3 times at points: 3 4 5

```
