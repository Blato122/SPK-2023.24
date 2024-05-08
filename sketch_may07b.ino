#include <OneWire.h>

// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// https://github.com/milesburton/Arduino-Temperature-Control-Library

OneWire ds(0);  // on pin 0 (a 4.7K resistor is necessary)

const byte heat = PB1;

void setup(void) {
  Serial.begin(9600);
  pinMode(heat, OUTPUT); // kiedy pullup?
}

float mean = -3.4028235E38;
int s = 0;

void loop(void) {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[9];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
  
  // basic version: (works fine)
//  if (celsius < 28.0) {
//    digitalWrite(heat, HIGH); // 0 is high??
//  } else {
//    digitalWrite(heat, LOW);
//  }
//  Serial.println(digitalRead(heat));  
  
  // PWM:
  // https://docs.arduino.cc/learn/microcontrollers/analog-output/
//  float amplifier = 150.0; // 27.5 max, oscyluje między 27.1, a 27.5 mniej więcej, mało kiedy się stosuje to grzanie proporcjonalne wtedy.
//  // w bojlerze się stosuje 0/1 bo tam lekko można przekroczyć temperaturę
//  float goal = 28.0;
//  int power = amplifier * (goal - celsius); // + s? i dać że mniej niż 0 być nie może?
//  analogWrite(heat, power % 256);
//  
//  Serial.println(power);
//  Serial.println(power % 256);

  // PID: 
  // srednia z n ostatnich pomiarow (np. 20/30 przy kroku sekundowym)
  // patrzymy czy sie utrzymuje na zadanym poziomie, czy nie
  
//  s = (celsius >= mean) ? s-1 : s+1;
  float goal = 28.0;

  if (goal >= mean){ //&& (mean > 0.9 * goal || mean < 1.1 * goal)) {
    ++s;
  } else {
    --s;
  }
  
  if (s > 255) { // -255 też?
    s = 255;
  } else if (s < -255) {
    s = -255; 
  }
  if (mean == -3.4028235E38) {
   mean = celsius; 
  }
  mean = 0.95 * mean + 0.05 * celsius; // 20
  
  float amplifier = 150.0; // 27.5 max, oscyluje między 27.1, a 27.5 mniej więcej, mało kiedy się stosuje to grzanie proporcjonalne wtedy.
  // w bojlerze się stosuje 0/1 bo tam lekko można przekroczyć temperaturę
  int power = amplifier * (goal - celsius) + s; // i dać że mniej niż 0 być nie może?
  
  if (power < 0) {
    power = 0;
  } else if (power > 255) {
    power = 255;
  }
  
  analogWrite(heat, power % 256);
 
  Serial.println(power);
  Serial.println(mean);
  Serial.println(s);

}

// za tydz 14 kol: i 28 maja ostatnie zajęcia - pewnie PWM, przerwania, rejestry przerwań i one wire?
// projekt licxnil przxerwania + PRZYCISK 
// onewire - podgrzewac do temp np 30 st i potem off i on znowu jak spadnie lekko itd 
// timer odlicza co okolo 2.5 s a nie 1s


// smart sum: https://forum.arduino.cc/t/efficient-way-of-buffering-and-averaging-values/513790/4
