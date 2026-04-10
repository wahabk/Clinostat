"""
taSPG path generator for OpenClino
Kim, Park & Kim (2026), Scientific Reports
https://doi.org/10.1038/s41598-026-40665-6

Algorithm
---------
Outer motor (tilt, angle theta) follows a biased speed profile [Eq 13]:
    w(theta) = wc * (1 - alpha * cos(theta))

Slows near theta=0 (upright), creating a non-zero time-averaged gravity:
    taSPG = (1 - sqrt(1 - alpha^2)) / alpha

Inner motor (azimuth, angle phi) spins at constant RPM to sweep the sphere.
Gravity vector in sample frame (standard spherical coords):
    g(theta, phi) = [sin(theta)*cos(phi), sin(theta)*sin(phi), -cos(theta)]

The only parameter to tune is target_taspg.
wc and wmin are hardware constants (measure wmin on your motors).
"""

import numpy as np
import matplotlib.pyplot as plt

np.random.seed(1234)
np.set_printoptions(suppress=True)

# Rest-time lookup from paper Table 1 (alpha=alphamax, wc=2.061, wmin=0.229)
_RT_SEC   = np.array([0.,  1.,  5.,  10.,  20.,  30.,  60., 120.])
_RT_TASPG = np.array([0.682, 0.685, 0.700, 0.717, 0.746, 0.769, 0.819, 0.874])


def taspg_from_alpha(alpha):
    """taSPG = (1 - sqrt(1 - alpha^2)) / alpha"""
    a = np.asarray(alpha, dtype=float)
    return (1.0 - np.sqrt(np.clip(1.0 - a**2, 0, 1))) / np.where(a > 1e-12, a, 1e-12)


def simulate_taspg(target_taspg, wc=2.061, wmin=0.229, n_steps=10000, dt=0.1):
    """
    Simulate the Kim 2026 taSPG algorithm and return gravity vector path.

    Hardware constants [Eq 16]:
        alphamax = 1 - wmin/wc

    alpha is derived from target_taspg analytically: alpha = 2t/(1+t^2).
    For taSPG above alphamax limit, rest_time is added per revolution.

    Returns
    -------
    path          : (n_steps+1, 3)  gravity unit vectors on sphere
    alpha         : float
    rest_time     : float  seconds
    achieved_taspg: float
    """
    alpha_max = 1.0 - wmin / wc
    max_no_rt = taspg_from_alpha(alpha_max)

    if target_taspg <= max_no_rt:
        t         = np.clip(target_taspg, 0.0, 0.9999)
        alpha     = float(min(2.0 * t / (1.0 + t**2), alpha_max))
        rest_time = 0.0
    else:
        alpha     = alpha_max
        base      = taspg_from_alpha(alpha_max)
        rt_taspg  = base + (_RT_TASPG - _RT_TASPG[0])
        rest_time = float(np.interp(target_taspg, rt_taspg, _RT_SEC))

    TWO_PI   = 2.0 * np.pi
    # Inner motor: irrational ratio to outer to avoid closed orbits
    phi_rpm  = wc * (1.0 + 1.0 / 1.618)
    phi_rate = phi_rpm * (TWO_PI / 60.0)

    theta    = 0.0
    phi      = 0.0
    rt_timer = 0.0
    path     = np.zeros((n_steps + 1, 3))

    for i in range(n_steps + 1):
        # Standard spherical coords: tilt=theta, azimuth=phi
        # gz = -cos(theta) is the taSPG-bearing component
        path[i] = [
            np.sin(theta) * np.cos(phi),
            np.sin(theta) * np.sin(phi),
           -np.cos(theta),
        ]

        phi = (phi + phi_rate * dt) % TWO_PI

        if rt_timer > 0.0:
            rt_timer = max(0.0, rt_timer - dt)
        else:
            w_rpm   = wc * (1.0 - alpha * np.cos(theta))
            dtheta  = w_rpm * (TWO_PI / 60.0) * dt
            new_th  = theta + dtheta
            if new_th >= TWO_PI and rest_time > 0.0:
                rt_timer = rest_time
            theta = new_th % TWO_PI

    achieved = float(np.linalg.norm(path.mean(axis=0)))
    return path, alpha, rest_time, achieved


def analyze_gravity_statistics(path, target_gz):
    """Unchanged from original script."""
    g_avg = np.mean(path, axis=0)
    g_mag = np.linalg.norm(g_avg)

    print("=" * 50)
    print("GRAVITY STATISTICS")
    print("=" * 50)
    print(f"Target gravity magnitude:  {target_gz:.3f} g")
    print(f"Actual average magnitude:  {g_mag:.3f} g")
    print(f"Error:                     {abs(g_mag - target_gz):.3f} g")
    print(f"\nZ-component statistics:")
    print(f"  Mean:   {np.mean(path[:, 2]):.4f}, +/- {np.std(path[:, 2]):.4f}")

    angular_changes = []
    for i in range(1, len(path)):
        angle = np.arccos(np.clip(np.dot(path[i], path[i-1]), -1, 1))
        angular_changes.append(np.degrees(angle))

    print(f"  Mean step angle:  {np.mean(angular_changes):.2f}°")

    z_values = path[:, 2]
    equator_crossings = np.sum(np.diff(np.sign(z_values)) != 0)
    time_north = np.sum(z_values > 0) / len(z_values) * 100
    time_south = np.sum(z_values < 0) / len(z_values) * 100

    print(f"\nSphere Coverage:")
    print(f"  Equator crossings:     {equator_crossings}")
    print(f"  Time in north (z>0):   {time_north:.1f}%, Time in south (z<0):   {time_south:.1f}%")
    print("=" * 50)


def plot_clinostat_path(path, target_gz):
    fig = plt.figure(figsize=(14, 6))

    # ── Left: 3D trajectory on unit sphere ───────────────────────────────────
    ax1 = fig.add_subplot(121, projection='3d')

    φ = np.linspace(0, 2 * np.pi, 30)
    θ = np.linspace(0, np.pi, 20)
    ax1.plot_wireframe(
        np.outer(np.cos(φ), np.sin(θ)),
        np.outer(np.sin(φ), np.sin(θ)),
        np.outer(np.ones_like(φ), np.cos(θ)),
        color='gray', alpha=0.1, linewidth=0.5,
    )

    t = np.linspace(0, 2 * np.pi, 100)
    ax1.plot(np.cos(t), np.sin(t), 0,    'k--', alpha=0.3, linewidth=0.5)
    ax1.plot(np.cos(t), 0,         np.sin(t), 'k--', alpha=0.3, linewidth=0.5)

    n = len(path)
    colors = plt.cm.viridis(np.linspace(0, 1, n))
    for i in range(n - 1):
        ax1.plot(path[i:i+2, 0], path[i:i+2, 1], path[i:i+2, 2],
                 color=colors[i], linewidth=1.5, alpha=0.7)

    ax1.scatter(*path[0], color='green', s=100, marker='o',
                label='Start', zorder=5, edgecolors='black')
    ax1.scatter(*path[-1], color='red', s=100, marker='s',
                label='End', zorder=5, edgecolors='black')

    if target_gz > 0:
        ax1.quiver(0, 0, 0, 0, 0, target_gz,
                   color='orange', arrow_length_ratio=0.15,
                   linewidth=2, label=f'Target ⟨g⟩={target_gz:.2f}')

    g_avg = np.mean(path, axis=0)
    ax1.quiver(0, 0, 0, g_avg[0], g_avg[1], g_avg[2],
               color='red', arrow_length_ratio=0.15,
               linewidth=2.5, label=f'Actual ⟨g⟩={np.linalg.norm(g_avg):.3f}')

    ax1.set_xlabel('X')
    ax1.set_ylabel('Y')
    ax1.set_zlabel('Z (gravity direction)')
    ax1.set_xlim([-1, 1])
    ax1.set_ylim([-1, 1])
    ax1.set_zlim([-1, 1])
    ax1.legend(loc='upper left', fontsize=8)
    ax1.set_title(f'Clinostat Path (target g={target_gz:.2f})')

    # ── Right: Z-component + running |⟨g⟩| over time ────────────────────────
    ax2 = fig.add_subplot(122)
    steps    = np.arange(n)
    z_values = path[:, 2]

    ax2.plot(steps, z_values, color='steelblue', linewidth=1, alpha=0.7,
             label='gz (instantaneous)')
    ax2.axhline(y=np.mean(z_values), color='red', linestyle='--',
                linewidth=2, label=f'Mean gz = {np.mean(z_values):.3f}')
    ax2.axhline(y=target_gz, color='orange', linestyle='--',
                linewidth=2, label=f'Target z = {target_gz:.2f}')
    ax2.axhline(y=0, color='black', linestyle='-', linewidth=0.5, alpha=0.3)

    # Running |⟨g⟩| = magnitude of cumulative mean vector
    cum_mean_g = np.cumsum(path, axis=0) / np.arange(1, n + 1)[:, None]
    running_mag = np.linalg.norm(cum_mean_g, axis=1)
    ax2.plot(steps, running_mag, color='darkgreen', linewidth=1.5,
             label=f'Running |⟨g⟩| (final={running_mag[-1]:.3f})')

    ax2.set_xlabel('Step')
    ax2.set_ylabel('Gravity magnitude / component')
    ax2.set_title('Gravity Direction Over Time')
    ax2.set_ylim([-1, 1])
    ax2.legend()
    ax2.grid(True, alpha=0.3)

    plt.tight_layout()
    return fig


def sphere_path_to_clinostat(path_xyz, filename="path.txt", pole_threshold=0.05):
    """
    Convert sphere path to motor delta commands.
    
    Rather than projecting rotation vectors (lossy — discards the X component),
    convert each point to spherical angles (θ, φ) directly, then diff.
    
    For g = [sin(θ)cos(φ), sin(θ)sin(φ), -cos(θ)]:
        θ = arccos(-gz)           outer motor (tilt)
        φ = arctan2(gy, gx)       inner motor (azimuth)
    pole_threshold : sin(θ) below this value suppresses the φ delta.
                     0.05 corresponds to ~3° from the pole.
    """
    path_xyz = np.asarray(path_xyz)

    gz  = np.clip(path_xyz[:, 2], -1.0, 1.0)
    θ   = np.arccos(-gz)
    φ   = np.arctan2(path_xyz[:, 1], path_xyz[:, 0])
    φ_unwrapped = np.unwrap(φ)

    Δθ = np.diff(np.degrees(θ))
    Δφ = np.diff(np.degrees(φ_unwrapped))

    # At each step, the relevant sin(θ) is the smaller of the two endpoints
    # (if either is near the pole, the φ command is meaningless)
    sin_θ = np.sin(θ)
    near_pole = (sin_θ[:-1] < pole_threshold) | (sin_θ[1:] < pole_threshold)
    Δφ[near_pole] = 0.0

    deltas = np.column_stack([Δθ, Δφ])
    np.savetxt(filename, deltas, fmt="%.6f", delimiter=" ")
    return deltas


if __name__ == "__main__":
    target_g = 0.38  # Mars. Moon = 0.17

    path, alpha, rest_time, achieved = simulate_taspg(
        target_g, n_steps=500, dt=0.5
    )

    print(f"alpha={alpha:.4f}  RT={rest_time:.1f}s  achieved taSPG={achieved:.3f}g")
    analyze_gravity_statistics(path, target_g)

    fig = plot_clinostat_path(path, target_g)
    plt.show()

    clino_path = sphere_path_to_clinostat(path)
    print(clino_path[:10])