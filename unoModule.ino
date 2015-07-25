//pins
int pLogicOut = 8;
int pLogicA = 6;
int pLogicB = 7;
int pIntClockOut = 10;
int pExtClockIn = 11;
int pDivClockOut = 4;
int pDivider;

//variables
int interrupt = 1; //digital pin 3
int index = 0; //index to choose which delayVar value
int clockDivider = 0; //divider for clock in
int delayVar[7] = {400, 500, 600, 700, 800, 900, 0}; //different freq
int count = 0; //counter for clock
volatile int state = LOW;
int intClockState = LOW;
int clockHappened = 0;
int logicOperator = (int)random(4);
int logicA = 0;
int logicB = 0;
int logicOut = 0;

void setup()
{
  //set pin modes
  pinMode(pLogicOut, OUTPUT);
  pinMode(pIntClockOut, OUTPUT);
  pinMode(pDivClockOut, OUTPUT);
  pinMode(pLogicA, INPUT);
  pinMode(pLogicB, INPUT);
  pinMode(pExtClockIn, INPUT);
  pinMode(pDivider, INPUT);

  //other setup
  Serial.begin(9600);
  attachInterrupt(interrupt, clockPulse, RISING);
}
  
void loop()
{
  //clock divider
  clockDivider = analogRead(pDivider);
  clockDivider = clockDivider/200; //adjust 
    
  if (clockHappened && (count == clockDivider)){
    state = !state;
    digitalWrite(pDivClockOut, state);
    clockHappened = 0;

    logicA = digitalRead(pLogicA);
    logicB = digitalRead(pLogicB);
  
    logicOut = logicOp(logicA, logicB, logicOperator);
    digitalWrite(pLogicOut, logicOut); 
  
    index = (int)random(7);
  }  
  
  intClockState = !intClockState;
  digitalWrite(pIntClockOut, intClockState);
}

void clockPulse()
{
  if (count < clockDivider)
    count++;
  else count = 0;
  
  clockHappened = 1;
}

int logicOp(int a, int b, int op)
{
  switch(op)
  {
    case 1:
      return a & b;
    case 2:
      return a | b;
    case 3:
      return a ^ b;
  }
}
