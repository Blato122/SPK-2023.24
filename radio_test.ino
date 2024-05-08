///*
// * See documentation at https://nRF24.github.io/RF24
// * See License information at root directory of this library
// * Author: Brendan Doherty (2bndy5)
// */
//
///**
// * A simple example of sending data from 1 nRF24L01 transceiver to another.
// *
// * This example was written to be used on 2 devices acting as "nodes".
// * Use the Serial Monitor to change each node's behavior.
// */
//#include <SPI.h>
//#include "printf.h"
//#include "RF24.h"
//
//#define CE_PIN 7
//#define CSN_PIN 8
//// instantiate an object for the nRF24L01 transceiver
//RF24 radio(CE_PIN, CSN_PIN);
//
//// Let these addresses be used for the pair
//uint8_t address[][6] = { "1Node", "2Node" };
//// It is very helpful to think of an address as a path instead of as
//// an identifying device destination
//
//// to use different addresses on a pair of radios, we need a variable to
//// uniquely identify which address this radio will use to transmit
//bool radioNumber = 1;  // 0 uses address[0] to transmit, 1 uses address[1] to transmit
//
//// Used to control whether this node is sending or receiving
//bool role = false;  // true = TX role, false = RX role
//
//// For this example, we'll be using a payload containing
//// a single float number that will be incremented
//// on every successful transmission
//float payload = 0.0;
//
//void setup() {
//
//  Serial.begin(115200);
//  while (!Serial) {
//    // some boards need to wait to ensure access to serial over USB
//  }
//
//  // initialize the transceiver on the SPI bus
//  if (!radio.begin()) {
//    Serial.println(F("radio hardware is not responding!!"));
//    while (1) {}  // hold in infinite loop
//  }
//
//  // print example's introductory prompt
//  Serial.println(F("RF24/examples/GettingStarted"));
//
//  // To set the radioNumber via the Serial monitor on startup
//  Serial.println(F("Which radio is this? Enter '0' or '1'. Defaults to '0'"));
//  while (!Serial.available()) {
//    // wait for user input
//  }
//  char input = Serial.parseInt();
//  radioNumber = input == 1;
//  Serial.print(F("radioNumber = "));
//  Serial.println((int)radioNumber);
//
//  // role variable is hardcoded to RX behavior, inform the user of this
//  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
//
//  // Set the PA Level low to try preventing power supply related problems
//  // because these examples are likely run with nodes in close proximity to
//  // each other.
//  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.
//
//  // save on transmission time by setting the radio to only transmit the
//  // number of bytes we need to transmit a float
//  radio.setPayloadSize(sizeof(payload));  // float datatype occupies 4 bytes
//
//  // set the TX address of the RX node into the TX pipe
//  radio.openWritingPipe(address[radioNumber]);  // always uses pipe 0
//
//  // set the RX address of the TX node into a RX pipe
//  radio.openReadingPipe(1, address[!radioNumber]);  // using pipe 1
//
//  // additional setup specific to the node's role
//  if (role) {
//    radio.stopListening();  // put radio in TX mode
//  } else {
//    radio.startListening();  // put radio in RX mode
//  }
//
//  // For debugging info
//  // printf_begin();             // needed only once for printing details
//  // radio.printDetails();       // (smaller) function that prints raw register values
//  // radio.printPrettyDetails(); // (larger) function that prints human readable data
//
//}  // setup
//
//void loop() {
//
//  if (role) {
//    // This device is a TX node
//
//    unsigned long start_timer = micros();                // start the timer
//    bool report = radio.write(&payload, sizeof(float));  // transmit & save the report
//    unsigned long end_timer = micros();                  // end the timer
//
//    if (report) {
//      Serial.print(F("Transmission successful! "));  // payload was delivered
//      Serial.print(F("Time to transmit = "));
//      Serial.print(end_timer - start_timer);  // print the timer result
//      Serial.print(F(" us. Sent: "));
//      Serial.println(payload);  // print payload sent
//      payload += 0.01;          // increment float payload
//    } else {
//      Serial.println(F("Transmission failed or timed out"));  // payload was not delivered
//    }
//
//    // to make this example readable in the serial monitor
//    delay(1000);  // slow transmissions down by 1 second
//
//  } else {
//    // This device is a RX node
//
//    uint8_t pipe;
//    if (radio.available(&pipe)) {              // is there a payload? get the pipe number that recieved it
//      uint8_t bytes = radio.getPayloadSize();  // get the size of the payload
//      radio.read(&payload, bytes);             // fetch payload from FIFO
//      Serial.print(F("Received "));
//      Serial.print(bytes);  // print the size of the payload
//      Serial.print(F(" bytes on pipe "));
//      Serial.print(pipe);  // print the pipe number
//      Serial.print(F(": "));
//      Serial.println(payload);  // print the payload's value
//    }
//  }  // role
//
//  if (Serial.available()) {
//    // change the role via the serial monitor
//
//    char c = toupper(Serial.read());
//    if (c == 'T' && !role) {
//      // Become the TX node
//
//      role = true;
//      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
//      radio.stopListening();
//
//    } else if (c == 'R' && role) {
//      // Become the RX node
//
//      role = false;
//      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
//      radio.startListening();
//    }
//  }
//
//}  // loop

// sprawdzić co w ogóle oznaczają te porty!!!

//#include <SPI.h>
//#include <nRF24L01.h>
//#include <RF24.h>
//
//RF24 radio(0, 1); // CE, CSN
//
//const byte address[6] = "00001";
//
//void setup() {
//  radio.begin();
//  radio.openWritingPipe(address);
//  radio.setPALevel(RF24_PA_MIN);
//  radio.stopListening();
//}
//
//void loop() {
//  const char text[] = "Hello World";
//  radio.write(&text, sizeof(text));
//  delay(1000);
//}

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(0, 1); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
#define led 2

void setup() {
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MIN);
//  Serial.println("setup");
//  nie można z serial korzystać przy tym??
  pinMode(led, OUTPUT);

}

byte action = 0;
float distance = 0.0;  
int i = 0;

void loop() {
//  delay(5);

//  Serial.println("enter loop");
  radio.stopListening();
  if (i++ % 2 == 0) {
   distance = 25.0; 
  } else {
   distance = 15.0; 
  }
//  const char text[] = "Hello World";
//  Serial.print("sending: ");
//  Serial.println(distance);
  radio.write(&distance, sizeof(distance));

  delay(1000);

  radio.startListening();
  while (!radio.available());
  radio.read(&action, sizeof(action));
  digitalWrite(led, action);
//  Serial.println(action);
}

// usunąłem trzy funkcje printDetails() i printPrettyDetails() i jeszcze jakąś trzecią
// bo dawały błąd! ale może je kiedyś dodać znowu

// https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/?utm_content=cmp-true
// https://docs.arduino.cc/learn/communication/spi/?_gl=1*1qwhn27*_ga*MjA5MzAyMjc2NS4xNzA5MDY2MjQy*_ga_NEXN8H46L5*MTcxNTE3MzQ2MS4zMC4xLjE3MTUxNzYwMTAuMC4wLjY1MzI4MDc1MA..*_fplc*ZnZ3SzF3Z1ZnMUZadUpKJTJCN3VGZENwTThNR0J3ZHMlMkJqT3FEZVVsUU81S0hKUiUyRlV4ZEdlODVySWdJb0xvNjlPTkZYWGNtakh6Y0UwbUJIdkZIdiUyRkcxMmhnanZsM3dBazVGRVFuUnBkZFFhYzFWdUttYUI3MVhoUkVOQmNvNlElM0QlM0Q.

// okienko czasowe na odbioe komunikatu

// koła w tył - czerwony masa, czarny 5v
// koła w przód - na odwrót

//trzy napięcia pod niebieską płytką to 3.3v
