# OpenClino Documentation

This document provides reference information about the functions used to control stepper motors in the Arduino project. Each function is described with its purpose, parameters, and behavior.

## Functions

### `void yClockwise(bool foo)`

Sets the direction of the Y-axis motor.

- **Parameters:**
  - `foo` (bool): If `true`, sets the direction to clockwise; if `false`, sets it to counterclockwise.

---

### `void spin_continuous(float speedRPMX = 60, float speedRPMY = 60, int finalDelay = 100)`

Spins the X and Y motors continuously at specified speeds until a button is pressed.

- **Parameters:**
  - `speedRPMX` (float, default = 60): Speed of the X-axis motor in RPM.
  - `speedRPMY` (float, default = 60): Speed of the Y-axis motor in RPM.
  - `finalDelay` (int, default = 100): Delay in milliseconds after stopping the motors.

---

### `void spin_degs(float degX, float degY, float speedX = 10, float speedY = 10, int finalDelay = 100)`

Spins the X and Y motors by specified degrees at specified speeds.

- **Parameters:**
  - `degX` (float): Degrees to rotate the X-axis motor.
  - `degY` (float): Degrees to rotate the Y-axis motor.
  - `speedX` (float, default = 10): Speed of the X-axis motor in RPM.
  - `speedY` (float, default = 10): Speed of the Y-axis motor in RPM.
  - `finalDelay` (int, default = 100): Delay in milliseconds after stopping the motors.

---

### `void RPM()`

Placeholder function for future implementation.

---

### `void calibrate_pulley_teeth()`

Calibrates the pulley teeth by rotating the motors in specific patterns.

---

### `void test_spin_degs_multi()`

Tests the `spin_degs` function with multiple rotations.

---

### `void setup()`

Initializes the Arduino pins and serial communication, and prints a welcome message.

---

### `void loop()`

Main loop that reads the button state and performs actions based on the button press.

- **Behavior:**
  - Turns off motors when the button is not pressed.
  - When the button is pressed, it turns on the motors, lights up the built-in LED, and runs calibration and test functions.
