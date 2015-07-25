
#define LEN 16
#define N 7

//pins
int pLogicOut = 8;
int pLogicA = 6;
int pLogicB = 7;
int pIntClockOut = 10;
int pExtClockIn = 3;
int pDivClockOut = 4;
int pDivider = 5;

//variables
int interrupt = 1; //digital pin 3
int index = 0; //index to choose which delayVar value
int clockDivider = 0; //divider for clock in
int delayVar[N] = {400, 500, 600, 700, 800, 900, 1000}; //different freq
int melody[LEN];
int notes[N];
double timbre[N];
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
  pinMode(9,OUTPUT);
  pinMode(11, OUTPUT);
  //other setup
  Serial.begin(9600);
  attachInterrupt(interrupt, clockPulse, RISING);
  randomSeed(millis());

  restGen(notes);
  melodyGen(melody);
}
  
void loop()
{
  

  //clock divider
  clockDivider = analogRead(pDivider);
  clockDivider = clockDivider/100; //adjust 

  
  if (clockHappened){
    state = !state;
    digitalWrite(pDivClockOut, state);
    clockHappened = 0;

    logicA = digitalRead(pLogicA);
    logicB = digitalRead(pLogicB);
  
    logicOut = logicOp(logicA, logicB, logicOperator);
    //Serial.println(logicOut);
    digitalWrite(pLogicOut, logicOut);
  
    if (notes[index])
    {
      tone(11, melody[index]);
    }
    else 
    {
      noTone(11);
    }
    
    index = (index+1)%LEN;// = (int)random(7);
  }  
  
  intClockState = !intClockState;
  digitalWrite(pIntClockOut, intClockState);
  delay(50);
}

void clockPulse()
{
  if (count < clockDivider)
    count++;
  else
  {
    count = 0;
    clockHappened = 1;
  }
  //Serial.println(clockDivider);
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

void restGen(int *notes)
{
  for(int i=0; i<N; i++)
  {
    if(random(5) < 4)
      notes[i] = 1;
    else notes[i] = 0;
  }
}

void timbreGen(double *timbre)
{
  for(int i=0; i<N; i++)
  {
    timbre[i] = random(100)/100.;
  }
}

void melodyGen(int *melody)
{
  for(int i=0; i<LEN; i++)
  {
    melody[i] = delayVar[random(7)];
  }
}

