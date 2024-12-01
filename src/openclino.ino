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
// eposilon to fix discrepancy between motor substeps
// set to 1 to undo calibration.
const double epsilon = 1.5;

// Pulley teeth
const float xMotTeeth = 16;
const float xPulleyTeeth = 50;
const float yMotTeeth = 16;
const float yTurnTeeth = 50;
const float yPulleyTeeth = 20;

// Calculate the gear ratios based on pulley teeth
const double xRatio = xPulleyTeeth / xMotTeeth;
const double yRatio1 = yTurnTeeth / yMotTeeth;
const double yRatio2 = yPulleyTeeth / yTurnTeeth;
const double yRatio = yRatio1 * yRatio2 * epsilon;

// Calculate effective steps per rotation
const double nStepsPerRotX = stepsPerRevolution * xRatio;
const double nStepsPerRotY = stepsPerRevolution * yRatio;

int buttonState = 0;

// Define AccelStepper objects for X and Y axes
AccelStepper stepperX(1, stepPinX, dirPinX);
AccelStepper stepperY(1, stepPinY, dirPinY);

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

  long IntervalX = (6e7 / speedX) / nStepsPerRotX;
  long IntervalY = (6e7 / speedY) / nStepsPerRotY;
  unsigned long stepsY = 0;
  unsigned long stepsX = 0;
  unsigned long previousTimeX = micros();
  unsigned long previousTimeY = micros();

  while (true)
  {
    unsigned long currentTimeX = micros();
    unsigned long currentTimeY = micros();
    bool xGo = false; // whether to step x
    bool yGo = false; // whether to step y
    bool skipCompensation = false;

    digitalWrite(stepPinX, HIGH);
    digitalWrite(stepPinY, HIGH);

    if (currentTimeX - previousTimeX > IntervalX)
    {
      xGo = true;
    }
    if (currentTimeY - previousTimeY > IntervalY)
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
      previousTimeX = currentTimeX;

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
      previousTimeY = currentTimeY;
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
  int nStepsX = nStepsPerRotX * abs(degX / 360.0);
  int nStepsY = nStepsPerRotY * abs(static_cast<float>(degY) / 360.0);

  // Determine intervals for each motor based on speed
  long intervalX = (6e7 / speedX) / nStepsPerRotX;
  long intervalY = (6e7 / speedY) / nStepsPerRotY;

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
    calibrate_pulley_teeth();
    test_spin_degs_multi();
    // uncomment to run continuous spin
    // spin_continuous(4, 20);
    // uncomment to run RPM
    // RPM();
  }
}
