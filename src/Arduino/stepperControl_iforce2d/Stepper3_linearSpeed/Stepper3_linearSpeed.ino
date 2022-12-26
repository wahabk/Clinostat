
#define DIR_PIN          2
#define STEP_PIN         3
#define ENABLE_PIN       4

void setup() {
  pinMode(STEP_PIN,   OUTPUT);
  pinMode(DIR_PIN,    OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
}

#define STEPS 400

void constantAccel() {
  int delays[STEPS];
  float angle = 1;
  float accel = 0.01;
  float c0 = 2000 * sqrt( 2 * angle / accel ) * 0.67703;
  float lastDelay = 0;
  int highSpeed = 100;
  for (int i = 0; i < STEPS; i++) {
    float d = c0;
    if ( i > 0 )
      d = lastDelay - (2 * lastDelay) / (4 * i + 1);
    if ( d < highSpeed )
      d = highSpeed;
    delays[i] = d;
    lastDelay = d;
  }

  // use delays from the array, forward
  for (int i = 0; i < STEPS; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds( delays[i] );
    digitalWrite(STEP_PIN, LOW);
  }

  // use delays from the array, backward
  for (int i = 0; i < STEPS; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds( delays[STEPS-i-1] );
    digitalWrite(STEP_PIN, LOW);
  }
}

void loop() {

  digitalWrite(DIR_PIN, LOW);
  constantAccel();
  digitalWrite(DIR_PIN, HIGH);
  constantAccel();

  while (true);
}








