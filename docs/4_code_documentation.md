# OpenClino Documentation

This document provides reference information about the functions used to control stepper motors in the Arduino project. Each function is described with its purpose, parameters, and behavior. The documentation is generated using AI.

## Functions

### `void enable(bool enableX, bool enableY)`

Enables or disables the X and Y motors.

- **Parameters:**
  - `enableX` (bool): If `true`, enables the X-axis motor; if `false`, disables it.
  - `enableY` (bool): If `true`, enables the Y-axis motor; if `false`, disables it.

---

### `void spin_continuous(float speedX = 10, float speedY = 10, int buttonDelay = 1000)`

Spins the X and Y motors continuously at specified speeds until a button is pressed.

- **Parameters:**
  - `speedX` (float, default = 10): Speed of the X-axis motor in RPM.
  - `speedY` (float, default = 10): Speed of the Y-axis motor in RPM.
  - `buttonDelay` (int, default = 1000): Delay in milliseconds after stopping the motors.

---

### `void spin_degs(float Q1_target, float Q2_target, float maxRPM = 10, int finalDelay = 100)`

Spins the X and Y motors by specified degrees at specified speeds.

- **Parameters:**
  - `Q1_target` (float): Degrees to rotate the X-axis motor.
  - `Q2_target` (float): Degrees to rotate the Y-axis motor.
  - `maxRPM` (float, default = 10): Maximum speed of the motors in RPM.
  - `finalDelay` (int, default = 100): Delay in milliseconds after stopping the motors.

---

### `void calibrate_y_correction()`

Calibrates the Y-axis correction factor by rotating the Y-axis motor and measuring the actual rotation.

---

### `void test_pulley_ratios()`

Tests the pulley ratios by rotating the X and Y motors by 360 degrees and back.

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
