void setup() {
  DDRB = 225; // z każdego portu sterujemy, a nie odbieramy
  PORTB = 1; // co ustawić
}

void loop() {
  delay(1000);

  for (int i = 0; i <= 7; ++i) {
    PORTB = ~(1 << i);
    delay(1000);
  }

  for (int i = 7; i >= 0; --i) {
    PORTB = ~(1 << i);
    delay(1000);
  }
}
