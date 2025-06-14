/*
 * OpenClino
 * 
 * X is the outer frame.
 * Y is the inner frame.
 */

// 🛠 Motor & Button Pins
const int buttonPin = 2;
const int enablePinX = 3;
const int stepPinX = 4;
const int dirPinX = 5;
const int enablePinY = 6;
const int stepPinY = 7;
const int dirPinY = 8;
int buttonState = 0;

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
const double xRatio = xPulleyTeeth / xMotTeeth;
const double yRatio1 = yTurnTeeth / yMotTeeth;
const double yRatio2 = yPulleyTeeth / yTurnTeeth;
double yCorrection = 1.5; // Found from calibration using calibrate_y_correction()
double yRatio = yRatio1 * yRatio2 * yCorrection;

// Calculate effective steps per rotation
const double nStepsPerRotX = stepsPerRevolution * xRatio;
double nStepsPerRotY = stepsPerRevolution * yRatio;

// 🏁 Enable or disable motors
void enable(bool enableX, bool enableY) {
    digitalWrite(enablePinX, enableX ? LOW : HIGH);  // LOW = Enabled, HIGH = Disabled
    digitalWrite(enablePinY, enableY ? LOW : HIGH);
}

// functions
void spin_degs(float Q1_target, float Q2_target, float maxRPM = 10, int finalDelay = 100) {
    // 🎯 Compute target step movement for X-axis
    long stepsX = round((Q1_target / 360.0) * nStepsPerRotX);
    
    // 🔄 Compute Y-axis movement with compensation to align timing
    long rawStepsY = round((Q2_target / 360.0) * nStepsPerRotY);
    long compensatedStepsY = rawStepsY - stepsX;  

    // Flip Y direction
    compensatedStepsY = -compensatedStepsY;

    // Normalize motion so that both axes complete at the same time
    long maxSteps = max(abs(stepsX), abs(compensatedStepsY));
    float ratioX = (maxSteps > 0) ? abs(stepsX) / (float)maxSteps : 0;
    float ratioY = (maxSteps > 0) ? abs(compensatedStepsY) / (float)maxSteps : 0;

    // Set direction
    digitalWrite(dirPinX, (stepsX > 0) ? HIGH : LOW);
    digitalWrite(dirPinY, (compensatedStepsY > 0) ? HIGH : LOW);

    // Convert max RPM to step delay
    float stepsPerSecond = (maxRPM / 60.0) * max(nStepsPerRotX, nStepsPerRotY);
    float stepDelay = (1.0 / stepsPerSecond) * 1000000.0;

    Serial.print("stepsX: ");
    Serial.println(stepsX);
    Serial.print("compensatedStepsY: ");
    Serial.println(compensatedStepsY);

    // Step both motors proportionally
    long stepCountX = 0, stepCountY = 0;
    for (long i = 0; i < maxSteps; i++) {
        if (stepCountX < abs(stepsX) && (i * ratioX) >= stepCountX) {
            digitalWrite(stepPinX, HIGH);
            digitalWrite(stepPinX, LOW);
            stepCountX++;
        }

        if (stepCountY < abs(compensatedStepsY) && (i * ratioY) >= stepCountY) {
            digitalWrite(stepPinY, HIGH);
            digitalWrite(stepPinY, LOW);
            stepCountY++;
        }

        delayMicroseconds(stepDelay);
    }
    delay(finalDelay);
    Serial.println("Motion complete.\n");
}

void spin_continuous(float speedX = 10, float speedY = 10, int buttonDelay = 1000) {
    // Convert RPM to steps per second
    float stepsPerSecondX = (speedX / 60.0) * nStepsPerRotX;
    float rawStepsPerSecondY = (speedY / 60.0) * nStepsPerRotY;
    float compensatedStepsPerSecondY = rawStepsPerSecondY - stepsPerSecondX;

    // Set motor direction, both should always spin clockwise
    digitalWrite(dirPinX, HIGH);
    digitalWrite(dirPinY, (compensatedStepsPerSecondY > 0) ? LOW : HIGH);
    
    stepsPerSecondX = fabs(stepsPerSecondX);
    compensatedStepsPerSecondY = fabs(compensatedStepsPerSecondY);

    // Compute step delays
    float stepDelayX = (stepsPerSecondX > 0) ? (1.0 / stepsPerSecondX) * 1e6 : 0;
    float stepDelayY = (compensatedStepsPerSecondY > 0) ? (1.0 / compensatedStepsPerSecondY) * 1e6 : 0;

    Serial.println("Starting continuous motion...");
    Serial.print("Step Delay X: "); Serial.println(stepDelayX);
    Serial.print("Step Delay Y: "); Serial.println(stepDelayY);

    delay(buttonDelay);
    buttonState = digitalRead(buttonPin);

    // Track last step times
    unsigned long lastStepTimeX = micros();
    unsigned long lastStepTimeY = micros();

    while (buttonState == LOW) {
        buttonState = digitalRead(buttonPin);
        unsigned long now = micros();

        // Step X motor if delay has passed
        if (stepsPerSecondX > 0 && (now - lastStepTimeX >= stepDelayX)) {
            digitalWrite(stepPinX, HIGH);
            digitalWrite(stepPinX, LOW);
            lastStepTimeX = now;
        }

        // Step Y motor if delay has passed
        if (compensatedStepsPerSecondY > 0 && (now - lastStepTimeY >= stepDelayY)) {
            digitalWrite(stepPinY, HIGH);
            digitalWrite(stepPinY, LOW);
            lastStepTimeY = now;
        }
    }

    delay(buttonDelay);
    Serial.println("Continuous motion stopped.");
}


// calibration
void calibrate_y_correction()
{
  Serial.println("Starting rotation diagnostic...");
  Serial.println("Resetting Y-axis correction factor to 1.0.");
  yCorrection = 1.0;
  yRatio = yRatio1 * yRatio2 * yCorrection;
  nStepsPerRotY = stepsPerRevolution * yRatio;

  // Step 2: Test Y-Axis Rotation
  Serial.println("Rotating Y-Axis by 360°...");
  spin_degs(0, 360);
  delay(1000); // Wait for movement to stop

  // Step 3: Ask User to Measure Actual Y-Axis Rotation
  Serial.println("⚠ Please enter how much the Y-axis actually rotated in degrees:");

  while (Serial.available() == 0) {} // Wait for user input

  // Rotate back to 0 to reset before measuring correction
  spin_degs(0, -360);

  float measuredY = Serial.parseFloat(); // Read user input

  if (measuredY == 0.0) {
    Serial.print("\n❌ Error, exiting calibration. You entered: "); 
    Serial.println(measuredY);
    return;
  }
  
  yCorrection = 360.0 / measuredY;
  yRatio = yRatio1 * yRatio2 * yCorrection;
  nStepsPerRotY = stepsPerRevolution * yRatio;

  Serial.println("\n✅ Calibration Complete!");
  Serial.print("  - Measured Y Rotation: ");
  Serial.print(measuredY);
  Serial.println("° / 360°");
  Serial.print("  - Suggested Correction Factor: ");
  Serial.println(yCorrection);
  Serial.print("  - Suggested yRatio: ");
  Serial.print(yRatio);
  Serial.println("\n");
}

// Unit tests
void test_pulley_ratios()
{
  Serial.println("Testing pulley ratios, outer frame (X) should rotate 360° and back.");
  spin_degs(360.0, 0);
  spin_degs(-360.0, 0);
  Serial.println("Inner frame should rotate 360° and back.");
  spin_degs(0, 360.0);
  spin_degs(0, -360.0);
}

void test_spin_degs_multi()
{
  Serial.println("--\nTesting spin_degs.\n--");
  Serial.println("90°");
  spin_degs(90, 90);
  spin_degs(-90, -90);
  Serial.println("180°");
  spin_degs(180, 180);
  spin_degs(-180, -180);
  Serial.println("360°");
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
  Serial.print("--\nHello from OpenClino.\n--\n");
  // print gear ratios and nStepsPerRotX, nStepsPerRotY
  Serial.print("xRatio: ");
  Serial.println(xRatio);
  Serial.print("yRatio: ");
  Serial.println(yRatio);
  Serial.print("nStepsPerRotX: ");
  Serial.println(nStepsPerRotX);
  Serial.print("nStepsPerRotY: ");
  Serial.println(nStepsPerRotY);
  Serial.print("--\n");

  enable(false, false);
}

void loop()
{
  buttonState = digitalRead(buttonPin);

  // Disable motors when button is not pressed
  enable(false, false);

  if (buttonState == HIGH)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    // Enable motors
    enable(true, true); 
    // Run calibration procedure
    // please open your Serial Monitor and follow the instructions.
    calibrate_y_correction();
    // These functions will run a test procedure.
    // Open the Serial Monitor and ensure the 
    // frames are rotating correctly.
    test_pulley_ratios();
    test_spin_degs_multi();
    // Uncomment to run continuous spin at 10 rpm for both axes
    // spin_continuous(10, 10);
  }
}
