#define DIR_PIN          2
#define STEP_PIN         3
#define ENABLE_PIN       4

void setup() {
  pinMode(STEP_PIN,   OUTPUT);
  pinMode(DIR_PIN,    OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
}

void simpleMove(int steps) {
  int interval = 100;
  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(interval);
  }
}

void loop() {
  digitalWrite(DIR_PIN, LOW);
  simpleMove(800);
  digitalWrite(DIR_PIN, HIGH);
  simpleMove(800);

  while (true);
}








