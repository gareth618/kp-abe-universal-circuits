import numpy as np
from galois import GF

with open('temp.txt', 'r') as file:
  lines = file.read().split('\n')[:-1]

[n, p] = [int(token) for token in lines[0].split(' ')]
gf = GF(p)
matrix = gf([[int(token) for token in line.split(' ')] for line in lines[1:]])
objective = gf([1] * n)
solution = objective @ np.linalg.inv(matrix)

with open('temp.txt', 'w') as file:
  file.write(' '.join(str(value) for value in solution) + '\n')
