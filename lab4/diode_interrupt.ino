// kolos zad 5 - https://www.allaboutcircuits.com/technical-articles/adc-resolution-vs-adc-accuracy-subrange-adc-two-step-adc-and-total-unadjusted-error/

// kilka zrodel zegara, u nas 8MHz, można ustawic na inne Hz chyba lub podac z zewnatrz z jakiejs nóżki

// przerwania poukladanie wg hierarchii
// przerwanie po resecie - po wlaczeniu lub po nacisnieciu resetu. pobiera adres w funkcji gdzie ma sie zaczac program

// str 71 - timer i jakiś pwm
// tcnt0 - 8 bitow - po 256 krokach sie przepelnia licznuk
// moze wtedy podniesc flage ktora spowoduje wykonanie przerwania

// to chcemy:
// przerwanie nie wyedy gdy przepelnienie licznuka, a zrownanie licznika biezaczego timera z liczba w rejestrze OCR

//tccr0 - biezacy stan licznika
// tabela 38

// str 85 - tabela 42! opis bitow CS - skad timer bedzie bral zrodlo taktujace
// wszystkie 3 bity na 0 - timer stoi w miejscu - zatrzymanie timera tak mozna zrobic
// 001 - tak szybko, jak zegar procesora,bez dzielenia
// pin T0 opis gdzieś jest który to na początku -- strona 2 -- PB0

// wybor preskalera - jak szybko przepelni sie liczik albo jak szybko bedzie zliczal

// programoowanie licznika pole manewru jakie:
//zwiekszy sie o 1 po ilu klokach procesora - 1 klok to jedna linia kodu (no, instrukcja konkretnie)
// bez preskalowania, miedzy przepelnieniami mamy maksymalny czas wtedy
// z preskalerem np. /256, można zrobić 256k instrukcji
// procedura przerwania ma robic cos wiecej - cos tam ehh...

//tcnt0 - przechowuje wartosc licznika???

// ocr0 - jesli bedziemy koryzystac z czegos takiego, ze cos tam ... co przerwaniwe jakies
// nie czekamy na overflow tylko na porownanie - ocr0? aha, ok chyba, na rysunku
// mozliwosc wpisania tam roznych liczb, zeby ?????

// przerwanie zwiazane z przepelnieniem vs przerwanie INNE (rozne od poprzedniego) wyikajace z porownania

//pwm - ???

// rejestr timsk - do sterowania
// dwa najmlodsze bity - wazne - zezwolenie na to, zeby to urzadzenie, czyli timer0, mogl zglosic przerwanie (osobno
// interrupt i compare)

//globalny rejestr SREG - bit i - mozna ustawic robiac instrukcje sei(), ustawia na wysoki - najstarszy bit 128 tam wstawic


int n = 1; // byte tez ok, int ma 32k tu tylko
int i = 0;
const byte diode = PA0; // A0, okres 1 sekunda migania!

// overflow vector przy mode 00
ISR(TIMER0_COMP_vect) { // bez void - pobranie adresu z początku funkcji, tworzy funkcje w pamieci i pobiera adres gdzie dal ja w pamieci, jakieś reti
  // definiujemy tu przerwanie
  i++; // 10000 przerwań na sekundę
//  Serial.println(i);
  if (i == 10000) {
//    Serial.println("interrupt"); // więcej niż czas międyz przerwaniami (baud rate)
// częste przerwania - możemy tylko krótkie rzeczy robić!
    i = 0;
    PORTA ^= (1<<diode);//(0b00000001); // 1<<0?
    n = -n;
  }
  // https://www.google.com/search?q=arduino+interrupt+duration&sca_esv=582ec9845e97149d&ei=tlweZtzSMLSPxc8Po7WR8A4&ved=0ahUKEwjchdHRzMaFAxW0R_EDHaNaBO4Q4dUDCBA&uact=5&oq=arduino+interrupt+duration&gs_lp=Egxnd3Mtd2l6LXNlcnAiGmFyZHVpbm8gaW50ZXJydXB0IGR1cmF0aW9uMgYQABgWGB4yCxAAGIAEGIoFGIYDMgsQABiABBiKBRiGA0ifDFCTBVjwCnACeACQAQCYAV6gAZsFqgEBOLgBA8gBAPgBAZgCCqACswXCAg4QABiABBiKBRiGAxiwA8ICBRAAGIAEwgIFECEYoAHCAgcQIRgKGKABmAMAiAYBkAYDkgcCMTCgB-sd&sclient=gws-wiz-serp
}

void setup() {

  pinMode(diode, OUTPUT);
  Serial.begin(9600);
  n = 1;
  PORTA &= (0<<diode); //?
  DDRA |= (1<<diode);
  i = 0;
  
  TCNT0 = 0;
  OCR0 = 100; // 8bit
//  TCCR0 = 0b00001010;
  TCCR0 = (1<<WGM01); // tryb 2 w tabeli 38 (str. 83) - nie dojdzie do 255, tylko do OCR0
  TCCR0 |= (1<<CS01);// & (1<<CS00); // dzielenie przez 8 na preskalerze - timer 1MHz, ale /100, bo po dojściu do 100, czyli 10000Hz
  // 8MHz / 1024 = 7812.5Hz, /255 = 30.517578125 Hz
  TIMSK = (1<<OCIE0); // oba bity zezwolic
  TIMSK |= (1<<TOIE0); // oba bity zezwolic
  // jakoś ładniej może da się te bity poustawiać? da się z = i + po prostu
  sei(); // przerwania w ogóle będą wykonywane
}

// str 45 wagi przerwań!! adres 2 bajty

void loop() {
  // timer0 jaka CZESTTOTLIWOSC W KONCU! 1 czy mhz
  //  Serial.println(i); 
//  Serial.print(F_CPU / 1000000UL);
//  Serial.println(" MHz"); // 8MHz - ok
  // spodziewamy sie, ze w nieskonczonosc bedzie wysylal nam to samo, z liczba sie nic wiecej nie stanie
  // a jesli sie stanie, to oznacza, ze n zmienilo wartosc
}
