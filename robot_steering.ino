#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

//#include <LiquidCrystal.h>

//LiquidCrystal lcd(0, 1, 2, 3, 4, 5); // LCD display initialization

const byte LF_backward_wheel = 11;
const byte LR_backward_wheel = 12;

const byte RF_forward_wheel = 13;
//const byte LR_backward_wheel = 10;

const byte trig_pin = 14;  
const byte echo_pin = 15; 

void setup() {
  pinMode(LF_backward_wheel, OUTPUT);
  pinMode(LR_backward_wheel, OUTPUT);
  pinMode(RF_forward_wheel, OUTPUT);
  
  // set up the LCD's number of columns and rows:
//  lcd.begin(16, 2);  
  
  pinMode(trig_pin, OUTPUT);  
  pinMode(echo_pin, INPUT_PULLUP); // chyba z pullup??; 
  
  Serial.begin(9600); // 9600 - bits per sec - port szeregowy
}

//void print_distance(float distance) {
//  lcd.clear();
//  lcd.setCursor(0, 0);    
//  lcd.print("Odleglosc: ");  
//  lcd.setCursor(0, 1);      
//  lcd.print(distance); 
//  lcd.print(" cm");
//}

float get_distance() {
  return 0.0;
}

//enum action {
//  FORWARD, BACKWARD, STOP
//};

float duration, distance;
byte spd = 0;

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
//  print_distance(distance);
  Serial.println(distance);
  delay(500);
  
  // ----------------------------------------
  // liquid crystal można usunąć
  
  if (distance < 10) {
    analogWrite(RF_forward_wheel, 0);
    // turn 180 degrees
    delay(2000);
    analogWrite(LF_backward_wheel, 255);
    analogWrite(LR_backward_wheel, 255);
    analogWrite(RF_forward_wheel, 255);
    // i jak długo to ma się obracać?!?!?
//    analogWrite(RR_forward_wheel, 255);
    // jak zrobić, żeby dane koło mogło poruszać się w obie strony?
    // jak zrobić, żeby robot mógł jeździć? radio moduł? pilot?
  } else {
    analogWrite(RF_forward_wheel, 255);
  }
   
//  } else if (distance < 20) {
//    analogWrite(LF_forward_wheel, 128); // pwm?
//  } 
//  else {
//    analogWrite(LF_forward_wheel, spd++);
//      Serial.println(spd);
//  }
}

//start - 128
//max - ?
