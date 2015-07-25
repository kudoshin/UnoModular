int pin = 6;
int light;
int lightIn = 4;
int state = LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  light = analogRead(lightIn);
  int tempo = (int)(light-200)*1.5;
  tempo = tempo < 0 ? 0 : tempo;
  Serial.println(light);
  digitalWrite(pin,state);
  state = !state;
  delay(tempo);
}
