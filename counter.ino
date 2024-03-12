// button 1 - add one
// button 2 - add ten

byte counter;
const byte button1Pin = 24;
const byte button10Pin = 25;

void setup() {
  DDRB = 255;
  counter = 0;

  pinMode(button1Pin, INPUT_PULLUP); // A0
  pinMode(button10Pin, INPUT_PULLUP); // A1
}

void loop() {
  byte button1State = PINA & 0b00000001;
  byte button10State = PINA & 0b00000010;

  if ((button1State == LOW) && (button10State == HIGH)) {
    ++counter;
    delay(200);
  }

  if ((button10State == LOW) && (button1State == HIGH)) {
    counter += 10;
    delay(200);
  }

  PORTB = ~counter;
}
