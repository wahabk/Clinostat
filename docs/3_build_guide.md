
# OpenClino Build Guide
<img src="images/build_guide/0_exploded_view.jpg" alt="y_turn" width="400"/>


## Contents

- [OpenClino Build Guide](#openclino-build-guide)
  - [Contents](#contents)
  - [3D Printing](#3d-printing)
  - [Hardware](#hardware)
    - [Tools required:](#tools-required)
    - [Build Instructions:](#build-instructions)
      - [**1 - Build y\_turn.**](#1---build-y_turn)
      - [**2 - Combiningx and y bodies.**](#2---combiningx-and-y-bodies)
      - [**3 - x\_pulley and m2y pulley.**](#3---x_pulley-and-m2y-pulley)
      - [**4 - Build the base.**](#4---build-the-base)
      - [**5 - Final Assembly.**](#5---final-assembly)


## 3D Printing

All files are available in `3d_files/`.
The parts can be printed in PLA but I prefer to use PETG. 

## Hardware

The build of openclino usually takes 1-2 hours. Step 0 is make sure you make a coffee or your favourite hot/cold beverage.
We will build the sub assemblies: y_turn, bodies, and the base, and the final step 5 is to combine everything.

### Tools required:

- Spanner/wrench: Number 13 metric (same as a skateboard).
- Alan keys: 3 mm for the base.
            2 mm for the motors and bodies.
            1.5 mm for the motor pulley grub screw.

### Build Instructions:

Ensure you have all the parts from the [`Bill of Materials`](2_BOM.md).

#### **1 - Build y_turn.**

   a) Place the washers between the bolthead/nut and the bearing.
      **NOTE**: Don't tighten the nuts too much as this will introduce a lot of unwanted friction.

<img src="images/build_guide/1_y_turn.jpg" alt="y_turn" width="400"/>

#### **2 - Combiningx and y bodies.**

   a) Push fit bearings to the y_guide and the x body. 

<img src="images/build_guide/2_combine.jpg" alt="combine" width="400"/>

   b) Install bolts through the x and y body, make sure to put a washer on each side of the bearing.

<img src="images/build_guide/3_bolt.jpg" alt="combine" width="400"/>

   c) Install y_pulley and y_guide, with washers in between. 
      
   **NOTE**: Remember to add the y belt before the nut!
   
   The hex nuts can be push fit into the x body.
   
   Tighten the y_pulley grub screw so that it rotates with the y body.
   
   **NOTE**: the grub screw design of the y pully relies on a threaded insert that needs to be installed with a soldering iron.
         I will endeavor to make one that has a normal hex nut.

<img src="images/build_guide/3_y_guide.jpg" alt="combine" width="400"/>


   d) Install the y_turn into the x body. The hex nuts can be push fit into the x body.

<img src="images/build_guide/3_y_turn.jpg" alt="combine" width="400"/>


   e) Ensure everything rotates smoothly. The Nylock nuts don't have to be tightened, just enough so that it doesn't shake.

#### **3 - x_pulley and m2y pulley.**

   Now that x and y are combined, lets add the x_pulley and m2y pulley.

   a) x_pulley: push fit the x pulley into the body, and place washers in for now. You might have to sand the pins on the x_pulley.

<img src="images/build_guide/4_x_pulley.jpg" alt="combine" width="400"/>

   b) m2y pulley: push fit two bearings into the m2y pulley and add washers for now.
      
  **NOTE**: the tricky part here is making sure the long belt doesnt scrape the motor like step 5e, 
      
      so remember to keep the wider part of the m2y close to the x body, 
      
      and place 2 or 3washers between the x body and the m2y pulley.
   
<img src="images/build_guide/4_x_pulley.jpg" alt="combine" width="400"/>

TODO update m2y image

   We have now fully combined x and y bodies!

<img src="images/build_guide/4_combined.jpg" alt="combine" width="400"/>
 
 #### **4 - Build the base.**

<img src="images/build_guide/5_start.jpg" alt="combine" width="400"/>

   a) Install the motor pulleys.
      
   Make sure to align the notch in the motor shaft with one of the grub screws in the pulley.
   
   Keep the motors loose for now as you will need to tension the belts later.
   
   Repeat for both sides.

<img src="images/build_guide/5_motor_pulley.jpg" alt="combine" width="200"/>
<img src="images/build_guide/5_motor.jpg" alt="combine" width="200"/>

   b) Install the motor to the motor mount using 4 hex screws.

   You can also push fit the bearings into the motor mount.

   Repeat for both sides.

<img src="images/build_guide/5_motor_mount.jpg" alt="combine" width="400"/>

   c) Push fit the bearings into the motor_mount and motor_enclosure.

<img src="images/build_guide/5_motor_enclosure.jpg" alt="combine" width="400"/>

   d) Install the enclosures and motor mounts to the aluminium rails.
      
   Keep all the screws loose for now, it will take a low of wiggling to align everything!

<img src="images/build_guide/5_motor_base.jpg" alt="combine" width="400"/>

#### **5 - Final Assembly.**

   You should now have two assemblies; the base, and the x+y bodies.

<img src="images/build_guide/6_start.jpg" alt="combine" width="400"/>

   a) Add the short belts to the x_pulley and m2y_pulley.

   b) Mount the x_body assembly to the base.
      
   Remember to add a washer between the pulleys and the motor_mount bearings.

<img src="images/build_guide/6_bolts.jpg" alt="combine" width="400"/>

   c) Tighten the long bolts but not too much, as it might break the mounts and enclosures.
      
   Again you don't want anything to be tight, make sure it all spins smoothly.

<img src="images/build_guide/6_nuts.jpg" alt="combine" width="400"/>

   d) Hook the short belts to the motor pulley.
      
   You will have to lift the motors up to attach the belt, then tension the belt and tighten the motor screws.

<img src="images/build_guide/6_x_belt.jpg" alt="combine" width="400"/>

   e) This step is just to ensure that the y belt (short) m2y belt (long) don't interfere.
      
   This is the tightest tolerance in the clinostat.

<img src="images/build_guide/6_tricky.jpg" alt="combine" width="400"/>

   f) Finally, only after installing everything and ensuring both axes rotate smoothly should you tighten the base.

<img src="images/build_guide/6_finito.jpg" alt="combine" width="400"/>

Make sure every pulley and axis rotates smoothly. If there is friction anywhere you might have to loosen the bolts or sand down interference from your prints.

