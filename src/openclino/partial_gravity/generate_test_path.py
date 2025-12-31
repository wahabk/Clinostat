import numpy as np
import pandas as pd

def generate_test_path(x_angle, y_angle):
    """
    Generate a test path that moves X and Y axes sequentially with gradually increasing step sizes.
    
    Parameters:
    - x_angle: Total angle in degrees for X axis movement
    - y_angle: Total angle in degrees for Y axis movement
    
    Returns:
    - DataFrame with columns 'x', and 'y',  containing the path coordinates
    """
    # Create path data
    positions = []
    
    # Starting position
    current_x = 0
    current_y = 0
    positions.append({'x': current_x, 'y': current_y})
    
    # Start with small steps and gradually increase
    remaining_x = x_angle
    remaining_y = y_angle
    
    step_size = 1  # Start with 1 degree steps
    max_step_size = 5  # Maximum step size
    
    while remaining_x > 0 or remaining_y > 0:
        # Calculate the actual step size for this iteration
        x_step = min(step_size, remaining_x)
        y_step = min(step_size, remaining_y)
        
        # Update positions
        if remaining_x > 0:
            current_x += x_step
            remaining_x -= x_step
        
        if remaining_y > 0:
            current_y += y_step
            remaining_y -= y_step
    
        # Add the position
        positions.append({'x': current_x, 'y': current_y})
    
    # Create DataFrame
    path_df = pd.DataFrame(positions)
    return path_df

# Generate RPM
if __name__ == "__main__":
    # Example usage
    x = 90
    y = 90    
    path_df = generate_test_path(x, y)
    
    # Save as space-delimited text file for Arduino's follow_path function
    with open("openclino_path.txt", "w") as f:
        for i, row in path_df.iterrows():
            # Skip the first row (starting position) as it has no delay
            if i > 0:
                f.write(f"{row['x']} {row['y']}\n")
    
    print(f"Path generated and saved to openclino_path.txt and openclino_path.csv")
    print(f"Total movements: {len(path_df)-1}")
