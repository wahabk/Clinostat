import numpy as np

def belt_length(p1, p2, centre_distance):
    return np.pi * (p1+p2) * 0.5 + (2*centre_distance) + ((p2-p1)**2/(4*centre_distance))


def main():
    b = belt_length(10, 30, 70)
    print(b)
    b = belt_length(10, 30, 70)
    print(b)

if __name__ == "__main__":
    main()