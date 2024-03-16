# OpenClino

## Contents list

- [`Bill of Materials`](2_BOM.md)
- [`Build Guide`](3_build_guide.md)
- [OpenClino](#openclino)
  - [Contents list](#contents-list)
  - [Notation](#notation)
  - [Code](#code)

## Design requirements

Clinostats are usually used in biology for plants, cell cultures, and small animals such as fish.

1 - OpenClino shall be robust to failures to not harm the animals or samples. 
2 - OpenClino should operate in hot/humid environments such as incubators.
3 - OpenClino should operate as a clinostat or RPM.

At the moment it is a work in progress. I'm not an electrical engineer so this is 12v.
Be careful with your electronics. 
At the moment it functions as a clino but not an RPM. The motors can't start a new direction or speed independently.

## Code

The code is work in progress, it requires the following:
 - control multiple motors.
 - acceleration.
 - docs.

## Notation
TODO 

Definitions:
motor_pulley
x_pulley
y_pulley
m2y pulley
y_turn
y_turn_pulley
y_guide
motor mount
motor enclosure
which is x and y
