import numpy as np
import matplotlib.pyplot as plt

# prolate if gamma > 1, oblate if gamma < 1
def normalize_to_spheroid(p, gamma):
    x, y, z = p
    scale = np.sqrt(x**2 + y**2 + (z / gamma)**2)
    return np.array([x, y, z]) / scale

def random_tangent_vector(p, gamma):
    n = np.array([p[0], p[1], p[2] / gamma**2])
    n = n / np.linalg.norm(n)
    v = np.random.normal(size=3)
    v -= np.dot(v, n) * n
    return v / np.linalg.norm(v)

def geodesic_random_walk(gamma, n_steps=50, step_size=0.5):
    p = np.array([1.0, 0.0, 0.0])
    path = np.zeros((n_steps + 1, 3))
    path[0] = p
    for i in range(1, n_steps + 1):
        v = random_tangent_vector(p, gamma)
        p = normalize_to_spheroid(p + step_size * v, gamma)
        path[i] = p
    return path

def smooth_geodesic_random_walk(
    gamma,
    n_steps=500,
    step_size=0.1,
    noise_strength=0.1,
):
    """
    Geodesic random walk with directional persistence.

    noise_strength → 0	Nearly geodesic curves
    noise_strength ≈ 0.05	Smooth wandering
    noise_strength ≥ 0.2	Rough, Brownian-like
    """
    # Initial point and velocity
    p = np.array([1.0, 0.0, 0.0])
    v = random_tangent_vector(p, gamma)

    path = np.zeros((n_steps + 1, 3))
    path[0] = p

    for i in range(1, n_steps + 1):
        # Move along current direction
        p_new = normalize_to_spheroid(p + step_size * v, gamma)

        # Surface normal at new point
        n = np.array([p_new[0], p_new[1], p_new[2] / gamma**2])
        n /= np.linalg.norm(n)

        # Parallel-transport approximation:
        # project velocity into new tangent plane
        v -= np.dot(v, n) * n

        # Add small isotropic noise
        eta = random_tangent_vector(p_new, gamma)
        v = (1 - noise_strength) * v + noise_strength * eta
        v /= np.linalg.norm(v)

        p = p_new
        path[i] = p

    return path


# Generate data
gamma = 1.2
path = smooth_geodesic_random_walk(gamma)

# Create spheroid surface
u = np.linspace(0, 2*np.pi, 100)
v = np.linspace(0, np.pi, 50)
x = np.outer(np.cos(u), np.sin(v))
y = np.outer(np.sin(u), np.sin(v))
z = gamma * np.outer(np.ones_like(u), np.cos(v))

# Plot
fig = plt.figure()
ax = fig.add_subplot(projection='3d')
ax.plot_surface(x, y, z, alpha=0.3)
ax.plot(path[:,0], path[:,1], path[:,2])
ax.set_box_aspect((1,1,gamma))
plt.show()

plt.clf()
steps = path[1:] - path[:-1]
down = np.array([0.0, 0.0, -1.0])
gravity_signal = steps @ down

# gravity_signal = np.sqrt(gravity_signal**2)

print(f"gamma: {gamma}")
print("Effective gravity:", np.mean(gravity_signal))

plt.hist(gravity_signal, bins=20)
plt.xlabel("Downward component per step")
plt.ylabel("Count")
plt.title(f"Distribution of instantaneous gravity signal, avg: {np.mean(gravity_signal):.3f}")
plt.show()
