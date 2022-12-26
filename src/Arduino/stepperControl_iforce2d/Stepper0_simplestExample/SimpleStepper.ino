#define DIR_PIN          2
#define STEP_PIN         3
#define ENABLE_PIN       4

void setup() {
  pinMode(DIR_PIN,    OUTPUT);
  pinMode(STEP_PIN,   OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW);
}

void loop() {
  digitalWrite(STEP_PIN, HIGH);
  digitalWrite(STEP_PIN, LOW);
  delay(1);
}

