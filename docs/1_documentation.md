# OpenClino

## Contents list

- [`Bill of Materials`](2_BOM.md)
- [`Build Guide`](3_build_guide.md)
- [OpenClino](#openclino)
  - [Contents list](#contents-list)
  - [Notation](#notation)
  - [Code](#code)

## Notation

TODO make a figure defining part names

Definitions:
motor_pulley
x_pulley
y_pulley
m2y pulley
y_turn
y_turn_pulley
y_guide
motor_mount
motor_enclosure
which is x and y

## Code

The code is work in progress, it requires the following:

- control multiple motors separately.
- acceleration.
- docs.
- Implementing the airbus algorithm for variable G.

### Usage

To run in clinorotation mode simply add these to your arduino's loop function, this will run the x axis at 30 rpm and the y axis at 60:

```cpp
void loop() {
    spin_continuous(30,60);
}
```

Or to run as a random positioning machine, this will run a random walk routine as specified in ESA's work:

```cpp
void loop() {
    RPM();
}
```

Don't forget to set the output pins for you motor controllers!

# Circuit design

This is a very simple arduino circuit with a button and 2 stepper motor controllers.

TODO add quiet motor controller

![Clinostat circuit.](images/openclino_circuit_diagram.png "Circuit diagram. Made with Cirkit Designer.")
