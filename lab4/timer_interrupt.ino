const byte A = 1 << 0; // top
const byte B = 1 << 1; // top right
const byte C = 1 << 2; // bottom right
const byte D = 1 << 3; // bottom
const byte E = 1 << 4; // bottom left
const byte F = 1 << 5; // top left
const byte G = 1 << 6; // middle
const byte H = 1 << 7; // dot

const byte number_segments[] = {
  A|B|C|D|E|F,    // 0
  B|C,            // 1
  A|B|G|E|D,      // 2
  A|B|G|C|D,      // 3
  F|G|B|C,        // 4
  A|F|G|C|D,      // 5
  A|F|E|D|C|G,    // 6
  A|B|C,          // 7
  A|B|C|D|E|F|G,  // 8
  A|F|G|B|C       // 9
};

struct Timer {
  int mins;
  int secs;
};

struct Timer mins_secs(int num) {
  struct Timer t;
  
  if (num > 9999 or num < 0) {
    Serial.println("number too high / too low");
    t.mins = 0;
    t.secs = 0;
    return t;
  }
  
  int mins = int(num / 100);
  int secs = num % 100;
  
  if (mins > 59 or secs > 59) {
    Serial.println("incorrect format");
    t.mins = 0;
    t.secs = 0;
    return t;
  }
  
  t.mins = mins;
  t.secs = secs;
  return t;
}

int four_digits(int mins, int secs) {
  return 100 * mins + secs;
}

int decrement(int num) {
  struct Timer t = mins_secs(num);
  // Serial.println(mins, secs);
  if (t.secs > 0) {
    return four_digits(t.mins, t.secs - 1);
  } else if (t.mins > 0 && t.secs == 0) {
    return four_digits(t.mins - 1, 59);
  } else {
    return 0;
  }
}

void display_digit(int number, int digit) {
  const byte split_digits[] = {number % 10, (number / 10) % 10, (number / 100) % 10, number / 1000};

  byte segments = number_segments[split_digits[digit]];
     
  if (digit == 2) {
    segments |= H; // decimal point
  }
     
  PORTB = ~(1 << digit);
  PORTA = ~segments;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

int i = 0;
int start = 3101; // timer duration, countdown starts at 31:01
int digit = 0;

// zmienić na overflow vector przy mode 00
// bez void - pobranie adresu z początku funkcji, tworzy funkcje w pamieci i pobiera adres gdzie dal ja w pamieci, jakieś reti
// Interrupt Service Routing - definiujemy tu przerwanie. Funkcja ta jest automatycznie wołana przez procesor
ISR(TIMER0_COMP_vect) { 
  // 10000 przerwań na sekundę, więc po 10000 przerwaniach minie jedna sekunda i należy zaktualizować licznik
  if ((++i) == 10000) {
    i = 0;
    start = decrement(start);
  }
    
  // tylko jedna cyfra wyświetlana jest przy jednym przerwaniu, ponieważ
  // w przerwaniu mamy ograniczony czas, a i tak są one na tyle często
  // (10000 razy na sekundę), że działa to praktycznie jak pętla
  digit = (++digit) % 4;
  display_digit(start, digit);
}

void setup() {
  DDRA = 255; // port A ustawiony na wyjście - segmenty
  PORTA = 255; // początkowo nie zasilaj żadnego segmentu
 
  DDRB = 255; // port B ustawiony na wyjście - cyfry
  PORTB = 255; // początkowo nie zasilaj żadnej cyfry
  
  Serial.begin(9600); // 9600 - bits per sec - port szeregowy

  // potrzebne to, skoro zainicjalizowałem je wyżej?
  i = 0;
  digit = 0;
  start = 3101;
  
  TCNT0 = 0;
  OCR0 = 100; // 8bit
//  TCCR0 = 0b00001010;
  TCCR0 = (1<<WGM01); // tryb 2 w tabeli 38 (str. 83) - nie dojdzie do 255, tylko do OCR0
  TCCR0 |= (1<<CS01);// & (1<<CS00); // dzielenie przez 8 na preskalerze - timer 1MHz, ale /100, bo po dojściu do 100 (OCR0), czyli 10000Hz
  TIMSK = (1<<OCIE0); // oba bity zezwolic
  TIMSK |= (1<<TOIE0); // oba bity zezwolic
  // jakoś ładniej może da się te bity poustawiać? da się z = i + po prostu
  sei(); // przerwania w ogóle będą wykonywane
}

void loop() {
  Serial.println(start);
}
