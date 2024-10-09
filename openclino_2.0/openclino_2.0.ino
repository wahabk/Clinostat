// Define steps per revolution
const int StepperSteps = 200;
const int subStep = 16;
const unsigned long stepsPerRevolution = StepperSteps * subStep;

// pins o
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
    digitalWrite(stepPin, HIGH);
    digitalWrite(stepPin, LOW);
  }

private:
  int stepPin;
  int dirPin;
  int enablePin;
  int nStepsPerRot;
};

void setup()
{
  Stepper motorX(stepPinX, dirPinX, enablePinX, nStepsXperRot);
  Stepper motorY(stepPinY, dirPinY, enablePinY, nStepsYperRot);
  
  pinMode(buttonPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Serial.print("--\nHello\n--");

  motorX.wake(false);
  motorY.wake(false);
}

void loop()
{
//  buttonState = digitalRead(buttonPin);
//
//  wake(false, false);
//
//  if (buttonState == HIGH)
//  {
//    digitalWrite(LED_BUILTIN, HIGH);
//    wake(true, true);
//    spin_continuous(1, 60, 60);
//    //  show_off();
//    //  RPM();
//  }
//
//  digitalWrite(LED_BUILTIN, LOW);
//  delay(10); // Wait a second
}
