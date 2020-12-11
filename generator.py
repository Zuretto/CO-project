import numpy, random
RHO = 0.2
DELTA = 0.5
R = 1000


V = 10
matrix = [[0 for i in range(V)] for j in range(V)]
permut = list(numpy.random.permutation(V))
alphas = [random.randrange(RHO * R, DELTA * R) for i in range(V)]

for i in range(V-1):
    matrix[permut[i]][permut[i+1]] = alphas[i] + alphas[i+1]
    matrix[permut[i+1]][permut[i]] = alphas[i] + alphas[i+1]
print(permut)
print(alphas)
for i in range(V):
    for j in range(V):
        if not matrix[i][j] and i!=j:
            matrix[i][j] = random.randrange(alphas[i] + alphas[j], R)
optimal_path = 0
for i in range(V-1):
    optimal_path += alphas[i] + alphas[i+1]
optimal_path += alphas[0] + alphas[-1]
print("optimal path: ", optimal_path)
print("THE MATRIX:")
for i in matrix:   
    print(i)



