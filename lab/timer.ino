const byte timer_button = 16;

void setup() {
  DDRA = 255; // port a ustawiony na wyjście - SEGMENTY
  PORTA = 0;
 
  DDRB = 255;
  PORTB = 0;
 
  pinMode(timer_button, INPUT_PULLUP); // C0
 
  Serial.begin(9600); // 9600 - bits per sec - port szeregowy
}

const byte A = 1 << 0; // top
const byte B = 1 << 1; // top right
const byte C = 1 << 2; // bottom right
const byte D = 1 << 3; // bottom
const byte E = 1 << 4; // bottom left
const byte F = 1 << 5; // top left
const byte G = 1 << 6; // middle
const byte H = 1 << 7; // dot

const byte number_segments[] = {
  A|B|C|D|E|F,    // 0 -> 63
  B|C,            // 1 -> 6
  A|B|G|E|D,      // 2
  A|B|G|C|D,      // 3
  F|G|B|C,        // 4
  A|F|G|C|D,      // 5
  A|F|E|D|C|G,    // 6
  A|B|C,          // 7
  A|B|C|D|E|F|G,  // 8
  A|F|G|B|C       // 9
};

void display_number(int dec) {
  unsigned long start = 0;
  unsigned long stop_ = 0;
  unsigned long elapsed_time = 0;
  byte started = false;
 

  while (Serial.available() <= 0) {
   
    byte timer_button_state = PINC & 0b00000001; // readDigital()?
    Serial.println(timer_button_state);
    if (timer_button_state == LOW && started == false) { // HIGH?
      started = true;
      start = millis();
    }

   
    Serial.println(dec);  
    const byte split_digits[] = {dec % 10, (dec / 10) % 10, (dec / 100) % 10, dec / 1000};
 
    for (byte i = 0; i <= 3; ++i) {
      byte segments = number_segments[split_digits[i]];
      Serial.println(split_digits[i]);
     
      if (i == 2) {
        segments |= H; // decimal point
      }
     
      PORTB = ~(1 << i);
      PORTA = ~segments;
      // delay(1);
    }

    stop_ = millis();
    elapsed_time = stop_ - start;
   
   
    Serial.println(elapsed_time);
    if (elapsed_time >= 1000 && dec > 0) { // && started == true
      --dec;
      start = stop_;
    }
   
  }
  PORTB = 255;
 
}

const int default_ = 0;

void loop() {
  if (Serial.available() > 0) {
    display_number(Serial.parseInt());
  }
  display_number(default_);
}


// 4 cyfry, kropka po srodku
// odliczanie do 0 po kliknieciu przycisku
