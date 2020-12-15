import numpy, random
RHO   = 0.1
DELTA = 0.5
R = 1000


V = 5
matrix = [[0 for i in range(V)] for j in range(V)]
permut = list(numpy.random.permutation(V))

alphas = [random.randrange(RHO * R, DELTA * R) for i in range(V)]

for i in range(V-1):
    matrix[permut[i]][permut[i+1]] = alphas[permut[i]] + alphas[permut[i+1]]
    matrix[permut[i+1]][permut[i]] = alphas[permut[i]] + alphas[permut[i+1]]
    #matrix[permut[i+1]][permut[i]] = alphas[i] + alphas[i+1]
    #matrix[permut[i]][permut[i+1]] = alphas[i] + alphas[i+1]

print(permut)
print(alphas)
print("THE MATRIX I:")
for i in matrix:   
    print(i)
for i in range(V):
    for j in range(V):
        if not matrix[i][j] and i!=j:
            temp = random.randrange(alphas[i] + alphas[j], R)
            matrix[i][j] = temp
            matrix[j][i] = temp
optimal_path = matrix[permut[-1]][permut[0]]
for i in range(V-1):
    optimal_path += matrix[permut[i]][permut[i+1]]
print("optimal path: ", optimal_path)
print("THE MATRIX:")
for i in matrix:   
    print(i)

fd = open("graph.txt", "w")
fd.write(str(V) + "\n")
for i in matrix:
    for j in i:
        fd.write(str(j) + "\n")


