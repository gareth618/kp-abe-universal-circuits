import numpy as np
from galois import GF

p = 7
gf = GF(p)

M = gf([
  [2 ** 0 % p, 2 ** 1 % p, 2 ** 2 % p, 2 ** 3 % p],
  [4 ** 0 % p, 4 ** 1 % p, 4 ** 2 % p, 4 ** 3 % p],
  [5 ** 0 % p, 5 ** 1 % p, 5 ** 2 % p, 5 ** 3 % p],
  [6 ** 0 % p, 6 ** 1 % p, 6 ** 2 % p, 6 ** 3 % p],
])

v = gf([1, 1, 1, 1])

print(v @ np.linalg.inv(M))
