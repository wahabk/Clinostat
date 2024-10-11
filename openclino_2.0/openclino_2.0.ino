// Define steps per revolution
const int StepperSteps = 200;
const int subStep = 16;
const unsigned long stepsPerRevolution = StepperSteps * subStep;

// pins
const int buttonPin = 2;
const int enablePinX = 3;
const int stepPinX = 4;
const int dirPinX = 5;
const int enablePinY = 6;
const int stepPinY = 7;
const int dirPinY = 8;

// pulley diameters
const float xMotD = 9.5;
const float xPulleyD = 31.5;
const float yMotD = 9.5;
const float yTurnD = 31.5;
const float yPulleyD = 18.5;
const double xRatio = xMotD / xPulleyD; // diameters
const double yRatio = yMotD / yPulleyD;
const int nStepsXperRot = stepsPerRevolution / xRatio;
const int nStepsYperRot = stepsPerRevolution / yRatio;

int buttonState = 0;

class Stepper
{
public:
  Stepper(int stepPin, int dirPin, int enablePin, int nStepsPerRot)
  {
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    this->enablePin = enablePin;
    this->nStepsPerRot = nStepsPerRot;
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(enablePin, OUTPUT);
    pinMode(stepPin, OUTPUT);
  }

  void wake(bool enable)
  {
    if (enable)
    {
      digitalWrite(enablePin, LOW);
    }
    else
    {
      digitalWrite(enablePin, HIGH);
    }
  }

  void setDirection(bool clockwise)
  {
    if (clockwise)
    {
      digitalWrite(dirPin, HIGH);
    }
    else
    {
      digitalWrite(dirPin, LOW);
    }
  }

  void step()
  {
    digitalWrite(stepPin, LOW);
    digitalWrite(stepPin, HIGH);
  }

private:
  int stepPin;
  int dirPin;
  int enablePin;
  int nStepsPerRot;
};

// Define the motors globally
Stepper motorX(stepPinX, dirPinX, enablePinX, nStepsXperRot);
Stepper motorY(stepPinY, dirPinY, enablePinY, nStepsYperRot);

void setup()
{
  pinMode(buttonPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Serial.print("--\nHello\n--");
  Serial.println(nStepsXperRot);

  motorX.wake(false);
  motorY.wake(false);
}

void loop()
{
  buttonState = digitalRead(buttonPin);
  motorX.wake(false);
  motorY.wake(false);
  motorX.setDirection(false);
  motorY.setDirection(true);
  if (buttonState == HIGH)
  {
    int speedX = 80;
    digitalWrite(LED_BUILTIN, HIGH);
    unsigned long IntervalX = (6e7 / speedX) / nStepsXperRot;
    
    for (int i = 0; i < 10000; i++)
    {
      motorX.wake(true);
      motorY.wake(true);
      motorX.step();
      motorY.step();
//      Serial.print("Stepping - interval: ");
//      Serial.print(IntervalX);
//      Serial.print(" - step: ");
//      Serial.println(i);
      delayMicroseconds(IntervalX);
    }
  }

  digitalWrite(LED_BUILTIN, LOW);
  delay(10); // Wait a second
}
