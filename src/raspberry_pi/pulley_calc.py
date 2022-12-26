import numpy as np

def belt_length(p1, p2, centre_distance):
    return np.pi * (p1+p2) * 0.5 + (2*centre_distance) + ((p2-p1)**2/(4*centre_distance))

b = belt_length(10, 30, 70)

print(b)
print(30/17) # 1.75

b = belt_length(10, 30, 70)

print(b)