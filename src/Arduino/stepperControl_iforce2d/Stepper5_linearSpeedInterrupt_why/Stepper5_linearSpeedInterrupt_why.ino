#define DIR_PIN          2
#define STEP_PIN         3
#define ENABLE_PIN       4

#define STEP_HIGH        PORTD |=  0b00001000;
#define STEP_LOW         PORTD &= ~0b00001000;

#define TIMER1_INTERRUPTS_ON    TIMSK1 |=  (1 << OCIE1A);
#define TIMER1_INTERRUPTS_OFF   TIMSK1 &= ~(1 << OCIE1A);

unsigned int c0;

void setup() {
  Serial.begin(9600);
  
  pinMode(STEP_PIN,   OUTPUT);
  pinMode(DIR_PIN,    OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 1000;                             
  TCCR1B |= (1 << WGM12);
  TCCR1B |= ((1 << CS11) | (1 << CS10));
  interrupts();

  c0 = 1600; //2000 * sqrt( 1 ) * 0.67703;
}

volatile int dir = 0;
volatile unsigned int maxSpeed = 80;
volatile unsigned long n = 0;
volatile float d;
volatile unsigned long stepCount = 0;
volatile unsigned long rampUpStepCount = 0;
volatile unsigned long totalSteps = 0;

volatile int stepPosition = 0;

volatile bool movementDone = false;

ISR(TIMER1_COMPA_vect)
{
  if ( stepCount < totalSteps ) {
    STEP_HIGH
    STEP_LOW
    stepCount++;
    stepPosition += dir;
  }
  else {
    movementDone = true;
    TIMER1_INTERRUPTS_OFF
  }

  if ( rampUpStepCount == 0 ) { // ramp up phase
    n++;
    d = d - (2 * d) / (4 * n + 1);
    if ( d <= maxSpeed ) {
      d = maxSpeed;
      rampUpStepCount = stepCount;
    }
    if ( stepCount >= totalSteps / 2 ) {
      rampUpStepCount = stepCount;
    }
  }
  else if ( stepCount >= totalSteps - rampUpStepCount ) { // ramp down phase
    n--;
    d = (d * (4 * n + 1)) / (4 * n + 1 - 2);
  }

  OCR1A = d;
}

void moveNSteps(int steps) {

  digitalWrite(DIR_PIN, steps < 0 ? HIGH : LOW);
  dir = steps > 0 ? 1 : -1;
  totalSteps = abs(steps);
  d = c0;
  OCR1A = d;
  stepCount = 0;
  n = 0;
  rampUpStepCount = 0;
  movementDone = false;

  TIMER1_INTERRUPTS_ON
}

void moveToPosition(int p, bool wait = true) {
  moveNSteps(p - stepPosition);
  while ( wait && ! movementDone );
}

void loop() {

  Serial.println("Starting...");

  moveToPosition( 8000, false );

  int count = 0;
  while ( !movementDone ) {    
    if ( count > 3 ) {
      movementDone = true;
      TIMER1_INTERRUPTS_OFF
    }
    else {
      Serial.print(count++);
      Serial.print(" ");
      Serial.println( stepPosition );
      delay(500);
    }
  }

  Serial.println("Finished.");

  Serial.println( stepPosition );

  while (true);

}








