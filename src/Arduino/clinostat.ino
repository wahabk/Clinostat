// Define steps per revolution
const int motorSteps = 200;
const int subStep = 16;
const unsigned long stepsPerRevolution = motorSteps * subStep;


//pins
const int buttonPin = 2;
const int enablePinX = 3;
const int stepPinX = 4;
const int dirPinX = 5;
const int enablePinY = 6;
const int stepPinY = 7;
const int dirPinY = 8;

// pulley diameters
// try n teeth
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

void wake(bool x, bool y) {
  if (x == true) {digitalWrite(enablePinX, LOW);}
  else           {digitalWrite(enablePinX, HIGH);}
  if (y == true) {digitalWrite(enablePinY, LOW);}
  else           {digitalWrite(enablePinY, HIGH);}
}

void xClockwise(bool foo) {
  if (foo == true) {digitalWrite(dirPinX, HIGH);}
  else             {digitalWrite(dirPinX, LOW);}
}

void yClockwise(bool foo) {
  if (foo == true) {digitalWrite(dirPinY, LOW);}
  else             {digitalWrite(dirPinY, HIGH);}
}

//function to continuous spin x and y given <run_time> and rpm where if 0 keep still
//function to spin <degX, degY> and <rpms>
//void spin_degs():
//think about smoothing

void spin_continuous(int t = 1, float speedX = 10, float speedY = 10, int finalDelay=100) {
  bool xClock;
  bool yClock;

  if (speedX >= 0) {xClock = true;}
  else           {xClock = false;}
  if (speedY >= 0) {yClock = true;}
  else           {yClock = false;}

  long IntervalX =  (6e7 / speedX) / nStepsXperRot;
  long IntervalY =  (6e7 / speedY) / nStepsYperRot;
  unsigned long stepsY = 0;
  unsigned long stepsX = 0;
  unsigned long previousTimeX = micros();
  unsigned long previousTimeY = micros();

  while (true) {
    unsigned long currentTimeX = micros();
    unsigned long currentTimeY = micros();
    bool xGo = false; // whether to step x
    bool yGo = false; // whether to step y
    bool skipCompensation = false; 
    
    digitalWrite(stepPinX, HIGH);
    digitalWrite(stepPinY, HIGH);
    
    if (currentTimeX - previousTimeX > IntervalX) {xGo = true;}
    if (currentTimeY - previousTimeY > IntervalY) {yGo = true;}
    if (yGo == true && xGo == true && yClock == false && xClock != yClock) {skipCompensation = true;}

    if (xGo) {
      xClockwise(xClock);
      digitalWrite(stepPinX, LOW);
      digitalWrite(stepPinX, HIGH);
      previousTimeX = currentTimeX;

      if (skipCompensation == false) {
        yClockwise(!xClock); // spin y motor with x
  
        digitalWrite(stepPinY, LOW); //compensator
        digitalWrite(stepPinY, HIGH);
      }
      stepsX++;
    }

    if (yGo) {
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

void spin_degs(float degX, float degY, float speedX = 60, float speedY = 60, int finalDelay = 100) {
//  wake(true, true);

  // define directions because y motor has to constantly change direction to compensate
  bool xClock;
  bool yClock;

  if (degX >= 0) {xClock = true;}
  else           {xClock = false;}
  if (degY >= 0) {yClock = true;}
  else           {yClock = false;}

  int nStepsX = nStepsXperRot * abs(degX / 360.); // define fraction of full turn
  int nStepsY = nStepsYperRot * abs(degY / 360.);

  bool keepGoing = true;

  long IntervalX =  (6e7 / speedX) / nStepsXperRot;
  long IntervalY =  (6e7 / speedY) / nStepsYperRot;
  unsigned long stepsY = 0;
  unsigned long stepsX = 0;
  unsigned long previousTimeX = micros();
  unsigned long previousTimeY = micros();
  
  while (keepGoing) {
    unsigned long currentTimeX = micros();
    unsigned long currentTimeY = micros();
    bool xGo = false; // whether to step x
    bool yGo = false; // whether to step y
    bool skipCompensation = false; 
    
    digitalWrite(stepPinX, HIGH);
    digitalWrite(stepPinY, HIGH);
    
    if (currentTimeX - previousTimeX > IntervalX && stepsX <= nStepsX) {xGo = true;}
    if (currentTimeY - previousTimeY > IntervalY && stepsY <= nStepsY) {yGo = true;}
    if (yGo == true && xGo == true && yClock == false && xClock != yClock) {skipCompensation = true;}

    if (xGo) {
      xClockwise(xClock);
      digitalWrite(stepPinX, LOW);
      digitalWrite(stepPinX, HIGH);
      previousTimeX = currentTimeX;

      if (skipCompensation == false) {
        yClockwise(!xClock); // spin y motor with x
  
        digitalWrite(stepPinY, LOW); //compensator
        digitalWrite(stepPinY, HIGH);
      }
      stepsX++;
    }

    if (yGo) {
      yClockwise(yClock);
      digitalWrite(stepPinY, LOW);
      digitalWrite(stepPinY, HIGH);
      previousTimeY = currentTimeY;
      stepsY++;
    }
    if (stepsX >= nStepsX && stepsY >= nStepsY) {keepGoing = false;} // check if finished
  }
  delay(finalDelay);
}

void show_off(){
  spin_degs(180, 180);
  spin_degs(-180, -180);
  spin_degs(-180, 180);
  spin_degs(180, -180);
}

void RPM(){
  spin_degs(random(-360,360), random(-360,360));
}

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(stepPinX, OUTPUT);
  pinMode(dirPinX, OUTPUT);
  pinMode(stepPinY, OUTPUT);
  pinMode(dirPinY, OUTPUT);
  pinMode(enablePinX, OUTPUT);
  pinMode(enablePinY, OUTPUT);
  Serial.begin(9600);
  Serial.print("--\nHello\n--");

  wake(false, false);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  wake(false, false);

  if (buttonState == HIGH) {
    digitalWrite(LED_BUILTIN, HIGH);
    wake(true, true);
    spin_continuous(1,60,60);
//    show_off();
//    RPM();
  }

  digitalWrite(LED_BUILTIN, LOW);
  delay(10); // Wait a second
}
