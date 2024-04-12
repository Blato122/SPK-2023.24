// ardiuno lcd, pomioar ciągły, wynik na wyświetlaczu
// arduino ultrasonic sensor HC-SR04
// 340 m/s

// https://projecthub.arduino.cc/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-7cabe1

#include <LiquidCrystal.h>

#define WAITING 0
#define MEASURING 1
#define DISPLAYING 2

//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // numeracja 0 do 31 z tablicy!
LiquidCrystal lcd(0, 1, 2, 3, 4, 5); // LCD display initialization

const int trig_pin = 6;  
const int echo_pin = 7; 
// Then we declare 2 floats, duration and distance, which will hold the length of the sound wave and how far away the object is. 
float duration, distance; 
const byte start_button = 24;

byte clicked = WAITING;
float distance_saved = 0;

void print_default_message() {
  lcd.clear(); //new!!! 
  lcd.setCursor(0, 0);
  lcd.print("kliknij przycisk");
  lcd.setCursor(0, 1);
  lcd.print("aby rozpoczac"); 
}

void print_distance(float distance) {
  lcd.clear();
  lcd.setCursor(0, 0);    
  lcd.print("Odleglosc: ");  
  lcd.setCursor(0, 1);      
  lcd.print(distance); 
  lcd.print(" cm");
}

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);  
  print_default_message();
  
  pinMode(start_button, INPUT_PULLUP); // A0
  
  pinMode(trig_pin, OUTPUT);  
  pinMode(echo_pin, INPUT); 
  Serial.begin(9600);  
}

void loop() {  
  // send a 10 second pulse
  digitalWrite(trig_pin, LOW);  
  delayMicroseconds(2);  
  digitalWrite(trig_pin, HIGH);  
  delayMicroseconds(10);  
  digitalWrite(trig_pin, LOW);  
  
  //When the sound waves hit the receiver, it turns the Echo pin high for however long the waves were traveling for.  To get that, we can use a handy Arduino function called pulseIn() .  It takes 2 arguments, the pin you are listening to(In our case, the Echo pin), and a state(HIGH or LOW).  What the function does is waits for the pin to go whichever state you put in, starts timing, and then stops timing when it switches to the other state.  In our case we would put HIGH since we want to start timing when the Echo pin goes high.  We will store the time in the duration variable. (It returns the time in microseconds) 
  duration = pulseIn(echo_pin, HIGH);  
  //Now that we have the time, we can use the equation speed = distance/time , but we will make it time x speed = distance because we have the speed.  What speed do we have?  The speed of sound, of course!  The speed of sound is approximately 340 meters per second, but since the pulseIn() function returns the time in microseconds, we will need to have a speed in microseconds also, which is easy to get.  A quick Google search for "speed of sound in centimeters per microsecond" will say that it is .0343 c/μS.  You could do the math, but searching it is easier.  Anyway, with that information, we can calculate the distance!  Just multiply the duration by .0343 and then divide it by 2(Because the sound waves travel to the object AND back).  We will store that in the distance variable. 
  distance = (duration*.0343)/2;  
  
  byte start_button_state = PINA & 0b00000001; // read?
  
  if (start_button_state == LOW && clicked == WAITING) {
    clicked = MEASURING;
    lcd.clear();
    lcd.print("czekaj...");
    delay(1000);
  } else if (start_button_state == LOW && clicked == MEASURING) { 
    clicked = DISPLAYING;
    lcd.clear();
    lcd.print("czekaj...");
    delay(1000);
  } else if (start_button_state == LOW && clicked == DISPLAYING) {
    clicked = WAITING;
    lcd.clear();
    lcd.print("czekaj...");
    delay(1000);
  }
  Serial.println(clicked);
  
  if (clicked == WAITING) {
    print_default_message();    
    delay(100);
  } else if (clicked == MEASURING) {
    print_distance(distance);  
    delay(100); 
    distance_saved = distance;
  } else if (clicked == DISPLAYING) {
    print_distance(distance_saved);
    delay(100);
  }
  
}  

// instrukcja 5V zasilanie
//
