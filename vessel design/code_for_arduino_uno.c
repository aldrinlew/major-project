#include <Servo.h>

/*
code for arduino bord ultrasonic radar system.
for more arduino projects --->  teamdiy.in
*/
             
Servo leftRightServo;         // set a variable to map the servo
int leftRightPos = 0;         // set a variable to store the servo position
const int numReadings = 10;   // set a variable for the number of readings to take
int index = 0;                // the index of the current reading
int total = 0;                // the total of all readings
int average = 0;              // the average
int echoPin = 7;              // the SRF05's echo pin
int initPin = 8;              // the SRF05's init pin
unsigned long pulseTime = 0;  // variable for reading the pulse
unsigned long distance = 0;   // variable for storing distance
 
/* setup the pins, servo and serial port */
void setup() {
  leftRightServo.attach(9);
  // make the init pin an output:
  pinMode(initPin, OUTPUT);
  // make the echo pin an input:
  pinMode(echoPin, INPUT);
  // initialize the serial port:
  Serial.begin(9600);
} 
 
/* begin rotating the servo and getting sensor values */
void loop() {
  for(leftRightPos = 0; leftRightPos < 180; leftRightPos++) {  // going left to right.
    leftRightServo.write(leftRightPos);
      for (index = 0; index<=numReadings;index++) {            // take x number of readings from the sensor and average them
        digitalWrite(initPin, LOW);
        delayMicroseconds(50);
        digitalWrite(initPin, HIGH);                           // send signal
        delayMicroseconds(50);                                 // wait 50 microseconds for it to return
        digitalWrite(initPin, LOW);                            // close signal
        pulseTime = pulseIn(echoPin, HIGH);                    // calculate time for signal to return
        distance = pulseTime/58;                               // convert to centimetres
        total = total + distance;                              // update total
        delay(10);
      }
    average = total/numReadings;                               // create average reading
 
    if (index >= numReadings)  {                               // reset the counts when at the last item of the array
      index = 0;
      total = 0;
    }
    Serial.print("X");                                         // print leading X to mark the following value as degrees
    Serial.print(leftRightPos);                                // current servo position
    Serial.print("V");                                         // preceeding character to separate values
    Serial.println(average);                                   // average of sensor readings
  }
  /*
  start going right to left after we got to 180 degrees
  same code as above
  */
  for(leftRightPos = 180; leftRightPos > 0; leftRightPos--) {  // going right to left
    leftRightServo.write(leftRightPos);
    for (index = 0; index<=numReadings;index++) {
      digitalWrite(initPin, LOW);
      delayMicroseconds(50);
      digitalWrite(initPin, HIGH);
      delayMicroseconds(50);
      digitalWrite(initPin, LOW);
      pulseTime = pulseIn(echoPin, HIGH);
      distance = pulseTime/58;
      total = total + distance;
      delay(10);
    }
    average = total/numReadings;
    if (index >= numReadings)  {
      index = 0;
      total = 0;
    }
    Serial.print("X");
    Serial.print(leftRightPos);
    Serial.print("V");
    Serial.println(average);
   }
}