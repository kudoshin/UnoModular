int interrupt = 1; //digital pin 3
int pinOUT = 12;
int pinIN = 8;
int delayVar[7] = {400, 500, 600, 700, 800, 900, 0};
int count = 0;
volatile int state = LOW;

void setup()
{
  Serial.begin(9600);
  pinMode(pinOUT, OUTPUT);
  pinMode(pinIN, INPUT);
  //digitalWrite(pinOUT, LOW);
  attachInterrupt(interrupt, sequence, RISING);
}
  
void loop()
{
  state =  !state;
  digitalWrite(pinOUT, state);
  delayMicroseconds(delayVar[count]);
}

void sequence()
{
  /*if (count < 3 ) 
    count++;
  else count = 0;*/

  count = (int)random(7);
  Serial.println(delayVar[count]);
}
