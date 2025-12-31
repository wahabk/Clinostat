# OpenClino

It is very difficult to access a 3D clinostat, there are some companies that sell it but can be prohibitively expensive for gravity research. OpenClino can be built for £100 using off the shelf parts. OpenClino can run in continuous clinorotation or as a Random Positioning Machine (RPM).

> [!NOTE]  
> Since Version 0.3.0 remember to run the [calibration procedure](#usage--calibration).

## Definitions

- X body is the outer body. 
- Y body is the inner body.
- Motor pulleys attach to the motor.
- X/Y pulleys attach to the body.
- The "`m2y`" pulley is the one with 2 belts attached.
- The "`y_turn`" is the bearings bending the long y belt.
- The "`y_guide`" is the L shaped bracket holding the y pulley.

## Circuit design

This is a very simple arduino circuit with a button and 2 stepper motor controllers.

I base the circuit on [this great article by lastminuteengineers](https://lastminuteengineers.com/a4988-stepper-motor-driver-arduino-tutorial/]).

![Clinostat circuit.](images/openclino_circuit_diagram.png "Circuit diagram. Made with Cirkit Designer.")

### Optional: MicroSD card reader

You can optionally add a MicroSD card reader to the circuit

Again I base the circuit on [this great article by lastminuteengineers](https://lastminuteengineers.com/arduino-micro-sd-card-module-tutorial/]).

![Clinostat with MicroSD circuit.](images\openclino_circuit_diagram_with_microSD_reader.png "Circuit w/ SD diagram. Made with Cirkit Designer.")


## Code

[OpenClino Documentation.](4_code_documentation.md)

## Usage
### Calibration

To calibrate the Y-axis correction factor, follow these steps:

1. Upload the code to your Arduino.
2. Open the Serial Monitor in the Arduino IDE.
3. Center the frames so that they are overlapping.
4. Press the button connected to the Arduino to start the calibration process.
5. The Y-axis motor will rotate by 360 degrees.
6. The Serial Monitor will prompt you to enter the actual degrees the Y-axis rotated.
7. Enter the measured degrees in the Serial Monitor and press Enter.
8. The new Y-axis correction factor will be calculated and displayed in the Serial Monitor.
9. Manually update the `yCorrection` variable in the code with the new correction factor.

Example:

```cpp
double yCorrection = 1.5; // Update this value with the new correction factor
```

> [!NOTE]  
> You might have to run the calibration a couple of times to get the right correction. The effect of the outer body on the inner body is dependant on friction in the system which is widely variable.

Now your clinostat is ready to use! To run in clinorotation mode simply add these to your arduino's loop function, this will run the x axis at 2 RPM and the y axis at 4:

```cpp
void loop() {
    spin_continuous(2,4);
}
```

### Partial gravity simulation

Partial gravity simulations require a more complex path to follow, which the arduino is not able to compute at runtime.

The method is based on a geodesic random walk on a spheroid. The sphere is sampled evenly which usually results in close to 0g, and the error from the target Gz is computed. A drift term is added to the clinostat's path to bias the running average Gz to converge to the target G. This is based on the paper - [Novel, Moon and Mars, partial gravity simulation paradigms and their effects on the balance between cell growth and cell proliferation during early plant development](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC5884789/).

After setting up your arduino with a MicroSD card reader as shown in the Circuit Design documentation above, follow these steps:

1. Generate a path with `python generate_random_walk_path.py`
2. Visualise the path and average Gz with the above script until you are happy with the path. You may need to adjust `n_steps`, `step_size`, and the `control_window`.
3. Copy the generated `path.txt` to a MicroSD card and insert into the arduino reader.
4. Run `openclino.ino` with `follow_path();` uncommented. The serial Monitor will help debug any issues with the SD card.

### SD Card debugging

There can be faults with the MicroSD card reader, the card itself, or the file added.

1. Make sure the file is called `path.txt` or `PATH_FILENAME` is adjusted in `openclino.cpp`.
2. Make sure the file is in the root directory of the SD card (not inside another folder).
3. Ensure the SD card and reader work with `test_sd/test_sd.ino`.
4. Test that the path file works with `test_path/test_path.ino`.