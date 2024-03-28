void setup() {
  // NIE MOŻE BYĆ NA D GDY KORZYSTAMY Z PORTU SZEREGOWEGO!!!
  DDRC = 255; // port C ustawiony na wyjście - segmenty
  PORTC = 255; // początkowo nie zasilaj żadnego segmentu
 
  DDRB = 255; // port B ustawiony na wyjście - cyfry
  PORTB = 255; // początkowo nie zasilaj żadnej cyfry
   
  // potencjometr na port A
 
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

void display_number(int dec) {
  unsigned long start = 0;
  unsigned long stop_ = 0;
  unsigned long elapsed_time = 0;
  
  start = millis();

  // wyświetlaj bieżącą liczbę dopóki nie minie 100ms (ładniejsze wyświetlanie - nie miga tak)
  while (elapsed_time <= 100) {
 
    //Serial.println(dec);  
    const byte split_digits[] = {dec % 10, (dec / 10) % 10, (dec / 100) % 10, dec / 1000};
 
    for (byte i = 0; i <= 3; ++i) {
      byte segments = number_segments[split_digits[i]];
//      Serial.println(split_digits[i]);
     
     if (i == 2) {
        segments |= H; // decimal point
      } 
     
      PORTB = ~(1 << i);
      PORTC = ~segments;
      delay(1);
    }

    stop_ = millis();
    elapsed_time = stop_ - start;
//    Serial.println(elapsed_time);
   
  }
  PORTB = 255;
 
}

int analog_read() { 
  // short zamiast int
  // https://www.electronicwings.com/avr-atmega/atmega1632-adc
  ADMUX = 0b11000000; // wynik dosunięty do dołu (prawej) (to ten ADLAR??? - chyba tak - str. 220) - wybór kanału z napięciem referencyjnym (strona 217) - 0b11... -> wewnętrzne 2.56V (można też jakoś z zewnątrz, jak ktoś chce)
  ADCSRA = 0b11000000; // str. 219 - opis bitów, nie chcemy przerwań, ani auto trigger - na 1 dane jest wyzwolenie przetwarzania (bit 6) i włączenie przetwornika (bit 7)
  // czekać 1ms żeby przetwarzanie się na pewno skończyło
  // w ADMUX channel ustawiony na 0 bo korzystamy z A0 - gdyby był inny to może na tych 5 najmłodszych (MUX0-MUX4) bitach trzeba by coś większego dać
  // str. 218 tabelka
  while((ADCSRA&(1<<ADIF))==0);	/* Monitor end of conversion interrupt */
  
//  delay(1); //  już niepotrzebne bo mamy while powyżej z ADIF - dokumentacja strona 219
  unsigned long a = (int)ADCL; // unsigned bo zwykły int ma 2 bajty i się nie mieści już przy 30k!!! i nawet to za mało, bo 2**15 to 32k, a musi być zakres do ponad 100k
  unsigned long b = (int)ADCH * 256; // bo to tak naprawdę bity 8-15 
  
  return ((a + b) * 100 / 1023) * 5; // od 0 do 5 V
}

const int analog_pin = 0; // A0 - numeracja portów analogowych jest inna
int val = 0; // 0 - 1023 - 10 bitowy przetwornik

void loop() {
  // adc hi i adc lo
  // adc mux - wpisac 0 - wybierze to zerowy kanał - dollny bajt jest pelny - gorny ma tylko 2 starsze bity odczytu
  // szybkie przetwarzanie cos tam
  // do adc a wpisac zeby 7 bit byl ustawiony
  // dwa najstarze bity wystarczy ustawic
  // wyzwalanie przetwarzania, cos tam, potem mozna odczytac wartosc z rejestru
  // miedzy wpis do adc sra a odczytaniem, zachowac delay 1ms zeby miec pewnosc ze przetwarzanie sie zakonczy
  val = analog_read();
//  Serial.println(val);
//  Serial.println(ADIF); // 4
  display_number(val);
}

// czym szybviej pracuje przetwornik, tym większy szum
// adlar - align do lewej - można użyć tylko 8 bitów zamiast 10 i odrzucic te 2 najmlodsze z szumem (najwazniejsze sa 2 najstarsze bity) - str 220
// 1/4ms - max czas konwersji w przetworniku wiec delay 1 jest ok
