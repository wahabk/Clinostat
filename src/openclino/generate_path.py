import time
import numpy as np
import pandas as pd

# generate a path for clinostat to follow 90, 180, 270 degrees
def generate_path(steps=1000, radius=1.0, angle=90):
    """
    Generate a path for the clinostat to follow.
    
    Parameters:
    steps (int): Number of steps in the path.
    radius (float): Radius of the circular path.
    angle (float): Angle in degrees to rotate the clinostat.
    
    Returns:
    pd.DataFrame: DataFrame containing the x and y coordinates of the path.
    """
    theta = np.linspace(0, np.deg2rad(angle), steps)
    x = radius * np.cos(theta)
    y = radius * np.sin(theta)
    
    path_df = pd.DataFrame({'x': x, 'y': y})
    
    return path_df

if __name__ == "__main__":
    # Example usage
    steps = 1000
    radius = 1.0
    angle = 90
    
    path_df = generate_path(steps, radius, angle)
    
    # Save the path to a CSV file
    path_df.to_csv('clinostat_path.csv', index=False)
    
    print("Path generated and saved to clinostat_path.csv")