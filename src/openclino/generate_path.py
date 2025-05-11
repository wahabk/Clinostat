import time
import numpy as np
import pandas as pd

def generate_test_path(x_angle, y_angle, speed):
    """
    Generate a test path that moves X and Y axes sequentially with gradually increasing step sizes.
    
    Parameters:
    - x_angle: Total angle in degrees for X axis movement
    - y_angle: Total angle in degrees for Y axis movement
    - speed: Speed in degrees per second
    
    Returns:
    - DataFrame with columns 'x', 'y', and 'delay_us' containing the path coordinates
       and timing information in microseconds
    """
    # Create path data
    positions = []
    
    # Starting position
    current_x = 0
    current_y = 0
    current_time = 0
    positions.append({'x': current_x, 'y': current_y, 'time': current_time, 'delay_us': 0})
    
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
        
        # Calculate time based on speed (degrees per second)
        # Time needed = distance / speed
        time_needed = max(x_step, y_step) / speed  # Time in seconds
        current_time += time_needed
        
        # Calculate delay in microseconds for this step
        delay_us = int(time_needed * 1000000)
        
        # Add the position
        positions.append({
            'x': current_x, 
            'y': current_y, 
            'time': current_time,
            'delay_us': delay_us
        })
        
        # Increase step size gradually, but cap at max_step_size
        if step_size < max_step_size:
            step_size += 0.5
    
    # Create DataFrame
    path_df = pd.DataFrame(positions)
    return path_df

if __name__ == "__main__":
    # Example usage
    x = 90
    y = 90
    speed = 10.0  # Speed in degrees per second
    
    path_df = generate_test_path(x, y, speed)
    
    # Save as space-delimited text file for Arduino's follow_path function
    with open("openclino_path.txt", "w") as f:
        for i, row in path_df.iterrows():
            # Skip the first row (starting position) as it has no delay
            if i > 0:
                f.write(f"{row['x']} {row['y']} {int(row['delay_us'])}\n")
    
    # Also save as CSV for reference/debugging
    path_df.to_csv("openclino_path.csv", index=False)
    
    print(f"Path generated and saved to openclino_path.txt and openclino_path.csv")
    print(f"Total movements: {len(path_df)-1}")
    print(f"Total time: {path_df['time'].max():.2f} seconds")