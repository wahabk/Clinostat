# OpenClino

It is very difficult to access a 3D clinostat, there are some companies that sell it but can be prohibitively expensive for gravity research. OpenClino can be built for £100 using off the shelf parts. OpenClino can run in continuous clinorotation or as a Random Positioning Machine (RPM).

This is a side project for me and is very much work in progress.

## Contents list

- [`Bill of Materials`](2_BOM.md)
- [`Build Guide`](3_build_guide.md)
- [OpenClino](#openclino)
  - [Contents list](#contents-list)
  - [Notation](#notation)
  - [Code](#code)

## Convention & Definitions

- X body is the external larger square. 
- Y body is the internal smaller body.
- Motor pulleys attach to the motor.
- X/Y pulleys attach to the body.
- The "`m2y`" pulley is the one with 2 belts attached.
- The "`y_turn`" is the bearings bending the long y belt.
- The "`y_guide`" is the L shaped bracket holding the y pulley.

## Code

[OpenClino Documentation.](4_code_documentation.md)

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

![Clinostat circuit.](images/openclino_circuit_diagram.png "Circuit diagram. Made with Cirkit Designer.")
