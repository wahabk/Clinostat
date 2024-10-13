#include <AccelStepper.h>

// Define the stepper motor connections and the motor interface type.
#define motorInterfaceType 1 // Basic 2-pin interface: Step and Direction

// Motor 1 pins
#define stepPin1 7
#define dirPin1 8

// Motor 2 pins
#define stepPin2 4
#define dirPin2 5

// Create stepper motor objects
AccelStepper stepper1(motorInterfaceType, stepPin1, dirPin1);
AccelStepper stepper2(motorInterfaceType, stepPin2, dirPin2);

void setup()
{
    // Set the maximum speed and acceleration for the motors
    stepper1.setMaxSpeed(100000);    // steps per second
    stepper1.setAcceleration(5000); // steps per second^2

    stepper2.setMaxSpeed(100000);    // steps per second
    stepper2.setAcceleration(5000); // steps per second^2

    // Randomize the seed for generating random movements
    randomSeed(analogRead(0));
}

void loop()
{
    // Check if both motors have finished their current moves
    if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0)
    {

        // Generate random steps for motor 1
        long randomSteps1 = random(-25000, 25000); // Random steps between -2000 and 2000
        stepper1.move(randomSteps1);

        // Generate random steps for motor 2
        long randomSteps2 = random(-25000, 25000); // Random steps between -2000 and 2000
        stepper2.move(randomSteps2);

        // Optional: You can add delays to simulate pauses between movements
//        delay(random(500, 2000)); // Pause for a random duration between 500ms and 2000ms
    }

    // Continue moving the motors to their target positions
    stepper1.run();
    stepper2.run();
}
