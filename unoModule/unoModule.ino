#include <Time.h>

#define LEN 16
#define N 7

//pins
int pLogicOut = 8;
int pLogicA = 6;
int pLogicB = 7;
int pIntClockOut = 10;
int pExtClockIn = 3;
int pDivClockOut = 4;
int pDivider = 0;
int pSwitch1 = 0;
int pSwitch2 = 1;
int pPot1 = 0;
int pPot2 = 1;
int FMin = 2;

//variables
int interrupt = 1; //digital pin 3
int index = 0; //index to choose which delayVar value
int clockDivider = 0; //divider for clock in
int delayVar[N] = {400, 500, 600, 700, 800, 900, 1000}; //different freq
int melody[LEN];
int notes[N];
int noise[N];
double timbre[N];
int count = 0; //counter for clock
volatile int state = LOW;
int intClockState = LOW;
int clockHappened = 0;
int logicOperator = 0;
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
  pinMode(pSwitch1, INPUT);
  pinMode(pSwitch2, INPUT);
  //other setup
  Serial.begin(9600);
  attachInterrupt(interrupt, clockPulse, RISING);
  randomSeed(now());

  restGen(notes);
  melodyGen(melody);
  noiseGen(noise);
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

    logicOperator = digitalRead(pSwitch1) + (digitalRead(pSwitch2)<<1);
    logicOut = logicOp(logicA, logicB, logicOperator);
    //Serial.println(logicOut);
    digitalWrite(pLogicOut, logicOut);
 
    if (notes[index])
    {
        if (noise[index])
        {
          while(noise[index])
          {
             float offset = (analogRead(pPot2) / 1024.);
              tone(11, (melody[index] *( offset * 2) ) +digitalRead(FMin) * 300);
          }
        }
        else
        {
          float offset = (analogRead(pPot2) / 1024.);
          tone(11, (melody[index] *( offset * 2)));
        }
    }
    else
    {
      noTone(11);
    }
   
   
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
    index = (index+1)%LEN;
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
      notes[i] = 0;
    else notes[i] = 1;
  }
}

void noiseGen(int *noise)
{
  for(int i=0; i<N; i++)
  {
    if(random(5) < 4)
      noise[i] = 1;
    else noise[i] = 0;
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
