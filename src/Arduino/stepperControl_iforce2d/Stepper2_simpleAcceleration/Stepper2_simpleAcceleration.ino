#define DIR_PIN          2
#define STEP_PIN         3
#define ENABLE_PIN       4

void setup() {
  pinMode(STEP_PIN,   OUTPUT);
  pinMode(DIR_PIN,    OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
}

void simpleAccel(int steps) {

  int lowSpeed = 2000;
  int highSpeed = 100;
  int change = 2;

  int rampUpStop = (lowSpeed - highSpeed) / change;
  if ( rampUpStop > steps / 2 )
    rampUpStop = steps / 2;
  int rampDownStart = steps - rampUpStop;

  int d = lowSpeed;

  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(d);

    if ( i < rampUpStop )
      d -= change;
    else if ( i > rampDownStart )
      d += change;
  }
}

void loop() {
  digitalWrite(DIR_PIN, LOW);
  simpleAccel(2400);
  digitalWrite(DIR_PIN, HIGH);
  simpleAccel(2400);

  while (true);

}
