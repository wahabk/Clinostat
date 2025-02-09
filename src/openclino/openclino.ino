#include <AccelStepper.h>

// pins
const int buttonPin = 2;
const int enablePinX = 3;
const int stepPinX = 4;
const int dirPinX = 5;
const int enablePinY = 6;
const int stepPinY = 7;
const int dirPinY = 8;

// Define steps per revolution
const int motorSteps = 200;
const int subStep = 16;
const unsigned long stepsPerRevolution = motorSteps * subStep;

// Pulley teeth
const float xMotTeeth = 16;
const float xPulleyTeeth = 50;
const float yMotTeeth = 16;
const float yTurnTeeth = 50;
const float yPulleyTeeth = 20;

// Calculate the gear ratios based on pulley teeth and calibration factor
const double xRatio = xPulleyTeeth / xMotTeeth; // 3.125
const double yRatio1 = yTurnTeeth / yMotTeeth;  // 50 / 16 = 3.125
const double yRatio2 = yPulleyTeeth / yTurnTeeth; // 20 / 50 = 0.4
const double yCorrection = 1.5; // Found from calibration using diagnose_rotation_error()
const double yRatio = yRatio1 * yRatio2 * yCorrection;

// Calculate effective steps per rotation
const double nStepsPerRotX = stepsPerRevolution * xRatio;
const double nStepsPerRotY = stepsPerRevolution * yRatio;

int buttonState = 0;

void wake(bool x, bool y)
{
  if (x == true)
  {
    digitalWrite(enablePinX, LOW);
  }
  else
  {
    digitalWrite(enablePinX, HIGH);
  }
  if (y == true)
  {
    digitalWrite(enablePinY, LOW);
  }
  else
  {
    digitalWrite(enablePinY, HIGH);
  }
}

void xClockwise(bool foo)
{
  if (foo == true)
  {
    digitalWrite(dirPinX, HIGH);
  }
  else
  {
    digitalWrite(dirPinX, LOW);
  }
}

void yClockwise(bool foo)
{
  if (foo == true)
  {
    digitalWrite(dirPinY, LOW);
  }
  else
  {
    digitalWrite(dirPinY, HIGH);
  }
}

void spin_continuous(float speedX = 10, float speedY = 10, int finalDelay = 100)
{
  bool xClock;
  bool yClock;

  if (speedX >= 0)
  {
    xClock = true;
  }
  else
  {
    xClock = false;
  }
  if (speedY >= 0)
  {
    yClock = true;
  }
  else
  {
    yClock = false;
  }

  double intervalX = (6e7 / speedX) / nStepsPerRotX;
  double intervalY = (6e7 / speedX) / nStepsPerRotY;
  long intervalXLong = (long)intervalX;
  long intervalYLong = (long)intervalY;
  unsigned long stepsY = 0;
  unsigned long stepsX = 0;
  unsigned long previousTimeX = micros();
  unsigned long previousTimeY = micros();

  while (true)
  {
    unsigned long currentTimeX = micros();
    unsigned long currentTimeY = micros();
    unsigned long currentTime = micros();
    bool xGo = false; // whether to step x
    bool yGo = false; // whether to step y
    bool skipCompensation = false;

    digitalWrite(stepPinX, HIGH);
    digitalWrite(stepPinY, HIGH);

    if (currentTime - previousTimeX > intervalXLong)
    {
      xGo = true;
    }
    if (currentTime - previousTimeY > intervalYLong)
    {
      yGo = true;
    }
    if (yGo == true && xGo == true && yClock == false && xClock != yClock)
    {
      skipCompensation = true;
    }

    if (xGo)
    {
      xClockwise(xClock);
      digitalWrite(stepPinX, LOW);
      digitalWrite(stepPinX, HIGH);
      previousTimeX = currentTime;

      if (skipCompensation == false)
      {
        yClockwise(!xClock); // spin y motor with x

        digitalWrite(stepPinY, LOW); // compensator
        digitalWrite(stepPinY, HIGH);
      }
      stepsX++;
    }

    if (yGo)
    {
      yClockwise(yClock);
      digitalWrite(stepPinY, LOW);
      digitalWrite(stepPinY, HIGH);
      previousTimeY = currentTime;
      stepsY++;
    }
    //    if (stepsX >= nStepsX && stepsY >= nStepsY) {keepGoing = false;} // check if finished
  }
  delay(finalDelay);
}

void spin_degs(float degX, float degY, float speedX = 10, float speedY = 10, int finalDelay = 100)
{
  // This function is perfect without accel
  // Initialize motor direction based on target degrees
  bool xClock = (degX >= 0);
  bool yClock = (degY >= 0);

  // Calculate steps needed for each axis based on degree input
//  int nStepsX = nStepsPerRotX * abs(degX / 360.0);
//  int nStepsY = nStepsPerRotY * abs(static_cast<float>(degY) / 360.0);
  long nStepsX = round(nStepsPerRotX * abs(degX / 360.0));
  long nStepsY = round(nStepsPerRotY * abs(degY / 360.0));

  // Determine intervals for each motor based on speed
  long intervalX = (6e7 / speedX) / (double)nStepsPerRotX;
  long intervalY = (6e7 / speedY) / (double)nStepsPerRotY;

  // Initialize step counters and timing variables
  unsigned long stepsX = 0;
  unsigned long stepsY = 0;
  unsigned long previousTimeX = micros();
  unsigned long previousTimeY = micros();

  while (stepsX < nStepsX || stepsY < nStepsY)
  {
    unsigned long currentTime = micros();
    bool skipCompensation = false;

    // Check if it's time to move motor X
    if ((currentTime - previousTimeX > intervalX) && (stepsX < nStepsX))
    {
      // Move X motor
      xClockwise(xClock);
      digitalWrite(stepPinX, LOW);
      digitalWrite(stepPinX, HIGH);
      previousTimeX = currentTime;
      stepsX++;

      // Compensation logic: Skip compensation when both axes move, Y motor moves in opposite direction to X
      if (!(stepsY < nStepsY && (currentTime - previousTimeY > intervalY) && yClock != xClock))
      {
        // Move Y motor to compensate
        yClockwise(!xClock);
        digitalWrite(stepPinY, LOW);
        digitalWrite(stepPinY, HIGH);
      }
    }

    // Check if it's time to move motor Y
    if ((currentTime - previousTimeY > intervalY) && (stepsY < nStepsY))
    {
      // Move Y motor
      yClockwise(yClock);
      digitalWrite(stepPinY, LOW);
      digitalWrite(stepPinY, HIGH);
      previousTimeY = currentTime;
      stepsY++;
    }
  }

  delay(finalDelay); // Final delay to settle motors
}

void RPM()
{
  spin_degs(random(-360, 360), random(-360, 360));
}

// Unit tests

void calibrate_pulley_teeth()
{
  spin_degs(90.0, 0);
  spin_degs(-90.0, 0);
  spin_degs(0, 90.0);
  spin_degs(0, -90.0);
}

void test_spin_degs_multi()
{
  spin_degs(90, -90);
  spin_degs(-90, 90);
  spin_degs(180, -180);
  spin_degs(-180, 180);
  spin_degs(360, 360);
  spin_degs(-360, -360);
}

void diagnose_rotation_error() {
  Serial.println("Starting rotation diagnostic...");
  
  // Step 1: Test X-Axis Rotation
  Serial.println("Rotating X-Axis by 360°...");
  spin_degs(360, 0);
  delay(1000); // Wait for movement to stop
  
  // Step 2: Test Y-Axis Rotation
  Serial.println("Rotating Y-Axis by 360°...");
  spin_degs(0, 360);
  delay(1000); // Wait for movement to stop

  // Step 3: Ask User to Measure Actual Y-Axis Rotation
  Serial.println("\n⚠️  Please measure how much the Y-axis actually rotated.");
  Serial.println("  - If it moved LESS than 360°, divide 360 by actual movement.");
  Serial.println("  - Example: If Y moved only 240°, then correction = 360 / 240 = 1.5.");
  Serial.println("\nEnter measured Y rotation in degrees: ");
  
  while (Serial.available() == 0) {
    // Wait for user input
  }

  float measuredY = Serial.parseFloat(); // Read user input
  float correctionFactor = 360.0 / measuredY;

  Serial.println("\n✅ Diagnostic Complete!");
  Serial.print("  - Measured Y Rotation: "); Serial.print(measuredY); Serial.println("°");
  Serial.print("  - Suggested Correction Factor: "); Serial.println(correctionFactor);
  Serial.print("  - Suggested yRatio: "); Serial.print(yRatio * correctionFactor); Serial.println("\n");
}

void setup()
{
  pinMode(buttonPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(stepPinX, OUTPUT);
  pinMode(dirPinX, OUTPUT);
  pinMode(stepPinY, OUTPUT);
  pinMode(dirPinY, OUTPUT);
  pinMode(enablePinX, OUTPUT);
  pinMode(enablePinY, OUTPUT);
  Serial.begin(9600);
  Serial.print("--\nHello from OpenClino.\n--");
  wake(false, false);
}

void loop()
{
  buttonState = digitalRead(buttonPin);

  // Turn off motors when button is not pressed.
  wake(false, false);

  if (buttonState == HIGH)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    wake(true, true);
    // diagnose_rotation_error();
    // calibrate_pulley_teeth();
    // test_spin_degs_multi();
    // uncomment to run continuous spin
     spin_continuous(20, 20);
    // uncomment to run RPM
    // RPM();
  }
}
