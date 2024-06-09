# Input format
points on a plane one per line coordinates separated by space
index of point with factory (points indexed from 1)
directed edges one per line as indexes of adjacent vertices separated by space
end of file

# Output format
fence length
routes from factory to points on convex hull one per line as destination index distance vertices forming a route separated by spaces

if parameter `-v` has been given explanatory information for a human readerwill be also printed

# Example
`in.txt`:
```
0 0
0 -1
-1 1
1 1
0 1
2
2 1
1 3
3 5
5 3
4 5
```

`fence_logistics in.txt out.txt -v`

`out.txt`:
```
Fence length: 6.47214
Routes:
destination: 2, length: 0, path: 2
destination: 3, length: 2.41421, path: 2 1 3
```
