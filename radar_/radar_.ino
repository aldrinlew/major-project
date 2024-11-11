#include <Servo.h>  // Include the Servo library

int trigPin = 2;    // Trigger pin of the ultrasonic sensor
int echoPin = 3;    // Echo pin of the ultrasonic sensor
int distance;       // Variable to store the distance measured by the sensor
Servo servo;        // Create a servo object to control a continuous rotation servo

void setup() {
  pinMode(trigPin, OUTPUT);   // Set trigPin as an OUTPUT
  pinMode(echoPin, INPUT);    // Set echoPin as an INPUT
  Serial.begin(9600);         // Initialize serial communication
  servo.attach(9);            // Attach the servo to pin 9
}

void loop() {
  left();    // Call the function to rotate the servo left
  delay(1000); // Delay for 1 second
  stopServo(); // Stop the servo
  delay(500); // Delay for 1 second
  right();   // Call the function to rotate the servo right
  delay(1000); // Delay for 1 second
  stopServo(); // Stop the servo
  delay(500); // Delay for 1 second
}

void left() {
  servo.write(0);  // Rotate left (counterclockwise at maximum speed)
  Serial.println("Rotating left");
  delay(500);      // Rotate for a short period
  distance = echoloop(); // Measure the distance
  Serial.print("Distance: ");
  Serial.println(distance);
}

void right() {
  servo.write(180);  // Rotate right (clockwise at maximum speed)
  Serial.println("Rotating right");
  delay(500);        // Rotate for a short period
  distance = echoloop(); // Measure the distance
  Serial.print("Distance: ");
  Serial.println(distance);
}

void stopServo() {
  servo.write(90);  // Stop the servo
  Serial.println("Servo stopped");
}

int echoloop() {
  long duration, cm;                         // Declare variables for duration and distance
  digitalWrite(trigPin, LOW);                // Ensure trigger pin is LOW
  delayMicroseconds(2);                      // Wait for 2 microseconds
  digitalWrite(trigPin, HIGH);               // Set trigger pin HIGH
  delayMicroseconds(10);                     // Wait for 10 microseconds
  digitalWrite(trigPin, LOW);                // Set trigger pin LOW
  duration = pulseIn(echoPin, HIGH);         // Read the signal from echo pin
  cm = duration / 29 / 2;                    // Convert the time into a distance
  return cm;                                 // Return the distance
}
