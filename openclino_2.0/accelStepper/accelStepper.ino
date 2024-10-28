#include <AccelStepper.h> // Include AccelStepper library for smooth motor control

// Stepper Motor Settings
const int motorSteps = 200;                                    // Number of steps per revolution for the motor
const int subStep = 16;                                        // Microstepping setting
const unsigned long stepsPerRevolution = motorSteps * subStep; // Full steps per revolution

// Pin assignments
const int buttonPin = 2;  // Pin for button input
const int enablePinX = 3; // Enable pin for X motor
const int stepPinX = 4;   // Step pin for X motor
const int dirPinX = 5;    // Direction pin for X motor
const int enablePinY = 6; // Enable pin for Y motor
const int stepPinY = 7;   // Step pin for Y motor
const int dirPinY = 8;    // Direction pin for Y motor

// Pulley ratios (motor pulley diameter vs axis pulley diameter)
const float xMotD = 9.5;     // Motor pulley diameter X
const float xPulleyD = 31.5; // Axis pulley diameter X
const float yMotD = 9.5;     // Motor pulley diameter Y
const float yPulleyD = 18.5; // Axis pulley diameter Y

// Calculate the gear ratio for X and Y
const double xRatio = xMotD / xPulleyD;
const double yRatio = yMotD / yPulleyD;

// Steps per revolution for each axis based on the pulley ratio
const int nStepsXperRot = stepsPerRevolution / xRatio;
const int nStepsYperRot = stepsPerRevolution / yRatio;

// Random walk settings
float targetSpeedX = 0;                    // Target speed for X axis (updated by random walk)
float targetSpeedY = 0;                    // Target speed for Y axis (updated by random walk)
unsigned long lastUpdateTime = 0;          // Time since the last random walk update
const unsigned long updateInterval = 5000; // Time interval for random walk updates (in milliseconds)

// Button and LED states
int buttonState = 0;

// Initialize the stepper motors using AccelStepper (motor interface type 1 means driver)
AccelStepper stepperX(1, stepPinX, dirPinX);
AccelStepper stepperY(1, stepPinY, dirPinY);

// Function to enable/disable the motors
void enableMotors(bool enableX, bool enableY)
{
    digitalWrite(enablePinX, enableX ? LOW : HIGH); // LOW enables the motor
    digitalWrite(enablePinY, enableY ? LOW : HIGH); // LOW enables the motor
}

// Function to initialize the system
void setup()
{
    pinMode(buttonPin, INPUT);    // Button pin set as input
    pinMode(LED_BUILTIN, OUTPUT); // Built-in LED used for indication
    pinMode(enablePinX, OUTPUT);  // Enable pin for X axis motor
    pinMode(enablePinY, OUTPUT);  // Enable pin for Y axis motor

    Serial.begin(9600); // Start serial communication
    Serial.println("--\nRandom Positioning Machine Initialized\n--");

    // Disable the motors initially
    enableMotors(false, false);

    // Set acceleration for smoother motor movement
    stepperX.setMaxSpeed(100000);   // Set the maximum speed for X axis
    stepperX.setAcceleration(5000); // Set acceleration for X axis
    stepperY.setMaxSpeed(100000);   // Set the maximum speed for Y axis
    stepperY.setAcceleration(5000); // Set acceleration for Y axis
}

// Function to perform a random walk
void randomWalk()
{
    // Update the speed in random directions every 'updateInterval' milliseconds
    if (millis() - lastUpdateTime > updateInterval)
    {
        targetSpeedX = random(-300, 300); // Random speed for X axis (-300 to 300 steps per second)
        targetSpeedY = random(-300, 300); // Random speed for Y axis (-300 to 300 steps per second)

        stepperX.setSpeed(targetSpeedX); // Set the new speed for X motor
        stepperY.setSpeed(targetSpeedY); // Set the new speed for Y motor

        lastUpdateTime = millis(); // Reset the update time
    }

    // Continue moving at the set speed
    stepperX.runSpeed();
    stepperY.runSpeed();
}

// Function to handle button press and start random movement
void loop()
{
    //    buttonState = digitalRead(buttonPin); // Read the state of the button
    //
    //    if (buttonState == HIGH)
    //    {
    digitalWrite(LED_BUILTIN, HIGH); // Turn on LED to indicate motors are active
    enableMotors(true, true);        // Enable both motors

    randomWalk(); // Call random walk function to move motors in random directions
    //    }
    //    else
    //    {
    //        digitalWrite(LED_BUILTIN, LOW); // Turn off LED when motors are inactive
    //        enableMotors(false, false);     // Disable motors when button is not pressed
    //    }

    //    delay(10); // Small delay to prevent button bouncing
}
