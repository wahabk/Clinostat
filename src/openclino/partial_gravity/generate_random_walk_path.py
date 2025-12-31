import numpy as np
import matplotlib.pyplot as plt

# set random seed
np.random.seed(1234)
np.set_printoptions(suppress=True)

def smooth_partial_gravity_walk(target_gz, n_steps=100, step_size=0.1, smoothness=0.95, 
                                control_window=200):
    """
    Smooth random walk with statistical bias for stepper motor control.
    
    Control the running average Gz, not instantaneous position.
    This allows full sphere exploration while maintaining target gravity.
    
    Parameters:
    -----------
    target_gz : float ∈ [0, 1]
        Target time-averaged gravity magnitude
    n_steps : int
        Number of steps in the trajectory
    step_size : float
        Step size for integration (smaller = smoother)
    smoothness : float ∈ [0, 1]
        Velocity persistence (0.95 = very smooth, 0.5 = jerky)
    control_window : int
        Window size for computing running average (50-200 recommended)
        Larger = more exploration, slower convergence
    
    Returns:
    --------
    path : ndarray, shape (n_steps+1, 3)
        Trajectory on unit sphere
    """
    # Start at south pole (gravity pointing down)
    p = np.array([0.0, 0.0, -1.0])
    # Initial velocity (random tangent direction)
    v = np.array([1.0, 0.0, 0.0])
    path = np.zeros((n_steps + 1, 3))
    path[0] = p
    
    for i in range(1, n_steps + 1):
        # Project velocity to tangent plane (enforce v ⊥ p)
        v -= np.dot(v, p) * p
        v /= np.linalg.norm(v) + 1e-10
        
        # Add random perturbation FIRST (before drift correction)
        noise = np.random.normal(size=3)
        noise -= np.dot(noise, p) * p  # tangent plane
        noise /= np.linalg.norm(noise) + 1e-10
        # Velocity update with persistence (smooth random walk)
        v = smoothness * v + (1 - smoothness) * noise
        v /= np.linalg.norm(v) + 1e-10
        
        # apply drift correction (after smooth random walk, so it doesn't get diluted)
        window_start = max(0, i - control_window)
        z_running_avg = np.mean(path[window_start:i, 2])
        z_avg_error = z_running_avg - target_gz
        
        # Drift correction based on running average error
        drift_strength = -z_avg_error * 3.0  # Aggressive correction
        
        # Construct drift vector in tangent plane
        drift = np.array([0, 0, drift_strength])
        drift -= np.dot(drift, p) * p  # project to tangent plane
        drift_norm = np.linalg.norm(drift)
        
        # Apply drift correction
        if drift_norm > 1e-8:
            drift /= drift_norm
            # Strong adaptive weight
            drift_weight = max(0.25, abs(z_avg_error) * 1.2)
            v = (1 - drift_weight) * v + drift_weight * drift
            v /= np.linalg.norm(v) + 1e-10
        
        # Integrate position
        p = p + step_size * v
        p /= np.linalg.norm(p)
        
        path[i] = p
    
    return path


def plot_clinostat_path(path, target_gz):
    """
    Visualize the 3D path on a unit sphere with gravity statistics.
    
    Parameters:
    -----------
    path : ndarray, shape (n, 3)
        Trajectory points on sphere
    target_gz : float
        Target gravity magnitude for comparison
    """
    fig = plt.figure(figsize=(14, 6))
    
    # === Left plot: 3D trajectory on sphere ===
    ax1 = fig.add_subplot(121, projection='3d')
    
    # Draw sphere wireframe
    u = np.linspace(0, 2 * np.pi, 30)
    v = np.linspace(0, np.pi, 20)
    x_sphere = np.outer(np.cos(u), np.sin(v))
    y_sphere = np.outer(np.sin(u), np.sin(v))
    z_sphere = np.outer(np.ones(np.size(u)), np.cos(v))
    ax1.plot_wireframe(x_sphere, y_sphere, z_sphere, 
                       color='gray', alpha=0.1, linewidth=0.5)
    
    # Draw equator and prime meridian
    theta = np.linspace(0, 2*np.pi, 100)
    ax1.plot(np.cos(theta), np.sin(theta), 0, 'k--', alpha=0.3, linewidth=0.5)
    ax1.plot(np.cos(theta), 0, np.sin(theta), 'k--', alpha=0.3, linewidth=0.5)
    
    # Draw path with color gradient (time progression)
    n = len(path)
    colors = plt.cm.viridis(np.linspace(0, 1, n))
    
    for i in range(n-1):
        ax1.plot(path[i:i+2, 0], path[i:i+2, 1], path[i:i+2, 2], 
                color=colors[i], linewidth=1.5, alpha=0.7)
    
    # Mark start (south pole) and end
    ax1.scatter(*path[0], color='green', s=100, marker='o', 
               label='Start (South Pole)', zorder=5, edgecolors='black')
    ax1.scatter(*path[-1], color='red', s=100, marker='s', 
               label='End', zorder=5, edgecolors='black')
    
    # Draw target gravity vector
    if target_gz > 0:
        ax1.quiver(0, 0, 0, 0, 0, target_gz, 
                  color='orange', arrow_length_ratio=0.15, 
                  linewidth=2, label=f'Target ⟨g⟩={target_gz:.2f}')
    
    # Draw actual average gravity vector
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
    
    # === Right plot: Z-component over time ===
    ax2 = fig.add_subplot(122)
    
    steps = np.arange(len(path))
    z_values = path[:, 2]
    
    ax2.plot(steps, z_values, color='steelblue', linewidth=1, alpha=0.7)
    ax2.axhline(y=np.mean(z_values), color='red', linestyle='--', 
               linewidth=2, label=f'Mean z = {np.mean(z_values):.3f}')
    ax2.axhline(y=target_gz, color='orange', linestyle='--', 
               linewidth=2, label=f'Target z = {target_gz:.2f}')
    ax2.axhline(y=0, color='black', linestyle='-', linewidth=0.5, alpha=0.3)
    
    ax2.set_xlabel('Step')
    ax2.set_ylabel('Z-component (gravity direction)')
    ax2.set_title('Gravity Direction Over Time')
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    return fig

def analyze_gravity_statistics(path, target_gz):
    """
    Print detailed statistics about the gravity trajectory.
    """
    g_avg = np.mean(path, axis=0)
    g_mag = np.linalg.norm(g_avg)
    
    print("="*50)
    print("GRAVITY STATISTICS")
    print("="*50)
    print(f"Target gravity magnitude:  {target_gz:.3f} g")
    print(f"Actual average magnitude:  {g_mag:.3f} g")
    print(f"Error:                     {abs(g_mag - target_gz):.3f} g")
    print(f"\nZ-component statistics:")
    print(f"  Mean:   {np.mean(path[:, 2]):.4f}, +/- {np.std(path[:, 2]):.4f}")
    
    # Angular velocity analysis (smoothness check)
    angular_changes = []
    for i in range(1, len(path)):
        angle = np.arccos(np.clip(np.dot(path[i], path[i-1]), -1, 1))
        angular_changes.append(np.degrees(angle))
    
    print(f"  Mean step angle:  {np.mean(angular_changes):.2f}°")
    
    # Hemisphere crossing analysis
    z_values = path[:, 2]
    equator_crossings = np.sum(np.diff(np.sign(z_values)) != 0)
    time_north = np.sum(z_values > 0) / len(z_values) * 100
    time_south = np.sum(z_values < 0) / len(z_values) * 100
    
    print(f"\nSphere Coverage:")
    print(f"  Equator crossings:     {equator_crossings}")
    print(f"  Time in north (z>0):   {time_north:.1f}%, Time in south (z<0):   {time_south:.1f}%")
    print("="*50)

def sphere_path_to_clinostat(path_xyz, filename="path.txt"):
    """
    Convert sphere path using direct axis-angle projection.
    It works because it avoids the Euler angle representation entirely.
    
    ALGORITHM:
    ---------
    For each pair of points (p0, p1):
    1. Compute rotation axis: ω = p0 × p1 (perpendicular to both)
    2. Compute rotation angle: θ = arctan2(||ω||, p0·p1)
    3. Project rotation vector (θ·ω) onto motor axes
    4. Output projection deltas as motor commands
    
    --------------
    The rotation vector θ·ω lives in R³ and encodes the full 3D rotation.
    The two motors span a 2D subspace of R³ (they can't generate arbitrary
    rotations - your gimbal is underactuated). By projecting θ·ω onto your
    motor axes, you're finding the "closest" achievable rotation.
    
    This works for sphere paths because:
    - Sphere paths only need 2 DOF (azimuth, elevation)
    - Your 2-motor gimbal provides exactly 2 DOF
    - The projection is well-defined and continuous everywhere
    
    GEOMETRIC INTUITION:
    -------------------
    Imagine the rotation as an arrow in 3D space. The arrow's direction is
    the axis, and its length is the angle. Your motors are like two coordinate
    axes in this space. You're just reading off the X and Y components.
    
    At the poles, the arrow smoothly changes direction, so the components
    change smoothly - no singularity!
    
    Parameters:
    -----------
    path_xyz : array_like, shape (N, 3)
        Points on unit sphere
    filename : str
        Output file
    
    Returns:
    --------
    deltas : ndarray, shape (N-1, 2)
        Motor commands in degrees
    """
    path_xyz = np.asarray(path_xyz)
    deltas = []
    
    for i in range(len(path_xyz) - 1):
        p0 = path_xyz[i]
        p1 = path_xyz[i+1]
        
        # Compute rotation axis (perpendicular to both vectors)
        axis = np.cross(p0, p1)
        axis_norm = np.linalg.norm(axis)
        
        if axis_norm < 1e-10:
            deltas.append([0.0, 0.0])
            continue
        
        # Normalize axis to unit length
        axis = axis / axis_norm
        
        # Compute rotation angle using both sin and cos for robustness
        # arctan2(sin, cos) handles all quadrants correctly
        sin_angle = axis_norm 
        cos_angle = np.dot(p0, p1)  # p0 · p1 = cos(θ)
        angle = np.arctan2(sin_angle, cos_angle)
        
        # Motor axis mapping
        # Standard clinostat (X=tilt, Y=spin):
        #   X-motor rotates around Y-axis in lab frame → axis[1]
        #   Y-motor rotates around Z-axis in lab frame → axis[2]
        # If your motors are different, adjust these indices:
        # axis[i], i = 0,1,2 corresponds to X,Y,Z axes respectively
        
        d_phi = np.degrees(angle * axis[1])    # X-motor command
        d_theta = np.degrees(angle * axis[2])  # Y-motor command
        
        deltas.append([d_phi, d_theta])
    
    deltas = np.asarray(deltas)
    np.savetxt(filename, deltas, fmt="%.6f", delimiter=" ")
    return deltas

if __name__ == "__main__":
    target_g = 0.38 # Mars gravity
    
    path = smooth_partial_gravity_walk(target_gz=target_g)
    analyze_gravity_statistics(path, target_g)
    
    # Visualize
    fig = plot_clinostat_path(path, target_g)
    plt.show()

    clino_path = sphere_path_to_clinostat(path)
    print(path[:50])
    print(clino_path[:50])