input format:
points on a plane one per line coordinates separated by space
empty line
index of point with factory (points indexed from 1)
directed edges one per line as indexes of adjacent vertices separated by space
end of flie

output format:
fence length
routes from factory to points on convex hull one per line as destination index distance vertices forming a route separated by spaces

example:
input:
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
output:
6.47214

2 0 2
3 2.41421 2 1 3
